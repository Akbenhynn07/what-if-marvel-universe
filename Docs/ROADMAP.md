# W.H.A.T. I.F. Development Roadmap

> This document is the authoritative reference for the project's development sequence.  
> Each phase must satisfy all success criteria before the next phase begins.  
> Phase scope ratings: **S** (Small ~days) · **M** (Medium ~weeks) · **L** (Large ~1-2 months) · **XL** (Extra Large ~quarter+)

---

## Phase 0 — Project Architecture & Documentation ✅ COMPLETE

**Goal**: Establish the full development foundation before a single line of gameplay code is written. Architecture, documentation, data contracts, and tooling first.

**Key Deliverables**:
- Complete documentation suite (13 markdown files)
- `WhatIf.uproject` with UE5 5.4, GAS, ModularGameplay, GameFeatures plugins
- C++ module skeleton: `WhatIf` (Runtime) + `WhatIfEditor` (Editor)
- All 13 Public C++ headers (data models + interfaces) — no implementation yet
- JSON Schema (Draft-07) for character data model
- Python schema validation tests (9 test cases)
- Git repository initialized with LFS configuration
- 12 Architectural Decision Records (ADRs) written

**Systems Involved**: Build System, Documentation, Core Interfaces  
**Estimated Scope**: S  
**Dependencies**: None  
**Success Criteria**:
- [x] 46 files committed to Git
- [x] 9/9 schema tests pass
- [x] All ADRs documented
- [ ] C++ headers compile against UE5 5.4 (deferred until engine installed)

---

## Phase 1 — Character Data Model

**Goal**: Implement and validate the full character data pipeline — from data asset definition through async loading to GAS attribute initialization on a live actor.

**Key Deliverables**:
- `UWhatIfCharacterData.cpp` — `GetPrimaryAssetId()`, constructor defaults, validation
- `UWhatIfAttributeSet.cpp` — `PreAttributeChange()`, `PostGameplayEffectExecute()`, OnRep functions, clamping
- `AWhatIfCharacter : public AModularCharacter` — holds `UAbilitySystemComponent` + `UWhatIfAttributeSet`, implements `IAbilitySystemInterface`
- `UWhatIfCharacterComponent` — lightweight actor component that bridges CharacterData → GAS initialization
- AssetManager integration: character loaded async via `FPrimaryAssetId`, attributes initialized from `BaseAttributeValues`
- 2+ sample `UWhatIfCharacterData` assets in Content Browser (generic archetypes, no licensed IP)
- UE5 Automation Tests: attribute initialization, AssetManager load, modifier application

**Systems Involved**: Character System, Attribute System, GAS  
**Estimated Scope**: M  
**Dependencies**: Phase 0 complete, UE5 5.4 installed  
**Success Criteria**:
- Character data asset can be defined in editor with all fields
- Character loaded async → actor spawned → GAS attributes initialized from `BaseAttributeValues`
- A `UGameplayEffect` can successfully modify an attribute and the change is reflected correctly
- Automation tests pass in UE5 Test Runner

---

## Phase 2 — Transformation System

**Goal**: Implement the user-facing core mechanic: apply modular modifications to a character and observe validated, effect-driven stat changes.

**Key Deliverables**:
- `UWhatIfTransformationComponent : UActorComponent` — implements `IWhatIfTransformationSystem`
- `ApplyModification()` — validates compatibility (tags, weight budget, energy capacity), applies `UGameplayEffect`
- `RemoveModification()` — reverses GAS effects cleanly
- `PreviewModificationEffect()` — computes projected stat deltas without applying (for UI display)
- Modification slot enforcement (only one mod per slot type)
- Energy drain system — ongoing attribute drain for active modifications
- Incompatibility tag system — modifications with conflicting tags cannot coexist
- Side effect system — experimental mods apply additional `UGameplayEffect` with negative consequences
- Tests: compatibility validation, weight budget enforcement, effect application and reversal, slot collision

**Systems Involved**: Transformation System, Attribute System, GAS  
**Estimated Scope**: L  
**Dependencies**: Phase 1  
**Success Criteria**:
- Character can have modifications applied and removed with correct stat changes
- Incompatible modifications are rejected with a meaningful error reason
- Weight budget overflow is detected and blocked
- Previewing a modification shows accurate projected attribute deltas
- All tests pass

---

## Phase 3 — Attribute & Modifier System (Deep GAS Integration)

**Goal**: Build out the full modifier stack — base values, equipment modifiers, transformation modifiers, temporary buffs — and validate that derived "effective" values are computed correctly at all times.

**Key Deliverables**:
- `UWhatIfModifierCalculation : UGameplayModMagnitudeCalculation` — custom calculation classes for complex modifiers
- `UGameplayEffect` library: standard damage GE, standard heal GE, standard buff/debuff GEs, energy drain GE
- `FWhatIfAttributeModifier` display pipeline — compute full modifier stack for UI tooltip
- Attribute clamping rules (e.g., Energy never below 0, Accuracy never above 100)
- `PostGameplayEffectExecute()` full implementation — clamp, derive secondary attributes, broadcast delegates
- Attribute change delegates wired to UI bindings
- GameplayTag taxonomy finalized (`Attribute.*`, `Character.*`, `Modification.*`, `Outcome.*`, `Goal.*`, `Status.*`, `Action.*`)
- Full attribute test suite

**Systems Involved**: Attribute System, Transformation System, GAS  
**Estimated Scope**: M  
**Dependencies**: Phase 2  
**Success Criteria**:
- Full modifier stack correctly computes effective values: Base + Equipment + Transformation + Buff = Effective
- Removing any modifier correctly restores prior effective value
- Attributes clamp correctly at boundaries
- GameplayTag taxonomy covers all systems without collision
- Attribute tests cover all 14 attributes and modifier combinations

---

## Phase 4 — Basic Combat Simulator

**Goal**: Implement the deterministic, seeded combat engine. Given two sets of characters and a seed, always produce the same result.

**Key Deliverables**:
- `UWhatIfCombatSimulator : UObject` implementing `IWhatIfCombatSimulator`
- `RunCombat(Setup, Seed)` — full deterministic combat loop
- `RunSingleRound(Setup, CurrentState, Seed)` — step-through for debugging/UI
- Initiative ordering algorithm (Reaction Time + randomized tie-breaking, seeded)
- Action-resolution pipeline: attacker selects action → hit chance calc (Accuracy vs MobilityRating) → damage calc (AttackRating vs DefenseRating vs Durability) → GAS effect application
- Energy consumption per action
- Cooldown management (GAS cooldown GEs)
- Environmental modifier application (`UWhatIfEnvironmentData`)
- Ability-specific resolution (melee, ranged, area, support)
- Termination conditions: all enemies defeated, time limit, resource exhaustion, objective met
- Combat log builder — structured `TArray<FString> CombatLog` for narrative engine input
- Determinism validation test: run same combat 100x with same seed, confirm identical `FWhatIfCombatResult`

**Systems Involved**: Combat System, Ability System, Attribute System, GAS  
**Estimated Scope**: L  
**Dependencies**: Phase 3  
**Success Criteria**:
- Same seed + same setup → identical outcome, every time (100/100 test runs)
- Combat log is human-readable and captures all significant events
- Invalid setup (e.g., no combatants) is correctly rejected by `ValidateCombatSetup()`
- Energy depletion correctly affects available actions

---

## Phase 5 — AI Agent Decision System

**Goal**: Give characters intelligent, personality-driven behavior in combat. Characters should feel meaningfully different from each other based on their personality values.

**Key Deliverables**:
- `UWhatIfDecisionEngine : UObject` implementing `IWhatIfDecisionEngine`
- Utility scoring system: each available action gets a score based on agent state + personality weights
- Personality → weight mapping: e.g., high `Aggression` → Attack actions scored higher; high `Altruism` → Protect actions prioritized
- Threat assessment: `PerceivedThreatLevel` evaluated from enemy attributes vs own attributes
- Goal stack: `EvaluateGoals()` dynamically reorders goal priorities based on state
- Relationship influence: high affinity allies weighted more heavily in protection scoring
- Tactical preference encoding: `Protective` stance weights shield/interpose actions; `Evasive` stance weights retreat
- Personality-based fallback: when no action is clearly optimal, personality determines tiebreaking
- Decision trace logging — record which action was selected and the utility scores for post-sim analysis
- Tests: verify personality-driven behavior (aggressive character chooses attack more than retreating character)

**Systems Involved**: AI Agent System, Decision Engine, Combat System  
**Estimated Scope**: M  
**Dependencies**: Phase 4  
**Success Criteria**:
- Two characters with identical stats but different personalities produce measurably different action distributions over 100 combat rounds
- Decision traces correctly reflect the scoring logic
- Relationship-driven protection decisions occur when an ally is threatened

---

## Phase 6 — Scenario Framework

**Goal**: Package simulation contexts into modular, data-driven scenarios that can be loaded at runtime without modifying engine code.

**Key Deliverables**:
- `UWhatIfScenarioManager : UGameInstanceSubsystem` — loads, validates, and runs scenarios
- Scenario validation pipeline — ensure all referenced characters/environments exist and are loadable
- Event system — `FWhatIfScenarioEvent` evaluated each simulation tick against current world state tags
- Objective tracking — primary/secondary objectives tracked as `FGameplayTagContainer` deltas
- World state — `TMap<FGameplayTag, float>` shared state updated by combat/events/transformations
- 2 complete sample scenarios (generic — no licensed IP): one 1v1, one multi-character
- Game Feature Plugin structure for scenario packs
- Scenario Editor scaffolding (editor-only, Phase 10 polishes the UI)

**Systems Involved**: Scenario System, World System, Game Feature Plugins  
**Estimated Scope**: M  
**Dependencies**: Phase 1, Phase 5  
**Success Criteria**:
- Scenario A and Scenario B load correctly and produce different starting states
- Events trigger when their `TriggerConditions` GameplayTag set is satisfied
- All objectives are tracked and correctly evaluated

---

## Phase 7 — Timeline Branching

**Goal**: Implement the state graph that is the heart of W.H.A.T. I.F. Every simulation run produces a new node; the player can revisit any node and produce a new branch.

**Key Deliverables**:
- `UWhatIfTimelineManager : UGameInstanceSubsystem` — manages the full `UWhatIfTimelineGraph`
- `CreateBranch()` — snapshot current world state + character states → create new `FWhatIfTimelineNode`
- `RestoreNode()` — reload a prior node's snapshot and resume simulation from that point
- World state serialization — `TMap<FName, FString>` snapshot (JSON per-character GAS state)
- Character state serialization — serialize `FWhatIfAgentState` per participant
- Branch reproducibility: restoring node + same seed → identical child outcome
- Save/Load integration: `UWhatIfSaveGame` wraps `UWhatIfTimelineGraph` for disk persistence
- Timeline Explorer data model (UI built in Phase 10)
- Tests: create 3 branches from a single root, verify independence and reproducibility

**Systems Involved**: Timeline System, Save/Load System, Scenario System  
**Estimated Scope**: XL  
**Dependencies**: Phase 6  
**Success Criteria**:
- A simulation run creates timeline nodes automatically at outcome boundaries
- Restoring a prior node and re-running produces a new, independent branch
- The timeline graph can be serialized to disk and reloaded with full fidelity
- 100% branch reproduction verified by test

---

## Phase 8 — Narrative Generation

**Goal**: Connect the deterministic simulation output to an LLM and produce compelling narrative text. Implement the abstraction layer so any LLM can be swapped in.

**Key Deliverables**:
- `UWhatIfNarrativeManager : UGameInstanceSubsystem` — manages narrative requests, rate limiting, caching
- `UWhatIfHttpNarrativeProvider : UObject` implementing `IWhatIfNarrativeProvider` — real HTTP client
- Async HTTP pipeline: `FHttpModule`, JSON serialization of `FWhatIfSimulationResult`, response parsing
- Prompt template system — structured prompt construction from simulation data (facts first, style second)
- Output validator — ensures LLM output doesn't contradict simulation facts (keyword/tag scan)
- Fallback template provider — template-based narrative when LLM unavailable
- `UWhatIfTemplateNarrativeProvider : UObject` — implements `IWhatIfNarrativeProvider` using string templates
- Provider registry — `INarrativeProvider` implementations registered and selectable at runtime
- Narrative cache — avoid re-requesting narrative for an already-visited timeline node
- Configuration: API key managed via environment variable, not hardcoded

**Systems Involved**: Narrative System, AI Layer, Timeline System  
**Estimated Scope**: L  
**Dependencies**: Phase 7  
**Success Criteria**:
- A combat log successfully generates a narrative paragraph via real LLM call
- Fallback provider produces readable (if formulaic) text with no LLM available
- Output validation correctly flags any narrative claiming an incorrect combat outcome
- Provider can be swapped at runtime without restarting the simulation

---

## Phase 9 — Unreal Engine Integration

**Goal**: Connect the simulation systems to UE5 visual gameplay — actors, animations, particle systems, and camera.

**Key Deliverables**:
- `AWhatIfCharacter` Blueprint subclass with skeletal mesh, animation blueprint
- Combat event → animation montage triggers (attack, hit, death, transformation)
- Combat event → Niagara particle system triggers
- Narrative event → camera cut / cinematic sequence (basic)
- GAS `UGameplayEffect` → visual feedback (screen flash, material parameter changes)
- Default map with basic arena environment
- Character spawning from `UWhatIfCharacterData` asset in editor and runtime
- Integration smoke test: run a scenario end-to-end in UE5 editor

**Systems Involved**: Game Layer, Character System, Combat System, Narrative System  
**Estimated Scope**: L  
**Dependencies**: Phase 8  
**Success Criteria**:
- Characters spawn correctly from data assets in a running UE5 level
- Combat produces visible animation and particle responses
- Narrative text appears in a HUD widget during simulation playback

---

## Phase 10 — Character Customization UI

**Goal**: Build all player-facing UI screens. Players should be able to configure a complete simulation run without touching the editor.

**Key Deliverables**:
- Character Laboratory screen — browse and select characters
- Transformation Editor screen — install/remove/preview modifications
- Simulation Chamber screen — configure scenario, participants, environment
- Timeline Explorer screen — navigate timeline graph nodes, see branch labels
- Character Codex screen — view character details, attributes, relationships, history
- All screens built with Common UI + UMG, data-bound via MVVM plugin
- All UE5 widget IDs unique for automation testing

**Systems Involved**: UI System, Character System, Transformation System, Timeline System  
**Estimated Scope**: M  
**Dependencies**: Phase 6, Phase 9  
**Success Criteria**:
- Complete simulation run (select character → apply mod → select scenario → run → view result → explore timeline) navigable entirely via UI
- All attribute values update in real time during modification preview

---

## Phase 11 — Playable Combat Prototype

**Goal**: Deliver a cohesive vertical slice: a complete end-to-end user experience from the main menu through simulation to alternate timeline exploration.

**Key Deliverables**:
- All Phase 1–10 systems integrated and communicating correctly
- Minimum 2 playable scenarios (generic)
- Minimum 4 character archetypes (generic)
- Minimum 12 modification components (3 per slot type)
- Full simulation loop playable by a user with no prior knowledge of the codebase
- Basic onboarding flow (first-run tutorial prompts)
- Performance baseline: simulation tick < 16ms on target hardware

**Systems Involved**: ALL  
**Estimated Scope**: XL  
**Dependencies**: Phase 10  
**Success Criteria**:
- An external playtester can complete a full loop without guidance
- No crashes or data corruption across 10 consecutive simulation runs
- Timeline graph maintains consistency across branching and restoration

---

## Phase 12 — Visual Polish

**Goal**: Elevate the production quality to premium standard.

**Key Deliverables**:
- High-quality UI materials (glassmorphism, gradient, animated borders)
- Smooth animated transitions between all UI screens
- Dynamic music system — adaptive audio responding to simulation tension (via Narrative sentiment tags)
- Particle and post-process polish on transformation events
- UI accessibility pass (text scaling, contrast ratios)

**Systems Involved**: UI Layer, Audio System  
**Estimated Scope**: L  
**Dependencies**: Phase 11  
**Success Criteria**:
- Application "looks and sounds premium" — validated by external design review

---

## Phase 13 — Advanced AI

**Goal**: Expand the AI systems to include rich inter-character dialogue, multi-step planning, and emergent narrative events.

**Key Deliverables**:
- Dialogue System — character-to-character conversations generated by LLM, reflecting personality + relationship + current state
- Multi-step goal planning — agents plan 2-3 steps ahead (look-ahead utility search)
- Emergent narrative events — simulation detects interesting state conditions and proposes narrative beats to the LLM
- Sentiment analysis — LLM output analyzed for emotional tone → drives dynamic music parameters
- Character memory — characters reference earlier timeline events in dialogue

**Systems Involved**: AI Layer, Narrative System, Dialogue System  
**Estimated Scope**: L  
**Dependencies**: Phase 11  
**Success Criteria**:
- Dialogue is contextually accurate (reflects personality, relationships, recent events)
- Multi-step planning produces measurably better tactical outcomes than single-step utility AI

---

## Phase 14 — Multiplayer & Community (Future Scope)

**Goal**: Enable players to share, compare, and build on each other's alternate timelines.

**Key Deliverables** (high-level, not yet specified):
- Timeline seed sharing — players can share a timeline via a seed code
- Asynchronous community scenarios — players propose scenarios, others run them
- Mod portal integration — community-created Game Feature Plugins
- Optional: real-time multiplayer scenario co-op

**Systems Involved**: Multiplayer, Mod System, Community Platform  
**Estimated Scope**: XL  
**Dependencies**: Phase 13  
**Note**: Multiplayer is deliberately deferred. GAS and `AModularCharacter` are already network-ready. Replication strategy to be designed when this phase begins.

---

## Roadmap Principles

1. **No phase is skipped** — each phase fully satisfies its success criteria before the next begins.
2. **No siloed hacks** — if a shortcut would cause technical debt visible in two phases' time, it is not taken.
3. **Architecture review at each phase boundary** — DECISIONS.md is updated with any new ADRs before next phase begins.
4. **Tests before integration** — each system has automated tests before it is connected to adjacent systems.
