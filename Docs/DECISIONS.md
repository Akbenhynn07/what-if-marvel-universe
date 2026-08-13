# W.H.A.T. I.F. Architecture Decision Records (ADRs)

This document tracks all significant architectural decisions made during the development of the W.H.A.T. I.F. (What Happens After Transformation: Infinite Futures) engine.

---

## ADR-001: Unreal Engine 5.4 as Game Engine

**Status**: Accepted
**Date**: 2024-05-10
**Context**: We require a robust, performant engine capable of handling complex simulation state, high-fidelity graphics, and deep systemic gameplay for the infinite futures simulation.
**Decision**: Adopt Unreal Engine 5.4 as the core technology stack.
**Rationale**: UE 5.4 provides mature tools for C++ gameplay programming, advanced rendering features (Nanite, Lumen), and established patterns for handling complex data (Data Assets, GAS). Its visual scripting (Blueprints) empowers designers to iterate quickly.
**Alternatives Considered**: 
- *Unity*: Lacks the built-in gameplay framework robustness out of the box (like GAS).
- *Custom Engine*: Too much overhead for rendering and tooling development; detracts from focusing on simulation logic.
**Consequences**: Requires strict adherence to Unreal's C++ macros and memory management (UObject/Garbage Collection). Teams must be trained on Engine conventions.

---

## ADR-002: Gameplay Ability System (GAS) for Attributes and Abilities

**Status**: Accepted
**Date**: 2024-05-15
**Context**: Characters undergo massive transformations that alter their stats, grant new abilities, and apply complex status effects across different timelines.
**Decision**: Utilize Epic's Gameplay Ability System (GAS) plugin to drive all character attributes, transformations, and active skills.
**Rationale**: GAS provides a battle-tested framework for modifying attributes, predicting client actions, and handling complex ability logic (Gameplay Effects, Gameplay Cues). Building a custom buff/debuff system would reinvent the wheel poorly.
**Alternatives Considered**: Custom Component-based attribute system. Deemed too risky and lacking built-in replication and prediction features.
**Consequences**: Steep learning curve for new developers. Requires rigid setup of Attribute Sets and Gameplay Tags.

---

## ADR-003: UPrimaryDataAsset for Character and Modification Data

**Status**: Accepted
**Date**: 2024-05-20
**Context**: The game requires vast amounts of static data to define characters, their potential transformation branches, and the visual/stat changes associated with each.
**Decision**: Store all primary definitional data in classes derived from `UPrimaryDataAsset`.
**Rationale**: Primary Data Assets integrate seamlessly with the Asset Manager, allowing for asynchronous loading and memory management. They are easy to edit in the Editor without writing Blueprint logic.
**Alternatives Considered**: Standard `UDataAsset` (lacks Asset Manager integration out of the box). JSON/CSV files (harder to reference engine assets like meshes and materials directly).
**Consequences**: Requires careful structuring of Data Asset hierarchies to avoid bloated base classes.

---

## ADR-004: TSoftObjectPtr for All Asset References in Data Assets

**Status**: Accepted
**Date**: 2024-05-22
**Context**: Loading a single character shouldn't inadvertently load every material, mesh, and particle effect for all their potential infinite future transformations, which would bloat memory and increase load times.
**Decision**: Use `TSoftObjectPtr<T>` and `TSoftClassPtr<T>` exclusively for asset references inside Data Assets.
**Rationale**: Soft pointers prevent hard dependencies from forcing synchronous loading of heavy assets. The Asset Manager can resolve these soft references asynchronously only when the transformation is actually triggered.
**Alternatives Considered**: Hard pointers (`UTexture2D*`). Rejected due to unacceptable memory footprint and cascading load times.
**Consequences**: Systems must be built to handle asynchronous loading states and handle cases where an asset is requested but not yet in memory.

---

## ADR-005: Modular Game Framework (AModularCharacter) as Character Base

**Status**: Accepted
**Date**: 2024-06-01
**Context**: Transformations may drastically alter a character, requiring the addition or removal of features, meshes, and logic dynamically at runtime.
**Decision**: Base all character classes on the Modular Game Framework (e.g., `AModularCharacter`, `UGameFrameworkComponentManager`).
**Rationale**: The modular framework allows for "feature-based" composition using Game Feature Plugins and components that register themselves automatically, rather than creating massive monolithic character classes.
**Alternatives Considered**: Standard `ACharacter` with hardcoded arrays of modular components. Less flexible and harder to manage via Game Feature plugins.
**Consequences**: Requires adopting the Receiver/Sender pattern for component initialization and dependency injection.

---

## ADR-006: All Major Systems Behind UInterface Contracts

**Status**: Accepted
**Date**: 2024-06-05
**Context**: To ensure decoupling, testability, and the ability to swap implementations (e.g., swapping a local Narrative simulation for a cloud-hosted one), subsystems need strict boundaries.
**Decision**: All communication between major engine modules must pass through `UInterface` abstractions.
**Rationale**: Interfaces decouple the caller from the implementation. This makes mocking systems for tests trivial and prevents cyclic dependencies between modules.
**Alternatives Considered**: Direct concrete class casting (`Cast<USpecificNarrativeSystem>()`). Rejected due to tight coupling and poor modularity.
**Consequences**: Increases the boilerplate needed to define and implement systems.

---

## ADR-007: BlueprintNativeEvent on All Interface Methods

**Status**: Accepted
**Date**: 2024-06-07
**Context**: Designers and technical artists need the ability to implement specific interface callbacks entirely in Blueprint without C++ intervention.
**Decision**: Annotate all methods in public `UInterface` classes with `UFUNCTION(BlueprintNativeEvent)`.
**Rationale**: This ensures that any interface can be implemented natively in C++ (via `_Implementation` overrides) or handled entirely within a Blueprint graph, offering maximum flexibility.
**Alternatives Considered**: `BlueprintImplementableEvent` (cannot have a C++ fallback). Pure C++ virtuals (cannot be implemented in Blueprint).
**Consequences**: C++ implementations must append `_Implementation` to the function name, and calls must use the `IInterface::Execute_Function` static wrapper.

---

## ADR-008: Deterministic Combat with Seeded RNG

**Status**: Accepted
**Date**: 2024-06-15
**Context**: To simulate "Alternate Timelines," the engine must be able to replay a sequence of events and diverge at a specific point with consistent results up to the divergence point.
**Decision**: Use an explicitly seeded Random Number Generator (`FRandomStream`) for all combat, transformation, and narrative logic. Never use `FMath::Rand()`.
**Rationale**: A seeded `FRandomStream` guarantees that given the same inputs and seed, the simulation will yield the exact same outcomes. This is critical for timeline branching features.
**Alternatives Considered**: Relying on unseeded RNG and saving full state snapshots constantly. Unfeasible due to memory and performance constraints.
**Consequences**: Every system that requires randomness must take an `FRandomStream` context or access a globally managed deterministic seed provider.

---

## ADR-009: Source Module Split — WhatIf (Runtime) + WhatIfEditor (Editor)

**Status**: Accepted
**Date**: 2024-06-20
**Context**: Custom editor tools (like Timeline graph visualizers and custom detail panels) rely on UnrealEd and Editor-only modules that cannot be shipped in a packaged game.
**Decision**: Split the project into at least two primary C++ modules: `WhatIf` (Runtime) and `WhatIfEditor` (Editor/Uncooked only).
**Rationale**: Prevents packaging failures caused by referencing Editor-only code in runtime builds. Enforces a clean separation of concerns.
**Alternatives Considered**: A single monolithic module using `#if WITH_EDITOR`. Rejected because it leads to messy code and increases the risk of accidentally shipping editor dependencies.
**Consequences**: Development environment configuration is slightly more complex, and cross-module dependencies must be managed in the `.Build.cs` files.

---

## ADR-010: Timeline as Directed Acyclic State Graph

**Status**: Accepted
**Date**: 2024-07-01
**Context**: Tracking the infinite futures requires a data structure that can handle diverging histories, choices, and transformation states without circular paradoxes.
**Decision**: Model the timeline simulation as a Directed Acyclic Graph (DAG) of state nodes.
**Rationale**: A DAG naturally models temporal progression where branches diverge but do not loop back into the past. It allows efficient traversal for querying "what if" scenarios based on historical nodes.
**Alternatives Considered**: Linear arrays with snapshot indexing. Fails to elegantly capture complex branching. Generic graphs (allowing cycles). Overly complex and semantically incorrect for time.
**Consequences**: Requires writing custom graph traversal and serialization logic tailored for the DAG structure.

---

## ADR-011: LLM Behind INarrativeProvider Interface Abstraction

**Status**: Accepted
**Date**: 2024-07-15
**Context**: We intend to integrate Large Language Models (LLMs) to dynamically generate narrative flavor text for transformations and timeline outcomes.
**Decision**: Isolate all LLM integration behind an `INarrativeProvider` interface, avoiding any direct hardcoding of specific vendor APIs (e.g., OpenAI, Anthropic) in gameplay code.
**Rationale**: The AI landscape changes rapidly. By querying narrative data through `INarrativeProvider->RequestFlavorText(Context, Callback)`, we can seamlessly swap between local, remote, or mock providers without touching core simulation logic.
**Alternatives Considered**: Direct API calls via HTTP inside GameModes. Highly brittle and couples gameplay code to external web services.
**Consequences**: Requires defining robust, generic context data structures to pass to the provider so it has enough information to generate text, regardless of the backend.

---

## ADR-012: DataTable + FTableRowBase for Tabular Stat/Progression Data

**Status**: Accepted
**Date**: 2024-07-25
**Context**: We need to define numeric progression curves, base stats per class, and experience requirements that designers can edit in bulk via Excel/CSV.
**Decision**: Define C++ structs inheriting from `FTableRowBase` and use Engine `UDataTable` assets for bulk numerical data.
**Rationale**: DataTables allow seamless import/export from CSV/JSON, providing a familiar workflow for system designers balancing the game.
**Alternatives Considered**: Curve tables (good for continuous data, bad for discrete structural data). Primary Data Assets (too much clicking around for bulk numerical balancing; better for distinct complex assets).
**Consequences**: Developers must ensure structs accurately reflect the column layout of external spreadsheets to avoid import errors.
