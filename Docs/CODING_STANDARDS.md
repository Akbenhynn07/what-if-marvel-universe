# W.H.A.T. I.F. Coding Standards

Welcome to the W.H.A.T. I.F. (What Happens After Transformation: Infinite Futures) coding standards document. This guide establishes a baseline for C++ and Blueprint development on this project. By adhering to these standards, we ensure code consistency, readability, and maintainability across our engine modules. Our primary goal is to ensure that the code is robust enough for complex simulation logic while remaining readable for new contributors.

## Table of Contents
1. [Introduction](#introduction)
2. [Naming Conventions](#naming-conventions)
3. [Formatting and Style](#formatting-and-style)
4. [Public/Private Directory Split Rules](#publicprivate-directory-split-rules)
5. [Include Rules and Module Management](#include-rules-and-module-management)
6. [UInterface Rules](#uinterface-rules)
7. [Blueprint Guidelines](#blueprint-guidelines)
8. [GAS (Gameplay Ability System) Coding Patterns](#gas-coding-patterns)
9. [Memory Management and Smart Pointers](#memory-management-and-smart-pointers)
10. [Error Handling and Logging](#error-handling-and-logging)
11. [Code Review Checklist](#code-review-checklist)
12. [Documentation Requirements](#documentation-requirements)
13. [Testing Requirements](#testing-requirements)

---

## 1. Introduction

W.H.A.T. I.F. relies heavily on Unreal Engine 5.4's most advanced features. Because the project bridges the gap between deep C++ systems and designer-facing Blueprints, consistency is crucial. If the codebase becomes fragmented in style, debugging timeline divergences and complex character transformations will become impossible.

---

## 2. Naming Conventions

We strictly follow Epic Games' Coding Standard, with specific additions for our modular simulation architecture.

### Classes and Structs
- **Prefixes**: 
  - `U` for classes inheriting from `UObject` (e.g., `UTransformationDataAsset`).
  - `A` for classes inheriting from `AActor` (e.g., `ANarrativeController`).
  - `F` for structs and non-UObject classes (e.g., `FSimulationState`).
  - `E` for enums (e.g., `ETransformationType`).
  - `I` for interface classes (e.g., `IStateMachineInterface`).
  - `T` for templates (e.g., `TStateGraph`).
  - `S` for Slate widgets (e.g., `SGraphNodeTimeline`).
- **PascalCase**: All types must be in PascalCase.

### Variables
- **PascalCase**: All variables should use PascalCase (e.g., `CurrentHealth`, `TransformationState`).
- **Booleans**: Prefix booleans with `b` (e.g., `bIsTransformed`, `bCanTransform`). This makes boolean checks immediately recognizable.
- **Pointers**: No special prefix is required, but clarity is key. Prefer `CharacterOwner` over `pCharacterOwner` or `MyCharacterPtr`.

### Functions
- **PascalCase**: Use PascalCase (e.g., `CalculateTransformationCost`).
- **Actions**: Function names should typically begin with a verb (e.g., `Get...`, `Set...`, `Apply...`, `Notify...`).
- **Delegates/Events**: Prefix event handlers with `On` (e.g., `OnTransformationComplete`).
- **Blueprint Implementations**: Functions intended to be overridden in Blueprint natively should suffix their C++ implementation with `_Implementation`.

---

## 3. Formatting and Style

### Braces
- Always use newline braces (Allman style).
```cpp
if (bIsTransformed)
{
    ApplyModifiers();
}
```

### Indentation
- Use tabs for indentation, not spaces. Set tab size to 4 characters.

### Switch Statements
- Always include a `default` case.
- Explicitly comment fallthroughs to prevent confusion.
```cpp
switch (TimelineState)
{
    case ETimelineState::Active:
        // Do active logic
        break;
    case ETimelineState::Paused:
        // Do paused logic
        break;
    default:
        UE_LOG(LogWhatIf, Warning, TEXT("Unhandled state"));
        break;
}
```

---

## 4. Public/Private Directory Split Rules

Unreal Engine modules support a strict `Public` and `Private` directory separation. We utilize this to enforce encapsulation and compile-time boundaries.

### Public Directory (`Source/Module/Public/`)
- Place header files (`.h`) here *only* if they need to be accessed by other modules or external systems.
- Interfaces (`I...`), pure abstract base classes, and core data types (`UPrimaryDataAsset` structures) typically belong here.
- Avoid placing implementation details or internal helper class headers in the Public directory. By minimizing public headers, we significantly speed up compile times.

### Private Directory (`Source/Module/Private/`)
- Place all source files (`.cpp`) here.
- Place header files (`.h`) here if they are only used internally within the module.
- Concrete implementations of interfaces should generally keep their headers in the Private folder if external modules only need the interface pointer.

---

## 5. Include Rules and Module Management

Proper include management minimizes compile times and reduces cyclical dependencies.

- **Include What You Use (IWYU)**: Every header must include the exact headers it requires to compile. Do not rely on transitive includes. If you use `FVector`, include `Math/Vector.h`.
- **Forward Declarations**: Always prefer forward declarations in header files over `#include` directives when possible (e.g., `class UAnimInstance;`). This prevents cascading recompiles when a header changes.
- **Implementation Includes**: In `.cpp` files, `#include` the corresponding header file first, followed by core engine headers, then module headers, and finally project-specific headers.
- **Precompiled Headers (PCH)**: PCHs are handled by Unreal Build Tool (UBT). Do not manually include a PCH file in your source files.

---

## 6. UInterface Rules

Interfaces are a critical pillar of W.H.A.T. I.F.'s modular architecture.

- **Definition**: Define interfaces using the standard `UInterface` boilerplate.
- **UFUNCTION Annotations**: Ensure that interface functions meant to be implemented or called in Blueprints are marked with `UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="W.H.A.T. I.F.|Interfaces")`.
- **Pure Virtuals**: Avoid pure virtual C++ functions (`virtual void DoThing() = 0;`) in UInterfaces if they need Blueprint support. Rely on the `BlueprintNativeEvent` generation.
- **Implementation Checking**: Use `Implements<UMyInterface>()` or `Cast<IMyInterface>()` to check for interface compliance before executing calls. 
- **Execute Pattern**: When calling an interface function, use the generated `IYourInterface::Execute_FunctionName(ObjectPtr, Args...)` pattern to ensure Blueprint implementations are correctly invoked.
- **Data Passing**: Pass structs by const reference (`const FMyStruct&`) in interfaces to avoid unnecessary copies.

---

## 7. Blueprint Guidelines

While W.H.A.T. I.F. is primarily C++ driven, Blueprints are essential for data definition and designer iteration.

- **Base Classes**: All significant Blueprints (Characters, Abilities, GameModes) must inherit from a custom C++ base class. Avoid deriving directly from engine classes like `AActor` without a project-specific intermediate layer.
- **Logic placement**: 
  - **C++**: Complex state machines, heavy math, network replication logic, and core systems.
  - **Blueprint**: Event hooking, visual effects, audio triggers, UI binding, and iteration-heavy designer data.
- **No Tick**: Avoid using `Event Tick` in Blueprints. Prefer Timers, Timelines, or event-driven logic. If per-frame logic is required, implement it in C++.
- **Variable Categorization**: Always assign Blueprint variables to clear categories (e.g., `Config`, `State`, `References`).
- **Casting**: Avoid hard casting in Blueprints if it loads heavy assets into memory. Use interfaces instead to communicate between disparate actor types.

---

## 8. GAS Coding Patterns

The Gameplay Ability System (GAS) manages character attributes, statuses, and transformations.

- **Attribute Sets**: Keep `UAttributeSet` classes small and focused (e.g., `UHealthAttributeSet`, `UManaAttributeSet`). Use the standard GAS macros for getters, setters, and initializers. Do not put unrelated attributes in the same set.
- **Gameplay Effects (GE)**: Use Data-Only Blueprints for Gameplay Effects. Never implement logic in a C++ Gameplay Effect class. Name them predictably (e.g., `GE_Damage_Fire`, `GE_Status_Poisoned`).
- **Gameplay Abilities (GA)**: 
  - Base mechanics in C++ (`UWhatIfGameplayAbility`).
  - Specific ability logic in Blueprint derivatives.
  - Always call `CommitAbility` and `EndAbility` appropriately. Failure to do so will lock the ability component.
- **Tags**: Use Gameplay Tags extensively for state management instead of boolean flags. Define tags in `DefaultGameplayTags.ini` or C++ native tag registries, never hardcode tag strings like `FName("State.Stunned")`.

---

## 9. Memory Management and Smart Pointers

Unreal provides its own memory management which we must strictly follow.

- **UObjects**: Always use `UPROPERTY()` macros to hold references to `UObject` derived classes to prevent them from being garbage collected.
- **Raw Pointers**: Raw pointers (`FVector*`) are acceptable for short-lived scopes or when interacting with non-UObject third-party libraries, but avoid them for long-term storage.
- **Smart Pointers**: For custom non-UObject C++ classes, use Epic's smart pointers:
  - `TSharedPtr<T>`: Shared ownership.
  - `TSharedRef<T>`: Shared ownership that cannot be null.
  - `TUniquePtr<T>`: Unique ownership.
  - `TWeakPtr<T>`: Non-owning weak reference to prevent cyclic dependencies.

---

## 10. Error Handling and Logging

- **Assertions**: Use `check()`, `checkf()`, and `ensure()` for conditions that indicate programmer error and should never happen in a correct program.
- **Logging**: Use `UE_LOG` extensively. Define a custom log category for your module (`LogWhatIf`).
- **Log Levels**: 
  - `Log`: General runtime information.
  - `Warning`: Recoverable errors or unexpected states.
  - `Error`: Critical failures that prevent a system from functioning.

---

## 11. Code Review Checklist

Before approving a Pull Request, reviewers must verify:

- [ ] Does the code compile without warnings on all target platforms?
- [ ] Are naming conventions and coding standards adhered to?
- [ ] Is IWYU respected? Are forward declarations used where possible?
- [ ] Are new classes properly categorized into Public/Private?
- [ ] Are all `UFUNCTION` and `UPROPERTY` macros correctly configured (Replication, Blueprint exposure)?
- [ ] Are there any hardcoded values that should be exposed as designer properties or data assets?
- [ ] Are pointer validity checks (e.g., `IsValid()`) in place before dereferencing?
- [ ] Have appropriate logging (`UE_LOG`) statements been added for debugging?
- [ ] Are attribute sets and gameplay tags used instead of hardcoded state flags?
- [ ] Does the architectural change violate any existing ADRs?

---

## 12. Documentation Requirements

- **Header Documentation**: Every class, struct, enum, and public function must have a documentation block above its declaration.
- **Format**: Use standard Javadoc/Doxygen style comments (`/** ... */`).
- **Clarity**: Explain *why* a function exists or *what* a class represents, not just *how* it does it.
- **ADR Updates**: If a system's architecture changes, ensure the corresponding Architectural Decision Record in `DECISIONS.md` is updated or a new one is created.
- **In-line Comments**: Use in-line comments to explain complex algorithms, bitwise operations, or mathematical formulas.

---

## 13. Testing Requirements

Testing is paramount for W.H.A.T. I.F. due to the branching timeline mechanics.

- **Unit Tests**: Core logic, math utilities, and standalone data processors must have accompanying Unreal Automation Tool (UAT) tests. Write tests that isolate the code using mock dependencies.
- **Functional Tests**: Complex interactions (e.g., GAS ability application, timeline branching) should be covered by Level Sequence based Functional Tests within a designated test map.
- **Manual Verification**: PR descriptions must include explicit steps for manual verification in the Editor. Never assume the reviewer knows how to test your specific feature.
- **Performance Profiling**: For systems running every tick, include Unreal Insights profiling data in the PR to prove it does not introduce hitches.

By following these standards, the W.H.A.T. I.F. team ensures a stable, scalable, and high-performance simulation engine ready for any timeline.
