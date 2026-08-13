# W.H.A.T. I.F. Coding Standards

Welcome to the W.H.A.T. I.F. (What Happens After Transformation: Infinite Futures) coding standards document. This guide establishes a baseline for C++ and Blueprint development on this project. By adhering to these standards, we ensure code consistency, readability, and maintainability across our engine modules.

## Table of Contents
1. [Naming Conventions](#naming-conventions)
2. [Public/Private Directory Split Rules](#publicprivate-directory-split-rules)
3. [Include Rules](#include-rules)
4. [UInterface Rules](#uinterface-rules)
5. [Blueprint Guidelines](#blueprint-guidelines)
6. [GAS Coding Patterns](#gas-coding-patterns)
7. [Code Review Checklist](#code-review-checklist)
8. [Documentation Requirements](#documentation-requirements)
9. [Testing Requirements](#testing-requirements)

---

## 1. Naming Conventions

We strictly follow Epic Games' Coding Standard, with specific additions for our modular simulation architecture.

### Classes and Structs
- **Prefixes**: 
  - `U` for classes inheriting from `UObject`.
  - `A` for classes inheriting from `AActor`.
  - `F` for structs and non-UObject classes.
  - `E` for enums.
  - `I` for interface classes.
  - `T` for templates.
  - `S` for Slate widgets.
- **PascalCase**: All types must be in PascalCase (e.g., `UCharacterTransformationData`).

### Variables
- **PascalCase**: All variables should use PascalCase (e.g., `CurrentHealth`, `TransformationState`).
- **Booleans**: Prefix booleans with `b` (e.g., `bIsTransformed`, `bCanTransform`).
- **Pointers**: No special prefix is required, but clarity is key. Prefer `CharacterOwner` over `pCharacterOwner`.

### Functions
- **PascalCase**: Use PascalCase (e.g., `CalculateTransformationCost`).
- **Actions**: Function names should typically begin with a verb (e.g., `Get...`, `Set...`, `Apply...`, `Notify...`).
- **Delegates/Events**: Prefix event handlers with `On` (e.g., `OnTransformationComplete`).

---

## 2. Public/Private Directory Split Rules

Unreal Engine modules support a strict `Public` and `Private` directory separation. We utilize this to enforce encapsulation and compile-time boundaries.

### Public Directory (`Source/Module/Public/`)
- Place header files (`.h`) here *only* if they need to be accessed by other modules or external systems.
- Interfaces (`I...`), pure abstract base classes, and core data types (`UPrimaryDataAsset` structures) typically belong here.
- Avoid placing implementation details or internal helper class headers in the Public directory.

### Private Directory (`Source/Module/Private/`)
- Place all source files (`.cpp`) here.
- Place header files (`.h`) here if they are only used internally within the module.
- Concrete implementations of interfaces should generally keep their headers in the Private folder if external modules only need the interface pointer.

---

## 3. Include Rules

Proper include management minimizes compile times and reduces cyclical dependencies.

- **Include What You Use (IWYU)**: Every header must include the exact headers it requires to compile. Do not rely on transitive includes.
- **Forward Declarations**: Always prefer forward declarations in header files over `#include` directives when possible (e.g., `class UAnimInstance;`).
- **Implementation Includes**: In `.cpp` files, `#include` the corresponding header file first, followed by core engine headers, then module headers, and finally project-specific headers.
- **Precompiled Headers (PCH)**: PCHs are handled by Unreal Build Tool (UBT). Do not manually include a PCH file in your source files.

---

## 4. UInterface Rules

Interfaces are a critical pillar of W.H.A.T. I.F.'s modular architecture.

- **Definition**: Define interfaces using the standard `UInterface` boilerplate.
- **UFUNCTION Annotations**: Ensure that interface functions meant to be implemented or called in Blueprints are marked with `UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="W.H.A.T. I.F.|Interfaces")`.
- **Pure Virtuals**: Avoid pure virtual C++ functions (`virtual void DoThing() = 0;`) in UInterfaces if they need Blueprint support. Rely on the `BlueprintNativeEvent` generation.
- **Implementation Checking**: Use `Implements<UMyInterface>()` or `Cast<IMyInterface>()` to check for interface compliance before executing calls. 
- **Execute Pattern**: When calling an interface function, use the generated `IYourInterface::Execute_FunctionName(ObjectPtr, Args...)` pattern to ensure Blueprint implementations are correctly invoked.

---

## 5. Blueprint Guidelines

While W.H.A.T. I.F. is primarily C++ driven, Blueprints are essential for data definition and designer iteration.

- **Base Classes**: All significant Blueprints (Characters, Abilities, GameModes) must inherit from a custom C++ base class. Avoid deriving directly from engine classes like `AActor` without a project-specific intermediate layer.
- **Logic placement**: 
  - **C++**: Complex state machines, heavy math, network replication logic, and core systems.
  - **Blueprint**: Event hooking, visual effects, audio triggers, UI binding, and iteration-heavy designer data.
- **No Tick**: Avoid using `Event Tick` in Blueprints. Prefer Timers, Timelines, or event-driven logic. If per-frame logic is required, implement it in C++.
- **Variable Categorization**: Always assign Blueprint variables to clear categories (e.g., `Config`, `State`, `References`).

---

## 6. GAS Coding Patterns

The Gameplay Ability System (GAS) manages character attributes, statuses, and transformations.

- **Attribute Sets**: Keep `UAttributeSet` classes small and focused (e.g., `UHealthAttributeSet`, `UManaAttributeSet`). Use the standard GAS macros for getters, setters, and initializers.
- **Gameplay Effects (GE)**: Use Data-Only Blueprints for Gameplay Effects. Never implement logic in a C++ Gameplay Effect class.
- **Gameplay Abilities (GA)**: 
  - Base mechanics in C++ (`UWhatIfGameplayAbility`).
  - Specific ability logic in Blueprint derivatives.
  - Always call `CommitAbility` and `EndAbility` appropriately.
- **Tags**: Use Gameplay Tags extensively for state management instead of boolean flags. Define tags in `DefaultGameplayTags.ini` or C++ native tag registries, never hardcode tag strings.

---

## 7. Code Review Checklist

Before approving a Pull Request, reviewers must verify:

- [ ] Does the code compile without warnings on all target platforms?
- [ ] Are naming conventions and coding standards adhered to?
- [ ] Is IWYU respected? Are forward declarations used where possible?
- [ ] Are new classes properly categorized into Public/Private?
- [ ] Are all `UFUNCTION` and `UPROPERTY` macros correctly configured (Replication, Blueprint exposure)?
- [ ] Are there any hardcoded values that should be exposed as designer properties or data assets?
- [ ] Are pointer validity checks (e.g., `IsValid()`) in place before dereferencing?
- [ ] Have appropriate logging (`UE_LOG`) statements been added for debugging?

---

## 8. Documentation Requirements

- **Header Documentation**: Every class, struct, enum, and public function must have a documentation block above its declaration.
- **Format**: Use standard Javadoc/Doxygen style comments (`/** ... */`).
- **Clarity**: Explain *why* a function exists or *what* a class represents, not just *how* it does it.
- **ADR Updates**: If a system's architecture changes, ensure the corresponding Architectural Decision Record in `DECISIONS.md` is updated or a new one is created.

---

## 9. Testing Requirements

- **Unit Tests**: Core logic, math utilities, and standalone data processors must have accompanying Unreal Automation Tool (UAT) tests.
- **Functional Tests**: Complex interactions (e.g., GAS ability application, timeline branching) should be covered by Level Sequence based Functional Tests.
- **Manual Verification**: PR descriptions must include steps for manual verification in the Editor.

By following these standards, the W.H.A.T. I.F. team ensures a stable, scalable, and high-performance simulation engine.
