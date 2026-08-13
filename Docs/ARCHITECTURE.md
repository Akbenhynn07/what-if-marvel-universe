# W.H.A.T. I.F. System Architecture

## Overview

W.H.A.T. I.F. is architected as a **layered, deterministic simulation platform** with a strict separation between the rule-governed game world and the language-model-powered narrative layer. The central design constraint is: **the LLM translates reality — it does not define it.**

Every system in the engine communicates through defined `UInterface` contracts, making each layer independently testable, replaceable, and extensible without breaking the systems that depend on it.

---

## Layered Architecture

```
╔═══════════════════════════════════════════════════════════════════════╗
║                            UI LAYER                                   ║
║   Character Lab │ Transformation Editor │ Timeline Explorer │ HUD     ║
║   [MVVM Plugin] [Common UI] [UMG Widgets] [Slate Editor Tools]        ║
╠═══════════════════════════════════════════════════════════════════════╣
║                          GAME LAYER                                   ║
║   World Manager │ Save/Load System │ Audio System │ Mod/GFP Loader    ║
║   [UGameInstance] [USaveGame] [UGameFeatureSubsystem]                 ║
╠═══════════════════════════════════════════════════════════════════════╣
║                        NARRATIVE LAYER                                ║
║   INarrativeProvider ←→ Async HTTP Client ←→ LLM API (abstracted)    ║
║   [UBlueprintAsyncActionBase] [FHttpModule] [Output Validator]        ║
╠═══════════════════════════════════════════════════════════════════════╣
║                          AI LAYER                                     ║
║   IDecisionEngine │ FWhatIfAgentState │ Utility Scorer                ║
║   [Perception] [Goal Evaluation] [Action Selection] [Threat Model]    ║
╠═══════════════════════════════════════════════════════════════════════╣
║                       SIMULATION LAYER                                ║
║   ICombatSimulator │ ITransformationSystem │ Timeline Graph           ║
║   [Seeded RNG] [State Machine] [UWhatIfTimelineGraph]                 ║
╠═══════════════════════════════════════════════════════════════════════╣
║                          DATA LAYER                                   ║
║   UWhatIfCharacterData │ UWhatIfModificationData │ UWhatIfScenarioData║
║   UWhatIfAbilityData   │ UWhatIfEquipmentData    │ UWhatIfEnvironment  ║
║   [UPrimaryDataAsset] [GAS UAttributeSet] [AssetManager]             ║
╚═══════════════════════════════════════════════════════════════════════╝
```

Each layer may only call **downward** (or horizontally within a layer). Higher layers never bypass lower layers to access raw state directly.

---

## System Responsibilities

### Data Layer
| System | Class | Responsibility |
|--------|-------|---------------|
| Character System | `UWhatIfCharacterData` | Defines a character's identity, base attributes, abilities, equipment, personality, relationships, and constraints. Loaded async via AssetManager. |
| Attribute System | `UWhatIfAttributeSet` | 14 GAS gameplay attributes (Strength, Speed, Durability, Energy, etc.). All attribute changes go through `UGameplayEffect` — never set directly. |
| Ability System | `UWhatIfAbilityData` | Defines discrete character actions. Backed by `UGameplayAbility`. Abilities have costs, cooldowns, and range. |
| Equipment System | `UWhatIfEquipmentData` | Items occupying a `EWhatIfModificationSlot`. Apply `UGameplayEffect` as passive attribute modifiers. |
| Transformation System (Data) | `UWhatIfModificationData` | User-installable modification components. Carry advantages, disadvantages, energy costs, side effects, and `UGameplayEffect` for stat changes. |
| Scenario System | `UWhatIfScenarioData` | Self-contained simulation context. References characters by ID, defines objectives, constraints, events, and possible outcomes. |
| Environment System | `UWhatIfEnvironmentData` | World context. Modifies attributes (e.g., reduced flight speed underwater), disables abilities, sets gravity and visibility. |

### Simulation Layer
| System | Interface | Responsibility |
|--------|-----------|---------------|
| Combat Simulator | `IWhatIfCombatSimulator` | Deterministic combat resolution. `RunCombat(Setup, Seed)` → reproducible `FWhatIfCombatResult`. |
| Transformation System (Logic) | `IWhatIfTransformationSystem` | Applies/removes `UWhatIfModificationData` to a live character instance. Validates compatibility, applies GAS effects. |
| Timeline System | `UWhatIfTimelineGraph` | Directed acyclic graph of `FWhatIfTimelineNode`. Each branch preserves a world state snapshot, character states, events, and the seed that produced them. |

### AI Layer
| System | Interface | Responsibility |
|--------|-----------|---------------|
| Decision Engine | `IWhatIfDecisionEngine` | Given `FWhatIfAgentState` + available actions → selects an action using utility scoring weighted by personality traits. |
| Agent State | `FWhatIfAgentState` | Live runtime state of an agent: current attributes, goals, perception (threats/allies), resources, stance, personality. |

### Narrative Layer
| System | Interface | Responsibility |
|--------|-----------|---------------|
| Narrative Provider | `IWhatIfNarrativeProvider` | Takes structured `FWhatIfSimulationResult` → returns prose via async LLM call. Provider is abstracted: OpenAI, Gemini, Claude, or local. |

---

## Module Architecture

### `WhatIf` (Runtime Module)
- **Type**: Runtime  
- **Dependencies**: Core, CoreUObject, Engine, GameplayAbilities, GameplayTags, GameplayTasks, ModularGameplay, GameFeatures, StructUtils, Enhanced Input  
- **Contains**: All simulation logic, data models, interfaces, AI systems, timeline engine, and narrative abstraction  
- **Never includes**: Editor-only APIs, SlateCore (UI is in separate widget module long-term)

### `WhatIfEditor` (Editor Module)
- **Type**: Editor, PostEngineInit  
- **Dependencies**: Core, Engine, WhatIf, UnrealEd, AssetTools, PropertyEditor  
- **Contains**: Custom Asset Type Actions, node-graph Timeline visualizer, LLM prompt testing utilities, data validation tools  
- **Never shipped**: Stripped from packaged builds automatically by UBT

---

## Key Interface Contracts

All cross-system communication uses `UInterface` with `BlueprintNativeEvent` — C++ provides default implementations, Blueprints can override without recompilation.

```
IWhatIfSimulationSystem   ← Base for all simulation subsystems
IWhatIfTransformationSystem ← Apply/remove/preview modifications
IWhatIfCombatSimulator    ← Deterministic combat engine
IWhatIfDecisionEngine     ← Agent action selection
IWhatIfNarrativeProvider  ← LLM backend abstraction
```

**Calling convention rule**: Always use `Execute_FunctionName(Target, Args...)` — never raw-cast to the I-class. This ensures Blueprint-implemented interfaces work correctly.

---

## Data Flow: Complete Simulation Run

```
1. Player configures transformation
        │ WhatIfModificationData loaded (async, AssetManager)
        │ IWhatIfTransformationSystem.IsModificationCompatible() checked
        │ UGameplayEffect applied to AttributeSet
        ↓
2. Player selects scenario
        │ UWhatIfScenarioData loaded (async)
        │ Characters referenced by FPrimaryAssetId, loaded async
        │ World state initialized from scenario
        ↓
3. Simulation tick begins
        │ UWhatIfTimelineGraph creates new FWhatIfTimelineNode (new branch)
        │ FWhatIfAgentState built from each character's GAS AttributeSet
        │ IWhatIfDecisionEngine.SelectAction() called per agent
        │ IWhatIfCombatSimulator.RunCombat(Setup, Seed) executes deterministically
        │ GAS GameplayEffects applied from combat outcomes
        │ Timeline node updated with world state snapshot + outcome flags
        ↓
4. Narrative request
        │ FWhatIfSimulationResult constructed from combat log + state delta
        │ IWhatIfNarrativeProvider.GenerateNarrative() called async
        │ Async HTTP → LLM API
        │ Output validated (cannot contradict simulation facts)
        ↓
5. Presentation
        │ UI Layer receives narrative text via delegate
        │ Timeline Explorer updated with new node
        │ Player can replay, branch, or continue
```

---

## Deterministic Boundary

This boundary is non-negotiable. Crossing it in either direction is an architectural violation.

### The LLM CANNOT control or modify:
- Attribute values (Strength, Speed, Durability, Energy, etc.)
- Combat outcomes (damage dealt, abilities used, winner/loser)
- Health or resource values
- Physics, collision, hit detection
- Character death or survival
- Timeline branch structure
- Inventory or equipment state
- Economy or resource consumption

### The LLM CAN produce:
- Narrative summaries of simulation outcomes
- Dialogue lines for characters (reflecting their personality and current state)
- Flavor descriptions of attacks, transformations, and events
- Internal monologue / emotional reaction text
- Timeline branch labels and descriptions
- Scene-setting prose (environment, atmosphere)

---

## UE5 Design Patterns Used

| Pattern | Where | Why |
|---------|-------|-----|
| `UPrimaryDataAsset` | All content definitions | AssetManager integration, granular async loading, `FPrimaryAssetId` for cross-reference |
| `TSoftObjectPtr` | All asset references inside data assets | Prevents loading entire game on startup; enables runtime unload |
| GAS `UAttributeSet` | All numeric character stats | Networked, modifier-stacking, change-delegate system out of the box |
| GAS `UGameplayEffect` | All stat modifications | Single legal path for attribute changes; supports stacking, duration, instant |
| `AModularCharacter` | Character base class | GameFrameworkComponentManager injects components at runtime |
| Game Feature Plugins | Content packs, transformation packs | Ship new characters/scenarios without recompiling core |
| `UInterface` + `BlueprintNativeEvent` | All cross-system boundaries | C++ defaults, Blueprint overrideable, safe `Execute_` calling |
| `FInstancedStruct` | Polymorphic data without inheritance | Heterogeneous collections (e.g., mixed event types in a scenario) |

---

## Future Architecture Considerations

| Concern | Current Approach | Future Option |
|---------|-----------------|---------------|
| Simulation isolation | In-process C++ | Extract to separate process (gRPC/REST) for independent testing |
| Multiplayer | Not implemented | GAS already network-ready; `AModularCharacter` supports replication |
| AI planning horizon | Single-step utility AI | Multi-step GOAP or hierarchical planning in Phase 13 |
| Timeline storage | In-memory `UWhatIfTimelineGraph` | Serialize to DB/JSON for community sharing in Phase 14 |
