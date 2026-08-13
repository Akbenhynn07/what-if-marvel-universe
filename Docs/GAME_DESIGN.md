# W.H.A.T. I.F. - Game Design Document

## 1. Core Design Pillars

### Pillar 1: Infinite Possibilities, Meaningful Trade-offs
W.H.A.T. I.F. is built on the premise that power always comes at a cost. Transformations should never be purely additive; every new capability, enhancement, or systemic shift introduces vulnerabilities or societal consequences.
* **The "Monkey's Paw" Rule**: Every major upgrade must have a measurable drawback. If a player gains the ability to manipulate fire, they become inherently weaker to extreme cold or lose the ability to regulate their own body temperature in specific biomes.
* **Societal Friction**: Visible mutations cause fear or awe. A character who transforms into a hulking beast will excel in combat but will be unable to access high-security areas requiring subtle diplomacy or biological scanning.
* **Economic Costs**: High-tier tech augmentations require constant upkeep (e.g., thermal paste, specialized batteries). Failure to maintain these items results in performance degradation or critical failures in combat.
* **Resource Dependency**: Transformations that consume significant energy will force the player to prioritize energy-gathering activities, fundamentally altering their moment-to-moment gameplay loops and long-term strategic planning.
* **Psychological Toll**: Transformations that alter the brain (cybernetic mind-links or psychic awakenings) have a measurable impact on the character's AI personality state, sometimes causing them to act erratically or independently if their sanity stat drops too low.

### Pillar 2: Emergent Narrative Through Systemic Simulation
The game relies on deeply interconnected systems rather than hard-coded scripts. The narrative emerges organically from the interactions between character attributes, personality systems, physics, and dynamic environments.
* **No "Golden Path"**: The game does not have a single correct ending or a specific way a quest must be resolved. The engine simulates the world, and the AI reacts to the player's presence and actions.
* **Chain Reactions**: If a player uses an explosive ability in a chemical plant, the resulting fire is handled by the physics engine. The AI guards react to the fire (fleeing, attempting to extinguish it, or using it as a tactical barrier), and the overarching narrative system registers the destruction of the plant, adjusting the global economy and faction standings accordingly.
* **Dynamic Factions**: Factions have goals and resources. They simulate actions in the background. The player's actions might inadvertently help a rival faction succeed in their goals, leading to unexpected alliances or betrayals.
* **Reputation Ecosystem**: Each sub-faction tracks relationship scores independently. Saving a street vendor might endear you to the local gang, but anger the corporate enforcers who were shaking them down.
* **Environmental Memory**: Regions permanently bear the scars of your actions. A cratered street corner where you tested a new explosive transformation will remain cratered, forcing NPCs to navigate around it in future timeline loops.

### Pillar 3: Deterministic Mechanics Meet Generative Flavor
While an LLM powers the descriptive narrative and character dialogue, all underlying mechanics, combat outcomes, and macro-level decisions are resolved deterministically by Unreal Engine and the Gameplay Ability System (GAS).
* **The Engine is King**: The LLM never decides if a shot hits, how much damage it does, or if a character dies. The engine performs these calculations.
* **Narrative Interpretation**: The LLM takes the raw telemetry from the engine (e.g., `Event: Player_CriticalHit; Target: Guard_04; Weapon: PlasmaBlade`) and generates the flavor text or dialogue: "The plasma blade sears through the guard's armor, a blinding flash of ionized gas illuminating the corridor."
* **Consistency Check**: If the LLM generates a response that contradicts the engine state (e.g., claiming a dead enemy is still fighting), the system rejects it and uses a deterministic fallback string.
* **Dialogue Nuance**: The LLM analyzes the player's current transformation state to flavor NPC dialogue. An NPC won't just say "Hello," they will say, "Keep those metal claws away from me, freak" if the player has heavily cybernetic arms.

### Pillar 4: Timeline Divergence as Progression
Failure or unintended consequences aren't the end—they are branching paths. Every significant choice or transformation alters the timeline, turning "Game Over" states into alternate realities to explore.
* **Embracing Failure**: If a player is defeated in combat, they do not reload from a checkpoint. They might be captured, left for dead, or rescued by a rival faction. This creates a new timeline branch.
* **The Multiverse Canvas**: The player's journey is mapped across a branching tree. They can always "jump" back to a previous node and make a different choice, effectively exploring multiple possibilities from a single origin point.
* **Meta-Knowledge**: Characters retain fragmented memories or "echoes" of other timelines, which can occasionally unlock unique dialogue options or provide hints about hidden dangers.
* **Cross-Timeline Resources**: While physical items don't travel between timelines, the *data* about a successful transformation recipe or an enemy's weakness does, allowing the player to progress faster in new branches.

## 2. The Central Loop

The core gameplay loop is designed to guide players through a cycle of creation, testing, consequences, and iteration. This loop is the heartbeat of W.H.A.T. I.F., ensuring that every action feeds back into the player's progression and narrative experience.

### Phase 1: CREATE
* **Initial Concept**: The player starts by defining the baseline human (or baseline alien/synthetic) they will be playing.
* **Archetype Selection**: Players choose from several starting archetypes (e.g., Street Rat, Corporate Operative, Freelance Mercenary). This defines their starting attributes, resources, and faction standing.
* **Initial Traits**: Players can select a few minor traits (e.g., "Fast Metabolism", "Slightly Cybernetic") that provide minor buffs and debuffs.
* **Origin Story Setup**: The initial scenario is seeded based on archetype choices, setting the initial state of the world's conflicts and alliances.

### Phase 2: CUSTOMIZE
* **The Transformation Editor**: This is where the core magic happens. Players apply "Catalysts" to their character.
* **Catalyst Types**:
    * **Biological/Mutagenic**: Alters DNA. Examples: Enhanced muscle density, venom glands, regenerative tissue.
    * **Technological/Cybernetic**: Implants and augments. Examples: Neural jacks, reinforced skeleton, integrated weapon systems.
    * **Arcane/Anomalous**: Manipulates the laws of physics. Examples: Telekinesis, temporal shifting, probability manipulation.
* **The Balancing Act**: Players must carefully weigh the benefits of a catalyst against its drawbacks. A powerful mutation might increase strength by 300% but cause rapid cellular degradation (constant health drain).
* **Synergy Mapping**: Some catalysts combine to create emergent properties. A "Heat Vents" cybernetic combined with a "Thermobaric" mutation might unlock a unique Overload ability.

### Phase 3: SIMULATE
* **Risk-Free Testing**: Before permanently applying a transformation and committing to a timeline branch, the player enters the Simulation Chamber.
* **Data Gathering**: The player tests their new abilities against various scenarios (combat, stealth, traversal) to see how the changes affect their performance.
* **Refinement**: If the build is unsatisfactory, the player can adjust the catalysts or abandon the transformation entirely.
* **Virtual Enemies**: The simulation allows spawning of any previously encountered enemy archetype to test specific counters and resistances.

### Phase 4: PLAY
* **Entering the World**: The player steps out into the dynamic sandbox environment.
* **Interaction**: They engage in missions, explore the world, interact with NPCs, and engage in combat.
* **Real-Time Execution**: All actions are resolved in real-time by the Unreal Engine, utilizing the Gameplay Ability System for accurate and deterministic results.
* **Physics Integration**: Gameplay relies heavily on physics. Throwing heavy objects requires the requisite strength stat, and the damage dealt is calculated by mass and velocity.

### Phase 5: DECIDE
* **Critical Junctures**: The player encounters moments that force a significant choice. These can be tactical (e.g., "Do I use my ultimate ability and risk destroying the objective?") or narrative (e.g., "Do I ally with the corrupt corporation or the radical rebels?").
* **No Easy Answers**: Choices should rarely be black and white. They often involve sacrificing one goal to achieve another.
* **Time Pressure**: Some decisions must be made in real-time during combat or chase sequences, forcing instinctual choices over calculated strategy.

### Phase 6: CONSEQUENCE
* **Systemic Reaction**: The game world reacts to the player's decisions and actions.
* **Ripple Effects**: If a player destroys a corporate facility, corporate security patrols increase, the price of corporate goods skyrockets, and the rebel faction gains territory.
* **Character Evolution**: The player's actions might trigger latent mutations or cause cybernetic systems to degrade, forcing further adaptation.
* **NPC Permadeath**: NPCs killed systemically stay dead in that timeline branch, fundamentally altering future quests that relied on them.

### Phase 7: BRANCH
* **Creating a Node**: When a major event occurs (a significant transformation, a critical narrative decision, or a major failure), the game automatically creates a persistent node in the timeline.
* **The Divergence Point**: This node represents a concrete branch in the multiverse. The state of the world at that exact moment is saved.
* **Branch Labeling**: The LLM automatically generates a descriptive label for the branch based on the preceding events (e.g., "The Bio-Plague Incident," "The Corporate Sellout").

### Phase 8: EXPLORE
* **Navigating the Multiverse**: The player accesses the Timeline Explorer to view the branching paths.
* **Jumping**: The player can choose to "Jump" to a different node, resuming gameplay from that exact point, but exploring a different possibility. The previous timeline remains intact and accessible.
* **Meta-Currency**: Exploring new branches awards a meta-currency used to unlock new starting archetypes and high-tier catalysts for future runs.

## 3. Detailed UI Screen Breakdown

### 3.1 Character Laboratory
The Character Laboratory is the central hub for viewing a character's current state. It is designed to feel like a high-tech medical or magical examination room.

* **Central Display**: A detailed, 3D holographic projection of the character model, reflecting all current physical transformations. Mutations, cybernetics, and gear are visually represented.
* **Attribute Panel (Left)**: Displays the character's core attributes (Strength, Speed, Durability, etc.) with precise numerical values. Hovering over an attribute provides a breakdown of how it's currently modified (e.g., "Strength: 15 (Base 10 + 5 from Cyber-Arm)").
* **Status Effects (Right)**: Shows all active buffs, debuffs, and environmental effects currently affecting the character.
* **Psychological Profile (Bottom)**: A summary of the character's AI personality traits (Aggression, Caution, etc.) and their current mental stability.
* **Relationship Web (Overlay)**: A visual representation of how different factions and key NPCs view the character. Lines connect the character to faction icons; green lines indicate positive relationships, red lines indicate hostility.
* **Inventory Sub-Screen**: A detailed list of currently held items, separated by category (Catalysts, Consumables, Quest Items).

### 3.2 Transformation Editor
The core customization interface where players apply Catalysts.

* **The Node Tree**: Transformations are presented in a sprawling, interconnected node tree. Some nodes are prerequisites for others, and some are mutually exclusive.
* **Catalyst Inventory**: A list of available Catalysts the player has discovered or purchased.
* **Predictive Modeling**: As the player hovers over a potential transformation node, the UI displays predictive data:
    * **Stat Changes**: "+20% Attack Rating, -15% Mobility Rating"
    * **New Abilities**: "Grants ability: Kinetic Dash"
    * **Side Effects**: "High chance of cyber-psychosis if mental stability drops below 20%"
* **The Commitment Button**: Once the player is satisfied with their loadout, they press "Communicate to DNA/Systems" to finalize the transformation, which usually triggers a significant animation and timeline branch.
* **Compatibility Checker**: Highlights conflicting catalysts in red, preventing the player from combining mutually exclusive upgrades (e.g., cannot install cyber-eyes if you have already mutated compound insect eyes).

### 3.3 Simulation Chamber
A risk-free testing ground before committing to a timeline branch.

* **Environment Controls**: The player can select the type of environment for the simulation (e.g., Urban Street, Corporate Lab, Jungle).
* **Enemy Spawner**: The player can spawn specific types of enemies or select pre-configured combat scenarios (e.g., "Assault Squad", "Heavy Mech").
* **Telemetry Overlay**: While in the simulation, a detailed HUD displays DPS meters, mobility tracking (distance covered, time spent airborne), and resource consumption analytics (energy drained per second).
* **Reset Function**: The player can instantly reset the simulation to try different tactics or return to the Transformation Editor.
* **Damage Log**: A scrolling text log showing the precise mathematical calculation of every hit, dodge, and critical strike to help min-max builds.

### 3.4 Battle View
The active gameplay interface. It is designed to be minimalist, keeping the focus on the action and the environment.

* **Health and Energy Bars (Bottom Center)**: Clean, easily readable bars indicating current resources.
* **Ability Hotbar (Bottom Right)**: Displays the currently equipped GAS abilities, their cooldown status, and their energy cost.
* **Dynamic Threat Indicator (Center Screen)**: Subtle visual cues (e.g., directional arrows, screen edge glowing) to indicate incoming attacks or high-threat enemies outside the field of view.
* **Contextual Prompts (Center Screen)**: Button prompts for environmental interactions (e.g., "Press F to hack terminal", "Press E to throw barrel").
* **Tactical Pause Overlay**: When activated, the action freezes, and the camera pulls back slightly. Players can queue up abilities, analyze enemy weaknesses, or assess the environment without time pressure.
* **Status Icons**: Small icons appearing above the health bar to indicate active buffs or debuffs (e.g., Poisoned, Empowered, Slowed).

### 3.5 Timeline Explorer
The meta-progression interface, replacing the traditional "Save/Load" menu.

* **The Multiverse Tree**: A sprawling, luminescent branching tree structure representing the multiverse. Each node represents a significant divergence point.
* **Node Details**: Clicking on a node displays a summary of the events that occurred in that branch (e.g., "Timeline 4A: Defeated the Corporate CEO, but the city was destroyed in the resulting explosion.").
* **Jump Mechanism**: The player can select any node and initiate a "Jump," instantly loading the game state from that point.
* **Timeline Comparison**: Players can compare the stats, resources, and faction standings of two different timelines side-by-side.
* **Pruning Feature**: Players can choose to "Prune" dead-end timelines they have no intention of returning to, freeing up save data space.

### 3.6 Character Codex
The lore and data repository.

* **Auto-Generated Biographies**: The game uses the LLM to generate a narrative biography based on the character's journey through the multiverse, detailing their triumphs, failures, and major transformations.
* **Entity Database**: A log of all encountered enemies, NPCs, and factions, including their known strengths, weaknesses, and relationship with the player.
* **Technology/Mutation Log**: Detailed information on all discovered Catalysts, including lore descriptions and their systemic effects.
* **Event History**: A chronological log of all major narrative events and decisions made in the current timeline branch.
* **Audio Logs / Notes**: A section to replay found audio logs or read intercepted text documents.

### 3.7 Scenario Editor
A sandbox tool for setting up specific world states.

* **World Parameters**: Players can adjust global variables (e.g., starting economy, overall faction hostility, weather patterns).
* **Starting Conditions**: Players can define their starting location, initial resources, and pre-applied transformations.
* **Custom Challenges**: Players can create specific scenarios (e.g., "Survive a horde of mutants with only basic cybernetics") and share them with the community.
* **Mod Support Hooks**: Direct integration for loading custom user-made assets, maps, and JSON scripts.

## 4. Transformation Design Philosophy: The Art of the Trade-off

Transformations are the core progression mechanism in W.H.A.T. I.F., but they are never strictly "upgrades." Every change must adhere to the "Law of Equivalent Exchange" within the game's systems.

### 4.1 Power vs. Stability
Massive power spikes increase the risk of the character losing control.
* **Mechanic**: Powerful abilities might generate "Instability" (a secondary resource or status effect). If Instability reaches a critical threshold, the character might suffer catastrophic failure (e.g., taking massive self-damage) or briefly lose player control as the AI personality takes over.
* **Example**: An "Overclocked Core" implant doubles energy regeneration but constantly generates Heat. If Heat isn't vented (which requires standing still), the core explodes.

### 4.2 Specialization vs. Flexibility
Focusing heavily on one damage type or defense renders the character hyper-vulnerable to specific counters.
* **Mechanic**: Applying a catalyst that grants immunity to fire damage might simultaneously apply a 200% vulnerability to cold damage.
* **Example**: A character who heavily invests in dense, metallic armor becomes incredibly resistant to kinetic weapons but becomes a walking lightning rod for EMPs or electrical attacks, which bypass armor entirely and stun the character.

### 4.3 Social Ramifications
Visible or extreme transformations alter how NPCs interact with the player.
* **Mechanic**: Characters possess a "Concealment Rating" or a "Monstrosity Rating." High Monstrosity prevents peaceful interactions with standard civilian factions and triggers immediate hostility from security forces.
* **Example**: A player who transforms their arms into giant, bladed mantis claws will excel in melee combat but will be unable to operate delicate machinery, hack standard terminals, or enter a bar without inciting a panic.

## 5. How the Game Avoids Being "Just an LLM Story Generator"

W.H.A.T. I.F. prevents feeling like a generic AI text adventure by maintaining strict boundaries between the narrative generation and the systemic simulation.

* **LLMs Do Not Determine Outcomes**: This is the golden rule. The LLM is an observer and narrator, not the arbiter of reality. If a player attempts an attack, the LLM does not decide if it hits. The engine's collision, stats, and GAS calculations determine the hit, damage, and effects. The LLM only contextualizes the result based on engine telemetry.
* **Hard Constraints**: The simulation runs on a rigid physics and state-machine framework. Characters cannot talk their way out of a burning building; the fire damage ticks at a fixed rate, and HP must be managed. The LLM cannot "invent" an escape hatch if the mechanics do not allow it.
* **Systemic Depth over Text Depth**: The primary vector of interaction is through gameplay verbs (move, attack, activate ability, hack), not open-ended text input. Players manipulate the world through mechanics, and the LLM translates those mechanical changes into narrative meaning.
* **Deterministic Fallbacks**: Every systemic action has a deterministic fallback. If the LLM integration goes offline, the game remains fully playable. "The LLM generated dialogue" is replaced by standard barks; "The LLM generated lore" is replaced by template text. The game is a game first.

## 6. Player Agency Design

Agency in W.H.A.T. I.F. is about living with the unforeseen consequences of systemic interactions, rather than choosing from pre-authored dialogue options.

* **Complete Build Control**: Players have absolute control over their character's development. If they want to build a fragile speedster who relies entirely on dodging, they can. If they want to build a slow, heavily armored tank who can barely fit through doors, they can.
* **No "Correct" Choices**: The game does not punish the player for making the "wrong" narrative choice because there are no right choices. Every decision simply leads to a different systemic output.
* **Systemic Problem Solving**: The game presents problems (e.g., a heavily fortified enemy base). It does not present solutions. The player must use their unique combination of abilities, transformations, and environmental interactions to overcome the challenge.
* **Failure as Progression**: As mentioned, failure is integrated into the design. Losing a battle opens new narrative and gameplay loops (capture, rescue, mutation) rather than a simple reload screen, ensuring that the player's agency is always moving the experience forward.

## 7. Save/Load Design: Timeline Branches as Persistent Save States

Traditional saving and loading breaks immersion and reduces the weight of choices. W.H.A.T. I.F. integrates the concept of saving into the core narrative and gameplay loops.

* **The Auto-Branching System**: The game automatically creates a persistent node (a save point) at every major transformation, critical plot juncture, or significant failure state. The player does not manually hit "Save."
* **Navigating Branches**: To "load a save," the player accesses the Timeline Explorer. Returning to an older branch is not "reloading"; it is treating the current timeline as an alternate reality and jumping to a different one.
* **Meta-Progression**: The player character retains meta-knowledge of other timelines. This can manifest as unique dialogue options (e.g., "I know you're going to betray me, you did it in another life"), or as subtle buffs or insights when facing an enemy they've fought in a different branch.
* **Storage Considerations**: To prevent massive file sizes, timeline branches only store the *deltas* (the changes) from the previous node, rather than a full copy of the entire world state every time.

## 8. Extensibility: Mod and Custom Content Design Goals

The architecture of W.H.A.T. I.F. is designed from the ground up to be highly modular and extensible, fostering a strong modding community.

* **Data-Driven Design**: Transformations, items, abilities, and characters are defined in JSON or DataAssets, rather than hard-coded in C++. This makes it incredibly easy for modders to add new content simply by creating new text files or data assets, without needing to recompile the game engine.
* **Custom AI Profiles**: Modders can inject custom personality templates and goal weightings for the AI engine to interpret. Want to create a faction of pacifist robots or hyper-aggressive mutants? Modders can define those behaviors through simple data overrides.
* **The Modding API**: A planned API will allow external creators to hook into the central simulation loop. They will be able to define new GAS attributes, create custom logic for environmental interactions, and even inject their own LLM prompts or backend services to completely alter the game's narrative flavor.
* **Scenario Sharing**: The Scenario Editor is explicitly designed to allow players to create and share custom challenges, starting conditions, and world states with the community, ensuring an infinite supply of new content.
