# W.H.A.T. I.F. Scenario System Documentation

## 1. What a Scenario Is

In W.H.A.T. I.F., a Scenario is a highly configurable, self-contained slice of simulation logic. It defines a specific situation—an encounter, an exploration phase, or a puzzle—that characters must navigate. 

Scenarios dictate the starting conditions, the objectives, and the constraints of a specific segment of the timeline. They are the building blocks that the Timeline System links together.

---

## 2. UWhatIfScenarioData

Scenarios are defined as Unreal Engine Primary Data Assets. This allows designers to create and configure scenarios in the editor without writing C++.

```cpp
UCLASS(BlueprintType)
class WHATIF_API UWhatIfScenarioData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Scenario")
    FName ScenarioID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Scenario")
    FText ScenarioName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Scenario")
    FText ScenarioDescription;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Requirements")
    int32 RequiredCharacterCount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Environment")
    FWhatIfEnvironmentData EnvironmentData;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category="Objectives")
    TArray<UWhatIfObjective*> Objectives;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category="Constraints")
    TArray<UWhatIfConstraint*> Constraints;
};
```

---

## 3. FWhatIfEvent Struct

During a scenario's execution, notable things that happen are emitted as events.

```cpp
USTRUCT(BlueprintType)
struct FWhatIfEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FName EventType; // e.g., "DoorOpened", "ItemFound"

    UPROPERTY(BlueprintReadOnly)
    FName ActorID;

    UPROPERTY(BlueprintReadOnly)
    FString EventDetails;
    
    UPROPERTY(BlueprintReadOnly)
    float Timestamp;
};
```

---

## 4. Objective System

Objectives are conditions that determine if a scenario is a success or failure.
Implemented as `UObject` subclasses with an `IsMet(const FWhatIfWorldState& State)` function.
- **EliminationObjective:** Requires all hostile entities to reach 0 HP.
- **SurvivalObjective:** Requires at least one friendly character to survive for N turns.
- **ExtractionObjective:** Requires characters to reach a specific coordinate.

---

## 5. Constraint System

Constraints impose rules on the scenario that alter how mechanics function or limit what can be done.
- **TimeLimitConstraint:** Scenario automatically fails if not completed in X seconds/turns.
- **PacifistConstraint:** Scenario fails instantly if any damage is dealt.
- **ResourceDraftConstraint:** Characters start with 0 Mana/Energy regardless of their timeline state.

---

## 6. Environment Data Integration

The `FWhatIfEnvironmentData` struct embedded in the Scenario Data dictates the spatial and atmospheric context.
- Provides the layout data (grid size, obstacle coordinates).
- Provides modifiers (temperature, visibility) that hook directly into the Combat System.
- Used by the Narrative System to establish the setting.

---

## 7. How Scenarios Reference Characters

Scenarios do not hardcode character identities. Instead, they define "Roles".
A scenario might require:
- Role A: "The Protagonist"
- Role B: "The Companion"
- Role C: "The Antagonist"

When the Timeline System executes a scenario, it binds specific character instances from the `WorldStateSnapshot` to these roles. This makes scenarios highly modular. A "Bar Fight" scenario can be played with any two characters mapped to the roles.

---

## 8. Scenario Execution Lifecycle

1. **Load:** The Scenario System loads the `UWhatIfScenarioData`.
2. **Bind:** Characters from the current timeline node are mapped to the scenario roles.
3. **Initialize:** Environment modifiers and constraints are applied. Objectives are registered.
4. **Simulate:** The loop begins (either delegating to the Combat System or running abstract simulation logic).
5. **Evaluate:** After every tick/turn, check if Objectives are met or Constraints are violated.
6. **Finalize:** Emit the `FWhatIfSimulationResult`, triggering the Timeline and Narrative systems.

---

## 9. Modular Scenario Design

To maximize replayability, scenarios can include randomized elements configured via weights.
- A "Forest Patrol" scenario might have a 50% chance to spawn 2 enemies, and a 50% chance to spawn 4.
- Because the overarching system is seeded, this random choice is rolled using the timeline node's seed, preserving determinism while allowing designer-authored variance.

---

## 10. Scenario Editor (Future)

**Roadmap:**
A custom Unreal Editor tool (built with Slate/UMG) to author scenarios visually.
- Drag-and-drop objectives and constraints.
- A visual node graph to define multi-stage scenarios (e.g., Stage 1: Sneak, Stage 2: Combat).
- A testing harness to immediately run the scenario with dummy characters and view the resultant Narrative output.

---

## 11. Validation

Before a scenario runs, `ValidateScenario()` is called.
Checks include:
- Do the bound characters meet the minimum requirements? (e.g., if a scenario requires someone who can cast magic, is the bound character a mage?)
- Are all objectives reachable?
- If validation fails, the scenario cannot be attached to the timeline.

---

## 12. Example Scenario Template

**ID:** `SCENARIO_Ambush_01`
**Name:** Roadside Ambush
**Description:** The party is attacked while traveling.
**Roles:**
- 1x Target (Player party)
- 3x Attackers (Hostile NPCs)
**Environment:** `ENV_DirtRoad_Night`
**Objectives:**
- `Obj_EliminateAllAttackers`
**Constraints:**
- `Const_NoRetreat` (Characters cannot leave the bounds)

---

## 13. Dynamic Scenario Injection

While most scenarios are authored data assets, the W.H.A.T. I.F. engine supports procedural scenario generation. The "Scenario Director" subsystem evaluates the current world state and can construct a `UWhatIfScenarioData` object at runtime.

### 13.1 Director Evaluation
The Director looks at:
- **Tension Curve:** Has it been too long since a combat encounter?
- **Character Relationships:** Are two party members currently angry at each other?
- **Resource Levels:** Is the party flush with health potions, or starving?

Based on these metrics, the Director might dynamically assemble a scenario:
> *The party is starving. The Director spawns a "Foraging" scenario, but adds a high-level predator into the `EnvironmentData` to spike the tension.*

---

## 14. The Scenario Flow Graph

For complex encounters, a single scenario might not suffice. The engine utilizes a Scenario Flow Graph (SFG) to link sequential micro-scenarios together.

```cpp
UCLASS()
class WHATIF_API UScenarioFlowGraph : public UObject
{
    GENERATED_BODY()

public:
    // Evaluates transition conditions to pick the next scenario
    UWhatIfScenarioData* GetNextScenario(const FWhatIfSimulationResult& PreviousResult);
};
```

**Example SFG:**
1. **Scenario 1:** The Infiltration (Stealth objectives).
   - *If Stealth fails -> Transition to Scenario 2A (Combat).*
   - *If Stealth succeeds -> Transition to Scenario 2B (Assassination).*

This provides a middle-ground between the infinite freedom of the open-ended Timeline System and the structured narrative of a traditional RPG quest.

---

## 15. Integrating Dialogue Constraints

Scenarios can enforce constraints not just on combat, but on dialogue choices presented to the Narrative Engine.

A scenario titled `SCENARIO_HostageNegotiation` might include a `UDialogueConstraint`. This constraint forces the LLM Narrative System to only generate "Aggressive" or "Placating" dialogue options, disabling "Humorous" responses entirely. This ensures tonal consistency when the authored scenario demands gravity.

---

## 16. Unit Testing Scenarios

Because scenarios are deterministic data assets, they are perfectly suited for automated CI/CD testing.

The `FScenarioTestingHarness` runs overnight, booting up every `UWhatIfScenarioData` in the project. It feeds them varying initial `FWhatIfCombatSetup` seeds and verifies:
1. Does the scenario crash?
2. Is it mathematically possible to achieve the objectives within the constraints? (e.g., if the time limit is 5 turns, but the minimum movement distance to the extraction point is 6 turns, the test fails and flags the designer).
3. Does it yield a valid `FWhatIfSimulationResult`?

This ensures that the massive combinatorial explosion of the "What If" graph never results in a structurally broken game state.
