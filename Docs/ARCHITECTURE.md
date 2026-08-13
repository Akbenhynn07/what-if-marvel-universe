# W.H.A.T. I.F. System Architecture

## Full System Architecture Overview
The W.H.A.T. I.F. engine is designed as a hybrid deterministic simulation and AI narrative generation platform. To achieve this, the architecture strictly separates the hard rules of the game world (attributes, abilities, combat resolution) from the subjective interpretation of those rules (dialogue, scene description, emotional state). This separation guarantees that the simulation remains stable and reproducible while allowing infinite creative variations in how the results are presented to the user.

## Module Dependency Diagram

```text
+-----------------------------------------------------------------------------------+
|                                 UI Layer                                          |
|  [Character UI]   [Timeline Viewer]   [Narrative Display]   [Combat HUD]          |
+-----------------------------------------------------------------------------------+
                                      |
+-----------------------------------------------------------------------------------+
|                                Game Layer                                         |
|  [World Manager]   [Save/Load System]   [Audio System]   [Mod Support]            |
+-----------------------------------------------------------------------------------+
                                      |
+-----------------------------------------------------------------------------------+
|                            Narrative Layer                                        |
|             [Narrative Generator] <-------> [Dialogue System]                     |
+-----------------------------------------------------------------------------------+
                                      |
+-----------------------------------------------------------------------------------+
|                              AI Layer                                             |
|        [AI Agent Decision] <-----------> [LLM Context Manager]                    |
+-----------------------------------------------------------------------------------+
                                      |
+-----------------------------------------------------------------------------------+
|                          Simulation Layer                                         |
|  [Timeline Manager] <-> [Scenario Manager] <-> [Combat Resolver]                  |
+-----------------------------------------------------------------------------------+
                                      |
+-----------------------------------------------------------------------------------+
|                               Data Layer                                          |
|  [Character Sys]  [Attribute Sys]  [Ability Sys]  [Equipment Sys]  [Trans. Sys]   |
+-----------------------------------------------------------------------------------+
```

## Layered Architecture Description

1. **UI Layer**: The highest level, responsible for all user interaction, built primarily using Common UI. It never modifies simulation data directly.
2. **Game Layer**: Handles traditional game loop responsibilities, persistence (Save/Load), audio cues triggered by the narrative or simulation, and mod loading.
3. **Narrative Layer**: Bridges the cold data of the simulation with human-readable text. It consumes simulation logs and state data to prompt the AI for contextual story beats.
4. **AI Layer**: Manages asynchronous requests to local or cloud LLMs. It enforces context window limits, sanitizes outputs, and structures prompts.
5. **Simulation Layer**: The deterministic core where the "game" happens. It advances time, resolves actions between entities, and branches timelines.
6. **Data Layer**: The foundational structures defining entities. Strictly leverages UE5's Gameplay Ability System (GAS) for numeric precision and modularity.

## System Responsibilities

- **Character System**: Manages base entity identities, relationships, and intrinsic tags.
- **Attribute System**: Handles numeric values (Health, Power, Speed) using GAS `UAttributeSet`.
- **Ability System**: Defines what entities can do (Attack, Defend, Transform) via `UGameplayAbility`.
- **Equipment System**: Manages items that grant abilities or modify attributes.
- **Transformation System**: The core mechanic—applies massive, permanent shifts to a character's Data Layer (e.g., swapping Attribute Sets, granting new Abilities).
- **Combat System**: Resolves adversarial scenarios deterministically using dice rolls or math models.
- **AI Agent Decision System**: Determines what action a character will take based on their personality and current state.
- **World System**: Tracks global variables, locations, and environment states.
- **Scenario Framework**: Packages specific starting conditions and goals into playable modules.
- **Timeline System**: Manages branching realities. Every major transformation or critical decision creates a new node in a persistent graph.
- **Narrative System**: Takes Timeline nodes and combat logs to generate descriptive text.
- **Dialogue System**: Generates and manages conversational exchanges between characters.
- **Save/Load System**: Serializes the Timeline graph and World state to disk.
- **UI System**: Renders the game state to the player using modular widgets.
- **Audio System**: Plays dynamic soundscapes based on Narrative Layer sentiment analysis.
- **Mod System**: Discovers and loads Game Feature Plugins containing new Scenarios, Characters, or Transformations.

## Inter-System Communication Patterns
- **UInterface Contracts**: Systems that need to decouple implementation details communicate via UInterfaces (e.g., `IInteractable`, `ITransformable`).
- **Delegates and Events**: The primary method for cross-layer communication. The Simulation Layer broadcasts `FOnStateChanged` delegates, which the UI and Narrative layers bind to.
- **Asynchronous Tasks**: AI and Narrative layers utilize `UBlueprintAsyncActionBase` for non-blocking HTTP requests to LLM APIs.

## UE5-Specific Patterns Used
- **Gameplay Ability System (GAS)**: Used exclusively for all Character, Attribute, Ability, Equipment, and Transformation logic. Ensures network-ready (future-proofing) and highly robust stat management.
- **UPrimaryDataAsset**: Used for defining static configuration data (e.g., Base Character Stats, Transformation Definitions). Loaded via the Asset Manager for memory efficiency.
- **Modular Game Framework & Game Feature Plugins**: New content (a new timeline pack, a new character) is built as a self-contained Game Feature Plugin, ensuring the core codebase remains pristine.

## Data Flow for a Complete Simulation Run
1. **Initialization**: The World System loads a Scenario (Primary Data Asset).
2. **Instantiation**: The Character System spawns entities and initializes their GAS components.
3. **Simulation Tick**: 
   - The Timeline System creates a new Node.
   - The AI Agent Decision System selects actions for NPCs based on current state.
   - The Combat System/Scenario Framework resolves these actions deterministically.
   - The Transformation System is invoked if a trigger condition is met, altering character GAS data.
4. **Narrative Generation**: The Simulation Layer broadcasts the results of the tick. The Narrative Layer packages this data into a prompt and sends it to the AI Layer.
5. **Presentation**: The AI Layer returns prose/dialogue. The UI Layer displays this text alongside updated stat bars.

## Deterministic Boundary (What the LLM CAN and CANNOT control)
- **CANNOT**: The LLM cannot change a character's stats, kill a character, invent new abilities, or alter the outcome of a simulation step. The simulation is strictly deterministic.
- **CAN**: The LLM controls tone, dialogue, flavor text, internal monologues, and descriptive embellishments of the deterministic events that just occurred.

## Module Split
- **WhatIf (Runtime)**: Contains all gameplay logic, simulation systems, AI integrations, and UI. Compiled for the final game client.
- **WhatIfEditor (Editor-only)**: Contains custom tooling, such as a node-based Timeline visualizer, custom Asset Type Actions for Character Data, and LLM prompt testing utilities. Never included in the packaged build.
