# W.H.A.T. I.F. - Game Design Document

## 1. Core Design Pillars

### Pillar 1: Infinite Possibilities, Meaningful Trade-offs
W.H.A.T. I.F. is built on the premise that power always comes at a cost. Transformations should never be purely additive; every new capability, enhancement, or systemic shift introduces vulnerabilities or societal consequences. 

### Pillar 2: Emergent Narrative Through Systemic Simulation
The game relies on deeply interconnected systems rather than hard-coded scripts. The narrative emerges organically from the interactions between character attributes, personality systems, physics, and dynamic environments. The AI adapts to the player's choices, creating a unique, responsive storyline.

### Pillar 3: Deterministic Mechanics Meet Generative Flavor
While an LLM powers the descriptive narrative and character dialogue, all underlying mechanics, combat outcomes, and macro-level decisions are resolved deterministically by Unreal Engine and the Gameplay Ability System (GAS). The game remains fundamentally a game, not just an open-ended chat interface.

### Pillar 4: Timeline Divergence as Progression
Failure or unintended consequences aren't the end—they are branching paths. Every significant choice or transformation alters the timeline, turning "Game Over" states into alternate realities to explore. The concept of saving and loading is integrated directly into the gameplay loop as timeline exploration.

## 2. The Central Loop

The core gameplay loop is designed to guide players through a cycle of creation, testing, consequences, and iteration:

1. **CREATE**: The player conceptualizes a baseline character or selects an archetype.
2. **CUSTOMIZE**: The player applies transformations, mutations, or technological augments.
3. **SIMULATE**: The engine calculates the theoretical impact of the changes on the world state.
4. **PLAY**: The player engages in active gameplay (combat, exploration, dialogue) with the new loadout.
5. **DECIDE**: The player is faced with critical narrative or tactical choices during gameplay.
6. **CONSEQUENCE**: The game world reacts. Factions shift, environments degrade, allies react.
7. **BRANCH**: A timeline divergence is recorded. The new state becomes a concrete branch.
8. **EXPLORE**: The player explores the new timeline, or steps back to a previous node to try a different approach.

## 3. UI Screen Breakdown

### 3.1 Character Laboratory
The central hub for viewing a character's current state.
* **Visuals**: A high-tech medical or magical examination room. The character model is displayed centrally, reflecting all current physical transformations.
* **Elements**: Detailed attribute breakdowns, current status effects, psychological profile summaries, and a relationship web showing how factions view the character.

### 3.2 Transformation Editor
The core customization interface.
* **Mechanics**: Players equip "Catalysts" (genetic modifications, cybernetics, arcane blessings).
* **UI**: A node-based skill tree meets a loadout screen. As players hover over a potential transformation, predictive data shows the expected changes to base stats and the likely side effects (e.g., "Increased Strength, but high chance of cyber-psychosis").

### 3.3 Simulation Chamber
A testing ground before committing to a timeline branch.
* **Function**: Players can drop their newly transformed character into a holographic sandbox environment to test abilities and combat feel against dummy targets or simulated enemies.
* **Feedback**: DPS meters, mobility tracking, and resource consumption analytics.

### 3.4 Battle View
The active gameplay interface.
* **HUD**: Minimalist. Focuses on spatial awareness, cooldowns for active GAS abilities, and a dynamic threat indicator.
* **Interaction**: Real-time action combat or tactical pause, allowing players to execute complex combos and utilize environmental interactions.

### 3.5 Timeline Explorer
The meta-progression interface.
* **Visuals**: A sprawling, luminescent branching tree structure representing the multiverse.
* **Functionality**: Replaces the traditional "Save/Load" menu. Players can view key nodes where major decisions or transformations occurred, read summaries of what happened in those branches, and choose to "Jump" to them to continue playing from that point.

### 3.6 Character Codex
The lore and data repository.
* **Content**: Automatically generated biographies based on the character's journey, logs of all encountered enemies, discovered technologies, and systemic interactions.

### 3.7 Scenario Editor
A sandbox tool for setting up specific world states.
* **Use Case**: Allows players to define starting conditions, enemy faction strength, and available resources before generating a new timeline seed.

## 4. Transformation Design Philosophy

Transformations must adhere to the "Law of Equivalent Exchange" within the game's systems.
* **Power vs. Stability**: Massive power spikes increase the risk of the character losing control (managed via the personality/AI system).
* **Specialization vs. Flexibility**: Focusing heavily on one damage type or defense renders the character hyper-vulnerable to specific counters.
* **Social Ramifications**: Visible or extreme transformations alter how NPCs interact with the player. Becoming a hulking monstrosity grants physical dominance but locks out subtle diplomacy paths.

## 5. Avoiding the "Just an LLM Story Generator" Trap

W.H.A.T. I.F. prevents feeling like a generic AI text adventure by maintaining strict boundaries:
* **LLMs Do Not Determine Outcomes**: If a player attempts an attack, the LLM does not decide if it hits. The engine's collision, stats, and GAS calculations determine the hit, damage, and effects. The LLM only *narrates* or *contextualizes* the result based on engine telemetry.
* **Hard Constraints**: The simulation runs on a rigid physics and state-machine framework. Characters cannot talk their way out of a burning building; the fire damage ticks at a fixed rate, and HP must be managed.
* **Systemic Depth over Text Depth**: The primary vector of interaction is through gameplay verbs (move, attack, activate ability, hack), not open-ended text input.

## 6. Player Agency Design

Agency in W.H.A.T. I.F. is about living with the unforeseen consequences of systemic interactions.
* Players have complete control over their build and approach.
* The game does not force "correct" choices, only choices with different systemic outputs.
* "Failure" is integrated into the design. Losing a battle might result in capture and subsequent modification by an enemy faction, opening a completely new gameplay loop rather than a reload screen.

## 7. Save/Load Design: The Timeline System

Traditional saving and loading breaks immersion and reduces the weight of choices. In W.H.A.T. I.F.:
* **Auto-Branching**: The game automatically creates a persistent node at every major transformation or plot juncture.
* **Navigating Branches**: To "load a save," the player accesses the Timeline Explorer. Returning to an older branch treats the current timeline as an alternate reality. The player character retains meta-knowledge of other timelines, which can occasionally unlock unique dialogue options or insights.

## 8. Extensibility: Mod and Custom Content Goals

The architecture is designed to be highly modular.
* **Data-Driven Design**: Transformations, items, and abilities are defined in JSON or DataAssets, making it easy for modders to add new content without recompiling C++.
* **Custom AI Profiles**: Modders can inject custom personality templates and goal weightings for the AI engine to interpret.
* **Modding API**: A planned API will allow external creators to define new GAS attributes and tie them into the central simulation loop.
