# W.H.A.T. I.F. Timeline System Documentation

## 1. Core Concept: Timeline as Directed Acyclic State Graph

The Timeline System is the backbone of the "What If" simulation engine. It allows users to explore different choices, simulation outcomes, and narrative branches without losing previous states. 

Conceptually, the timeline is a Directed Acyclic Graph (DAG) where:
- **Nodes** represent a discrete state of the world at a specific point in time.
- **Edges** represent the transition (an event, a combat encounter, a narrative choice) that moved the world from one state to another.

Because the system is deterministic, any node can be used as a restoration point to spawn new branches.

---

## 2. Data Structures

### 2.1 `FWhatIfTimelineNode`

The fundamental building block of the timeline.

```cpp
USTRUCT(BlueprintType)
struct FWhatIfTimelineNode
{
    GENERATED_BODY()

    // Unique identifier for this node.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Timeline Node")
    FGuid NodeID;

    // The ID of the node that preceded this one. Invalid GUID if root.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Timeline Node")
    FGuid ParentID;

    // List of nodes that branch off from this state.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Timeline Node")
    TArray<FGuid> ChildrenIDs;

    // Human-readable or auto-generated name of the branch/node.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline Node")
    FString NodeLabel;

    // The compressed binary snapshot of the entire simulation state at this point.
    UPROPERTY()
    TArray<uint8> WorldStateSnapshot;

    // The ID of the event or scenario that led to this state.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Timeline Node")
    FName TransitionEventID;

    // Simulation timestamp in seconds.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Timeline Node")
    float Timestamp;
};
```

---

## 3. How a Branch is Created

Branching occurs when an action is taken that alters the state of the world from an existing historical node rather than the current active leaf node.

1. **Select Parent Node:** The user or simulation designates a specific `NodeID` to branch from.
2. **Restore State:** The engine deserializes the `WorldStateSnapshot` of the parent node into memory, replacing the current world state.
3. **Execute Transition:** A new event or choice is injected (e.g., "What if Character A went left instead of right?").
4. **Generate New Node:** Once the new scenario resolves, a new `FWhatIfTimelineNode` is instantiated.
5. **Link:** The new node's `ParentID` is set to the selected parent, and the new node's `NodeID` is appended to the parent's `ChildrenIDs`.

---

## 4. World State Snapshot

To guarantee that branching works flawlessly, the world state must be completely serialized.
The serialization process involves:
- **Character States:** Health, inventory, position, relationship values, memory flags.
- **Environment States:** Destroyed objects, time of day, active modifiers.
- **Subsystem States:** Combat seeds, active quests.

This is achieved using Unreal Engine's `FMemoryWriter` and `FObjectAndNameAsStringProxyArchive` to serialize all relevant `UObject` and `UStruct` data tagged with `SaveGame` properties into the `WorldStateSnapshot` byte array.

---

## 5. Timeline Graph Traversal and Exploration

The graph can be traversed rapidly to preview alternate histories.
Algorithms provided in `UTimelineManager`:
- `GetPathToRoot(FGuid NodeID)`: Returns the chronological sequence of nodes from the start of the simulation to the specified node.
- `GetLeaves(FGuid NodeID)`: Finds all ultimate conclusion nodes stemming from a specific point.
- `GetCommonAncestor(FGuid NodeA, FGuid NodeB)`: Useful for UI to show where two distinct timelines diverged.

---

## 6. Reproducibility Guarantee

Because the combat and scenario systems are perfectly deterministic, a `WorldStateSnapshot` does not technically need to store *every* intermediate frame. It only needs to store the seed and the initial conditions. Re-running the simulation from a node with the exact same inputs will always produce the exact same subsequent node. 

However, for performance, we snapshot the state at the *end* of major transitions to avoid re-simulating hours of background logic.

---

## 7. Branch Naming and Labeling

Branches are automatically labeled based on the transition event.
Examples:
- "Encounter: Forest Ambush (Victory)"
- "Dialogue: Betrayed the King"

Users can manually rename nodes in the UI to keep track of their exploration (e.g., "The Good Ending Path", "Experiment 1").

---

## 8. Pruning

Timelines can grow exponentially. The pruning subsystem allows cleanup of unwanted branches.
- **Manual Pruning:** The user selects a node and deletes it. All children are recursively deleted.
- **Orphan Cleanup:** Garbage collection routinely purges `WorldStateSnapshots` for nodes that have been detached from the main graph.
- **Auto-Pruning (Optional):** Limit graph depth. If a branch reaches depth 50, the root nodes can be "baked" to save memory.

---

## 9. Save/Load Integration

The entire timeline graph is serializable to disk. 
The save file structure is essentially a serialized array of `FWhatIfTimelineNode` structs, along with metadata (thumbnail images, play time).
When loading a save, the engine rebuilds the DAG in memory and sets the active context to the most recently modified leaf node.

---

## 10. UI: Timeline Explorer Design

The Timeline Explorer is a core UI feature built in UMG.
- **Visual Representation:** Nodes are rendered as connected circles in a 2D panning canvas.
- **Color Coding:** Nodes are colored by outcome type (e.g., Green for survival, Red for death, Blue for narrative pivot).
- **Interaction:** Hovering over a node displays a tooltip with the `NodeLabel` and a summary of state changes. Clicking a node sets it as the active state for the world.

---

## 11. Future: Timeline Merging, Parallel Simulation

**Roadmap Features:**
- **Parallel Simulation:** Processing multiple branches asynchronously on background threads to pre-calculate outcomes of binary choices before the user even clicks them.
- **Timeline Merging (Theoretical):** Detecting if two distinct branches result in identical world states (a convergence), allowing the DAG to join paths. This is highly complex due to minor state variances (e.g., slightly different HP values) and is currently slated for post-launch.

---

## 12. Data Persistence

Snapshots can become very large. To mitigate memory overhead:
- Snapshots are compressed using Zlib before being stored in RAM.
- For extremely large graphs, only the nodes on the active path are kept in RAM; alternate branches are serialized to a temporary swap file on disk and loaded synchronously when requested.

---

## 13. Relationship between Timeline and Scenario Systems

The Scenario System dictates *what happens* at a node, while the Timeline System dictates *how it connects* to the past and future.
When a Scenario completes, it emits an `FWhatIfEvent`. The Timeline System intercepts this event, takes a snapshot, creates a Node, and waits for the next Scenario injection.

---

## 14. Advanced Concurrency and Branch Management

Managing a directed acyclic graph that scales infinitely requires robust concurrency checks. 
- **Read-Write Locks:** The DAG uses an `FRWLock` internally. Since multiple background threads may be attempting to evaluate "What If" scenarios passively, they acquire Read locks on the Timeline nodes. 
- **Thread Safety:** When a new `FWhatIfTimelineNode` is committed (a Write operation), all speculative background simulations branching from that active node are momentarily paused to ensure atomic insertion.

### 14.1 Background Simulation Worker

The system leverages `FRunnable` to spin up a worker thread:
```cpp
class WHATIF_API FTimelineSpeculationWorker : public FRunnable
{
public:
    virtual uint32 Run() override;
    virtual void Stop() override;
    
    // Pushes a speculative branching task to the queue
    void EnqueueSpeculation(const FGuid& ParentNode, const FName& ScenarioID);
};
```
By keeping this logic strictly detached from the Unreal Game Thread, the UI remains perfectly fluid even when 10 different alternate timelines are being simulated out to a depth of 5 moves ahead.

---

## 15. Garbage Collection and Memory Management

A critical constraint of this engine is RAM usage. Unchecked, taking full snapshots of a complex RPG world state every few seconds will exhaust memory.

### 15.1 Delta Encoding
Instead of full snapshots for every node, the Timeline System employs delta encoding for children.
1. A Keyframe Node contains a full `WorldStateSnapshot`.
2. A Delta Node contains only the serialized bytes that *changed* between its parent and itself.
3. Every 10th node is forced to be a Keyframe Node.

When `UTimelineManager::RestoreState(FGuid NodeID)` is called:
- If `NodeID` is a Keyframe, it is loaded directly.
- If `NodeID` is a Delta, the system traverses up the parent chain until it hits a Keyframe, deserializes the Keyframe, and then sequentially applies the Deltas downwards until it rebuilds the target state in RAM.

---

## 16. Event Pub/Sub Integration

The Timeline System relies on a robust Publish/Subscribe pattern to decouple the UI from the underlying graph logic.

- `OnTimelineNodeCreated(const FWhatIfTimelineNode& NewNode)`: Fired whenever a branch is added. The Timeline Explorer UI listens to this to animate the drawing of a new node circle.
- `OnTimelineStateRestored(const FGuid& RestoredNode)`: Fired when the user selects a node to jump to. The Level Sequence Manager, Audio System, and Combat System listen to this to reset their local states to match the snapshot.
- `OnBranchPruned(const FGuid& PrunedRoot)`: Fired when nodes are deleted, allowing UI components to clean up their cached references.
