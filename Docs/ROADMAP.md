# W.H.A.T. I.F. Development Roadmap

This document outlines the 15-phase development plan for the W.H.A.T. I.F. engine. 

---

### Phase 0: Project Architecture & Documentation
- **Goal**: Establish the foundational structure, coding standards, and system designs.
- **Key Deliverables**: README, ARCHITECTURE, ROADMAP, initial UE5.4 project setup, Git LFS configuration, module definition (WhatIf, WhatIfEditor).
- **Systems Involved**: Core, Build System
- **Estimated Scope**: Small (S)
- **Dependencies**: None
- **Success Criteria**: Project compiles successfully, documentation is approved, repository is accessible.

### Phase 1: Character Data Model
- **Goal**: Define the foundational data structures for entities in the world.
- **Key Deliverables**: Base Character classes, Primary Data Assets for character definitions, basic GAS integration setup.
- **Systems Involved**: Character System, Data Layer
- **Estimated Scope**: Medium (M)
- **Dependencies**: Phase 0
- **Success Criteria**: Can spawn a data-driven character in an empty map with basic identity tags.

### Phase 2: Transformation System
- **Goal**: Implement the core mechanic of altering a character's fundamental state.
- **Key Deliverables**: Transformation component, event triggers, state swapping logic.
- **Systems Involved**: Transformation System, Character System
- **Estimated Scope**: Large (L)
- **Dependencies**: Phase 1
- **Success Criteria**: Triggering a transformation correctly swaps out the character's visual data and underlying capability identifiers.

### Phase 3: Attribute & Modifier System
- **Goal**: Flesh out the Gameplay Ability System integration for hard stats.
- **Key Deliverables**: `UAttributeSet` definitions (Health, Power, Sanity, etc.), Gameplay Effects for buffs/debuffs/transformations.
- **Systems Involved**: Attribute System, Transformation System
- **Estimated Scope**: Medium (M)
- **Dependencies**: Phase 2
- **Success Criteria**: Character stats are accurately modified when a Transformation is applied.

### Phase 4: Basic Combat Simulator
- **Goal**: Create a deterministic resolution engine for conflicts.
- **Key Deliverables**: Combat loop logic, action queuing, math models for resolving damage and state changes.
- **Systems Involved**: Combat System, Ability System
- **Estimated Scope**: Large (L)
- **Dependencies**: Phase 3
- **Success Criteria**: Two characters can automatically resolve a conflict based purely on their stat discrepancies and assigned abilities.

### Phase 5: AI Agent Decision System
- **Goal**: Give characters the ability to choose actions based on their current state.
- **Key Deliverables**: Utility AI or Behavior Tree implementation for selecting abilities in the Combat/Simulation loop.
- **Systems Involved**: AI Agent System, Combat System
- **Estimated Scope**: Medium (M)
- **Dependencies**: Phase 4
- **Success Criteria**: Characters autonomously select logical actions during a simulation tick rather than relying on random selection.

### Phase 6: Scenario Framework
- **Goal**: Package specific starting conditions into playable modules.
- **Key Deliverables**: Scenario Data Assets, World Manager for loading conditions, Game Feature Plugin integration.
- **Systems Involved**: Scenario System, World System, Mod System
- **Estimated Scope**: Medium (M)
- **Dependencies**: Phase 1, Phase 5
- **Success Criteria**: Can load "Scenario A" and "Scenario B" via plugins and see different characters and starting states.

### Phase 7: Timeline Branching
- **Goal**: Implement the graph-based reality tracker.
- **Key Deliverables**: Timeline Node data structure, Timeline Manager, branch creation logic on major events.
- **Systems Involved**: Timeline System, Save/Load System
- **Estimated Scope**: Extra Large (XL)
- **Dependencies**: Phase 6
- **Success Criteria**: A simulation run generates a queryable graph of events; players can reload to a previous node and force a different outcome, creating a new branch.

### Phase 8: Narrative Generation
- **Goal**: Connect the deterministic simulation to the LLM for translation.
- **Key Deliverables**: LLM REST client, Prompt generation from simulation logs, JSON parsing of AI responses.
- **Systems Involved**: AI Layer, Narrative System
- **Estimated Scope**: Large (L)
- **Dependencies**: Phase 7
- **Success Criteria**: A combat log is successfully translated into a compelling 3-paragraph story beat by an external API.

### Phase 9: Unreal Engine Integration
- **Goal**: Deepen the integration of the text/systems with UE5 visuals.
- **Key Deliverables**: Tying narrative events to basic camera cuts, particle system triggers, or animation montages.
- **Systems Involved**: Game Layer, Narrative System
- **Estimated Scope**: Large (L)
- **Dependencies**: Phase 8
- **Success Criteria**: When the text describes a powerful attack, the game world visually reflects this with a basic particle effect and animation.

### Phase 10: Character Customization UI
- **Goal**: Build the player-facing interface for setting up initial conditions.
- **Key Deliverables**: Common UI screens for selecting characters, tweaking base attributes, and selecting Scenarios.
- **Systems Involved**: UI System
- **Estimated Scope**: Medium (M)
- **Dependencies**: Phase 6
- **Success Criteria**: A player can navigate a main menu, configure a scenario, and start the simulation without using the UE5 editor.

### Phase 11: Playable Combat Prototype
- **Goal**: Deliver a cohesive vertical slice of the engine.
- **Key Deliverables**: Integration of UI, Combat Simulation, Timeline tracking, and Narrative generation into a single playable loop.
- **Systems Involved**: ALL
- **Estimated Scope**: Extra Large (XL)
- **Dependencies**: Phase 10
- **Success Criteria**: A complete end-to-end user experience from main menu to viewing an alternate timeline outcome.

### Phase 12: Visual Polish
- **Goal**: Elevate the presentation quality.
- **Key Deliverables**: High-quality UI materials, transitions, audio integration (dynamic music based on narrative tension).
- **Systems Involved**: UI Layer, Audio System
- **Estimated Scope**: Large (L)
- **Dependencies**: Phase 11
- **Success Criteria**: The application looks and sounds like a premium product.

### Phase 13: Advanced AI
- **Goal**: Implement more complex AI features.
- **Key Deliverables**: Dialogue System (characters conversing with each other based on hidden prompts), sentiment analysis to drive visual changes.
- **Systems Involved**: Dialogue System, AI Layer
- **Estimated Scope**: Large (L)
- **Dependencies**: Phase 11
- **Success Criteria**: Characters generate dynamic, context-aware dialogue that accurately reflects their internal stats and recent timeline events.

### Phase 14: Multiplayer/Community (Future)
- **Goal**: Enable community sharing and interaction.
- **Key Deliverables**: Sharing timeline seeds, asynchronous multiplayer scenarios, Mod portal integration.
- **Systems Involved**: Game Layer, Mod System
- **Estimated Scope**: Extra Large (XL)
- **Dependencies**: Phase 13
- **Success Criteria**: Players can seamlessly share and explore each other's alternate timelines.
