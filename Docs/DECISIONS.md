# W.H.A.T. I.F. Architecture Decision Records (ADRs)

This document tracks all significant architectural decisions made during the development of the W.H.A.T. I.F. (What Happens After Transformation: Infinite Futures) engine. By recording these decisions, we preserve the historical context of the project and explain the reasoning behind our technical design choices.

---

## ADR-001: Unreal Engine 5.4 as Game Engine

**Status**: Accepted
**Date**: 2024-05-10

**Context**: We require a robust, highly performant game engine capable of handling complex simulation state, high-fidelity graphics, and deep systemic gameplay for the infinite futures simulation. The engine must support rapid iteration for designers while allowing deep C++ access for engineers.

**Decision**: Adopt Unreal Engine 5.4 as the core technology stack for the entire project.

**Rationale**: UE 5.4 provides mature tools for C++ gameplay programming, advanced rendering features (Nanite, Lumen), and established patterns for handling complex data (Data Assets, GAS). Its visual scripting system (Blueprints) empowers technical designers to iterate quickly without waiting on C++ compiles. Furthermore, the massive community and extensive documentation make onboarding new team members significantly easier.

**Alternatives Considered**: 
- *Unity*: While capable, it lacks the built-in gameplay framework robustness out of the box (like GAS). We would have to write too many core systems from scratch.
- *Custom Engine*: Too much overhead for rendering, cross-platform compilation, and tooling development. It would completely detract from focusing on our unique simulation logic.

**Consequences**: Requires strict adherence to Unreal's C++ macros and memory management paradigms (UObject/Garbage Collection). Teams must be trained on Engine conventions, which can present a steep learning curve for developers coming from standard C++.

---

## ADR-002: Gameplay Ability System (GAS) for Attributes and Abilities

**Status**: Accepted
**Date**: 2024-05-15

**Context**: Characters in our simulation undergo massive transformations that alter their fundamental stats, grant new abilities, and apply complex status effects that persist across different timeline branches. Tracking all of these modifiers manually is error-prone.

**Decision**: Utilize Epic's Gameplay Ability System (GAS) plugin to drive all character attributes, transformations, buffs, debuffs, and active skills.

**Rationale**: GAS provides a battle-tested framework for modifying attributes, predicting client actions across a network, and handling complex ability logic (Gameplay Effects, Gameplay Cues). Building a custom buff/debuff system would mean reinventing the wheel poorly, costing months of engineering time.

**Alternatives Considered**: 
- *Custom Component-based attribute system*: Deemed too risky. It would lack the built-in replication, prediction, and deep gameplay tag integration that GAS offers out of the box.

**Consequences**: There is a notoriously steep learning curve for new developers to understand GAS. It requires a rigid setup of Attribute Sets and Gameplay Tags. Debugging GAS issues requires specific knowledge of its internal execution pipeline.

---

## ADR-003: UPrimaryDataAsset for Character and Modification Data

**Status**: Accepted
**Date**: 2024-05-20

**Context**: The game requires vast amounts of static data to define characters, their potential transformation branches, and the visual/stat changes associated with each timeline divergence.

**Decision**: Store all primary definitional data in classes derived from `UPrimaryDataAsset`.

**Rationale**: Primary Data Assets integrate seamlessly with Unreal's Asset Manager, allowing for highly optimized asynchronous loading and memory management. They are easy to edit in the Editor via detail panels without writing Blueprint logic, making them perfect for designer-facing configuration.

**Alternatives Considered**: 
- *Standard `UDataAsset`*: Lacks the deep Asset Manager integration out of the box, making asynchronous streaming harder.
- *JSON/CSV files*: Harder to natively reference engine assets like meshes and materials directly without writing custom parsing layers.

**Consequences**: Requires careful structuring of Data Asset class hierarchies to avoid bloated base classes. We must maintain a clear inheritance structure for transformation definitions.

---

## ADR-004: TSoftObjectPtr for All Asset References in Data Assets

**Status**: Accepted
**Date**: 2024-05-22

**Context**: Loading a single character shouldn't inadvertently load every material, skeletal mesh, and particle effect for all their potential infinite future transformations. This would bloat memory and catastrophically increase initial load times.

**Decision**: Use `TSoftObjectPtr<T>` and `TSoftClassPtr<T>` exclusively for asset references inside our Data Assets.

**Rationale**: Soft pointers prevent hard dependencies from forcing synchronous loading of heavy assets. The Asset Manager can resolve these soft references asynchronously only when the transformation is actually triggered or pre-cached right before use.

**Alternatives Considered**: 
- *Hard pointers (`UTexture2D*`, `UStaticMesh*`)*: Rejected due to unacceptable memory footprint and cascading load times that would freeze the main thread.

**Consequences**: All gameplay systems must be built to handle asynchronous loading states. UI and logic must elegantly handle cases where an asset is requested but not yet in memory (e.g., showing a loading spinner or delaying a transformation).

---

## ADR-005: Modular Game Framework (AModularCharacter) as Character Base

**Status**: Accepted
**Date**: 2024-06-01

**Context**: Transformations may drastically alter a character, requiring the addition or removal of features, meshes, and logic dynamically at runtime. A monolithic character class cannot support infinite permutations.

**Decision**: Base all character classes on the Unreal Modular Game Framework (e.g., `AModularCharacter`, `UGameFrameworkComponentManager`).

**Rationale**: The modular framework allows for "feature-based" composition using Game Feature Plugins and components that register themselves automatically. We can inject new logic into a character dynamically without the character class knowing about it.

**Alternatives Considered**: 
- *Standard `ACharacter` with hardcoded arrays of modular components*: Less flexible, creates a massive master class, and harder to manage via Game Feature plugins.

**Consequences**: Requires adopting the Receiver/Sender pattern for component initialization. Dependency injection becomes the standard, requiring developers to think in a decoupled manner.

---

## ADR-006: All Major Systems Behind UInterface Contracts

**Status**: Accepted
**Date**: 2024-06-05

**Context**: To ensure decoupling, testability, and the ability to swap implementations (e.g., swapping a local Narrative simulation for a cloud-hosted LLM version), subsystems need strict, compile-time boundaries.

**Decision**: All communication between major engine modules (e.g., Timeline Management, Combat, Narrative) must pass through `UInterface` abstractions.

**Rationale**: Interfaces decouple the caller from the implementation. This makes mocking systems for automation tests trivial and prevents cyclic dependency graphs between Unreal modules, which slows down compilation and creates spaghetti code.

**Alternatives Considered**: 
- *Direct concrete class casting (`Cast<USpecificNarrativeSystem>()`)*: Rejected due to tight coupling and poor modularity.

**Consequences**: Increases the boilerplate code needed to define and implement systems. Finding implementations of interfaces in the IDE can sometimes be slightly more tedious than following a concrete pointer.

---

## ADR-007: BlueprintNativeEvent on All Interface Methods

**Status**: Accepted
**Date**: 2024-06-07

**Context**: Designers and technical artists need the ability to implement specific interface callbacks entirely in Blueprint without C++ intervention.

**Decision**: Annotate all methods in public `UInterface` classes with `UFUNCTION(BlueprintNativeEvent)`.

**Rationale**: This ensures that any interface can be implemented natively in C++ (via `_Implementation` overrides) or handled entirely within a Blueprint graph, offering maximum flexibility across disciplines.

**Alternatives Considered**: 
- *`BlueprintImplementableEvent`*: Cannot have a default C++ fallback implementation.
- *Pure C++ virtuals*: Cannot be implemented in Blueprint at all.

**Consequences**: C++ implementations must append `_Implementation` to the function name, and calls must use the `IInterface::Execute_Function` static wrapper. This adds a slight overhead but is worth the flexibility.

---

## ADR-008: Deterministic Combat with Seeded RNG

**Status**: Accepted
**Date**: 2024-06-15

**Context**: To simulate "Alternate Timelines," the engine must be able to replay a sequence of events and diverge at a specific point with perfectly consistent results up to the point of divergence.

**Decision**: Use an explicitly seeded Random Number Generator (`FRandomStream`) for all combat, transformation, and narrative logic. Never use the global `FMath::Rand()`.

**Rationale**: A seeded `FRandomStream` guarantees that given the same inputs and seed, the simulation will yield the exact same outcomes. This is the bedrock upon which our branching timeline features rely.

**Alternatives Considered**: 
- *Relying on unseeded RNG and saving full state snapshots constantly*: Unfeasible due to extreme memory constraints and the performance cost of serializing complex actors every frame.

**Consequences**: Every system that requires randomness must take an `FRandomStream` context or access a globally managed deterministic seed provider. Code reviews must strictly enforce this rule.

---

## ADR-009: Source Module Split — WhatIf (Runtime) + WhatIfEditor (Editor)

**Status**: Accepted
**Date**: 2024-06-20

**Context**: Custom editor tools (like Timeline graph visualizers, custom detail panels, and asset factories) rely on UnrealEd and Editor-only modules that cannot be shipped in a packaged standalone game.

**Decision**: Split the project into at least two primary C++ modules: `WhatIf` (Runtime) and `WhatIfEditor` (Editor/Uncooked only).

**Rationale**: This split prevents catastrophic packaging failures caused by referencing Editor-only code in runtime builds. It enforces a clean separation of concerns and keeps the shipped executable as small as possible.

**Alternatives Considered**: 
- *A single monolithic module using `#if WITH_EDITOR` macros*: Rejected because it leads to messy, unreadable code and increases the risk of accidentally shipping editor dependencies if a macro is misplaced.

**Consequences**: Development environment configuration is slightly more complex. Cross-module dependencies must be carefully managed in the respective `.Build.cs` files to avoid linkage errors.

---

## ADR-010: Timeline as Directed Acyclic State Graph

**Status**: Accepted
**Date**: 2024-07-01

**Context**: Tracking the infinite futures requires a data structure that can handle diverging histories, player choices, and transformation states without circular paradoxes.

**Decision**: Model the timeline simulation as a Directed Acyclic Graph (DAG) of state nodes.

**Rationale**: A DAG naturally models temporal progression where branches diverge based on decisions or RNG but do not loop back into the past. It allows efficient graph traversal for querying "what if" scenarios based on any historical node in the network.

**Alternatives Considered**: 
- *Linear arrays with snapshot indexing*: Fails to elegantly capture complex branching and merging.
- *Generic graphs (allowing cycles)*: Overly complex and semantically incorrect for the concept of time.

**Consequences**: Requires writing custom graph traversal, visualization, and serialization logic tailored for the DAG structure. We must ensure memory leaks do not occur when pruning dead branches.

---

## ADR-011: LLM Behind INarrativeProvider Interface Abstraction

**Status**: Accepted
**Date**: 2024-07-15

**Context**: We intend to integrate Large Language Models (LLMs) to dynamically generate narrative flavor text for transformations and timeline outcomes based on simulation state.

**Decision**: Isolate all LLM integration behind an `INarrativeProvider` interface, avoiding any direct hardcoding of specific vendor APIs (e.g., OpenAI, Anthropic, local LLaMA) in gameplay code.

**Rationale**: The AI landscape changes rapidly. By querying narrative data through `INarrativeProvider->RequestFlavorText(Context, Callback)`, we can seamlessly swap between local, remote, or mock providers without touching core simulation logic. It also allows us to write offline tests.

**Alternatives Considered**: 
- *Direct API calls via HTTP inside GameModes*: Highly brittle, couples gameplay code to external web services, and makes offline testing impossible.

**Consequences**: Requires defining robust, generic context data structures to pass to the provider so it has enough information to generate text, regardless of the backend implementation.

---

## ADR-012: DataTable + FTableRowBase for Tabular Stat/Progression Data

**Status**: Accepted
**Date**: 2024-07-25

**Context**: We need to define numeric progression curves, base stats per class, and experience requirements that designers can edit in bulk via Excel/CSV spreadsheets.

**Decision**: Define C++ structs inheriting from `FTableRowBase` and use Engine `UDataTable` assets for bulk numerical data storage and retrieval.

**Rationale**: DataTables allow seamless import/export from CSV/JSON formats, providing a highly familiar and efficient workflow for system designers balancing the game outside of the Unreal Editor.

**Alternatives Considered**: 
- *Curve tables*: Good for continuous data, but bad for discrete structural data (like experience per level).
- *Primary Data Assets*: Too much clicking around in the editor for bulk numerical balancing; better suited for distinct complex assets with asset references.

**Consequences**: Developers must ensure C++ structs accurately reflect the exact column layout of external spreadsheets to avoid import errors and mismatched data at runtime.
