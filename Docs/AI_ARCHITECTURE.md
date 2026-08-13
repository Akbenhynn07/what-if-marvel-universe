# W.H.A.T. I.F. - AI Architecture Document

## 1. Overview of Layered AI Architecture

The AI in W.H.A.T. I.F. operates on a sophisticated, layered architecture designed to maintain systemic integrity while allowing for complex, emergent, and highly adaptive behavior. It strictly separates low-level engine execution from high-level decision making and generative narrative processes. This ensures the game always remains mechanically fair and performant, while still exhibiting the illusion of deep, dynamic intelligence.

### The Pipeline Layers:

1.  **Game State (The Engine Reality)**: This is the objective, raw, untempered data within Unreal Engine 5. It includes absolute positions, precise hit points, active Gameplay Tags, physics calculations, and rigid body states. It is the absolute truth of the world. It is calculated in C++ and highly optimized.
2.  **Simulation State (The Filtered Reality)**: A layer that abstracts the raw Game State into information relevant for AI. It filters out unnecessary data (like the specific rotation of a decorative prop) and highlights actionable data (like "Cover Location Available", "Line of Sight Blocked", or "Enemy Sighted").
3.  **AI Perception (The Sensory Layer)**: This layer simulates how an agent experiences the world using Unreal's AIPerception Component. It processes sight (with cone-of-vision calculations and occlusion), sound (attenuated by distance and obstacles), damage taken events, and social cues. It populates the agent's internal "Knowledge" database.
4.  **Decision / Planning Layer (The Cognitive Core)**: The brain of the agent. Here, the agent evaluates its internal Goals against its perceived reality using Utility Theory. It weighs various options based on personality, threat assessments, and current resources.
5.  **Action Selection (The Intent Phase)**: The system chooses the optimal tactical, social, or movement action from the pool of currently available possibilities. This output is usually an intent to activate a specific GameplayTag or move to a specific Navigation Node.
6.  **Game Engine (GAS Execution)**: The chosen action is fed back into the engine, specifically the Gameplay Ability System. The engine attempts to execute the action within the rigid rules of the game (checking cooldowns, costs, range, and collision).
7.  **Updated State (The Consequence)**: The world changes as a result of the action (or failure of the action), feeding new data back into the Game State, and the cycle repeats on the next tick.

## 2. The Agent Model

Every AI entity in the game (and potentially the player character when under simulation control, such as during a timeline jump simulation) is defined by a comprehensive struct of data that dictates its behavior. This is not a state machine, but a data-driven model.

### Agent Data Struct Breakdown:

*   **Goals**: Hierarchical objectives that drive behavior.
    *   *Primary*: Overarching drive (e.g., "Survive", "Protect VIP", "Assassinate Target", "Patrol Route").
    *   *Secondary*: Operational goals (e.g., "Maintain Cover", "Regroup with Squad", "Reload Weapon").
    *   *Tertiary*: Opportunistic goals (e.g., "Loot corpse", "Investigate noise", "Heal minor wound").
*   **Personality**: Modifiers (0.0 to 1.0) that affect how goals are weighed. A highly "Aggressive" personality will heavily weight the "Assassinate Target" goal over the "Maintain Cover" goal.
*   **Knowledge**: A memory bank of known facts.
    *   *World Knowledge*: Known paths, hazard locations, ammo caches, location of healing stations.
    *   *Entity Knowledge*: "The player used a fire attack last time; they might be weak to water." Or "That character is allied with the Police."
*   **Perception**: Current sensory data. Who is visible right now? What sounds were heard in the last 5 seconds? Which allies are in distress?
*   **Current State**: The physical reality of the agent. Current Health, Energy pool, active buffs (e.g., "Empowered"), debuffs (e.g., "Crippled").
*   **Available Actions**: The specific subset of GAS abilities currently off cooldown, within cost limits, and tactically viable.
*   **Resources**: Physical items in inventory. Ammo counts, grenades, health stims, environmental tools.
*   **Threat Assessment**: A constantly updating, calculated value for all perceived entities. Determines target priority.
*   **Tactical Preferences**: Preferred engagement ranges (melee, mid-range, sniper), preference for high ground, or tendency to flank versus charge directly.
*   **Relationships**: A numerical matrix defining affinity values towards other factions and specific characters, heavily influencing target selection and support actions.

## 3. Decision Engine (Utility Scoring)

The Decision / Planning Layer uses a Utility AI approach, offering far more nuance and flexibility than traditional finite state machines or static behavior trees.

### How Utility Theory Works in W.H.A.T. I.F.

Instead of a rigid "If X, then Y" logic structure, Utility AI asks: "Of all the things I *can* do, which one makes the most sense right now based on my personality and current situation?"

1.  **Scoring Available Actions**: Every available action (movement, attacking, healing, fleeing) is continuously evaluated against the agent's current state and active goals.
2.  **The Utility Formula**: Each action has a unique formula that generates a score between 0.0 and 1.0.
    *   *Example: Use Health Pack*
        *   Utility Score = `(1.0 - CurrentHealthPercentage) * Weight_Survival * Supply_Factor`
        *   If Health is at 10%, `(1.0 - 0.1) = 0.9`. The score is high. If Health is 90%, `(1.0 - 0.9) = 0.1`. The score is low.
    *   *Example: Flank Enemy*
        *   Utility Score = `Threat_Level * Distance_To_Cover_Factor * Weight_Tactics * (1.0 - Aggression)`
3.  **Dynamic Goal Weighting**: Personality and situational context dynamically adjust the weights in these formulas.
    *   A "Berserker" archetype might have a fixed low `Weight_Survival` (0.2) and a high `Weight_Aggression` (1.5). This leads them to prioritize attacking even when near death, as the Utility Score for attacking will almost always beat the score for healing.
4.  **Selection and Execution**: The Decision Engine ranks all actions by their final Utility Score. The agent selects the action with the highest score and attempts to execute it. This recalculation happens multiple times a second.

## 4. The Deterministic Boundary (Engine vs. AI Influence)

It is critical to maintain a hard, impenetrable boundary between what the AI *decides* and what the engine *determines*. This ensures the game remains fair, systemic, and mechanically sound.

*   **AI Influence (The Intent)**: The AI chooses *what* it wants to attempt. It sets an intent based on its utility scoring. (e.g., "My highest utility action is to shoot the player with my plasma rifle at these coordinates.")
*   **Engine Determinism (The Reality)**: Unreal Engine, physics, and the Gameplay Ability System determine *what actually happens*.
    *   Did the shot hit? (Calculated by physics traces or hit-chance formulas).
    *   How much damage was dealt? (Calculated by GAS Attribute interactions).
    *   Did the armor mitigate it? (Calculated by Damage Executions in GAS).
*   **No Cheating**: The AI cannot bend or break the rules of the engine. If an agent wants to heal but has no healing ability assigned to its GAS component, or lacks the mana to cast it, it cannot heal, regardless of how high the utility score for healing might be. It must play by the exact same rules as the player.

## 5. LLM Integration

Large Language Models (LLMs) are utilized purely for narrative flavor, dialogue generation, and summarizing complex state changes. They are strictly isolated from core gameplay mechanics to ensure performance, determinism, and safety.

### 5.1 INarrativeProvider Interface
The system uses an abstract C++ interface (`INarrativeProvider`) defining how the game requests narrative context. This allows developers to hot-swap different LLM backends (local models via llama.cpp, cloud APIs like OpenAI or Gemini) without altering any core game code. This future-proofs the game against changes in AI technology.

### 5.2 Structured Input Format
LLMs are never fed raw, chaotic game state data. The Simulation State is carefully translated into a structured, highly constrained JSON or prompt format that summarizes key context efficiently:
```json
{
  "Event_Type": "Combat_Resolution",
  "Primary_Actor": "Player_Character",
  "Target_Entity": "Faction_A_Squad_Leader",
  "Key_Action_Tags": ["Ability.PlasmaNova", "Result.MassiveCollateralDamage"],
  "World_Context": "Location.FuelDepot",
  "Faction_A_Current_Relationship": -80,
  "Player_Mutations_Visible": ["Cybernetic_Arm", "Glowing_Eyes"]
}
```

### 5.3 Output Validation
Responses from the LLM must adhere to strict formatting (usually a defined JSON schema). The game system parses the response.
*   **Anti-Hallucination Measures**: If the response contains invalid data or attempts to alter game mechanics (e.g., the LLM generates: `"The player finds a legendary sword"` when no such item was spawned by the engine), the validation layer rejects the output or strips the invalid data. The LLM can only describe what *is*, it cannot *create* objects or states.

### 5.4 Async Request Pattern
LLM API calls (especially cloud-based) are notoriously slow and variable. All narrative requests are handled entirely asynchronously on background threads.
*   The game simulation never pauses to wait for an LLM response.
*   Narrative text, barks, or dialogue are buffered. When the response arrives and passes validation, it is played or displayed to the player.

### 5.5 Fallback Behavior
Redundancy is key. If the LLM backend fails, times out, loses internet connection, or repeatedly returns invalid data, the system instantly falls back to a deterministic, pre-authored string table based on the event tags, ensuring the player experience is uninterrupted.

## 6. GAS Interaction with AI

The AI system does not manually trigger animations, move actors directly via tick, or apply damage manually. It interacts exclusively through the Unreal Engine Gameplay Ability System (GAS).

*   **Tag-Driven Execution**: The AI Decision Engine outputs a `GameplayTag` representing the desired action (e.g., `Ability.Ranged.Shoot`, `Ability.Defensive.Shield`).
*   **Systemic Activation**: The AI Controller attempts to activate the GAS ability associated with that tag on the agent's `AbilitySystemComponent`.
*   **Parity**: This ensures that AI agents follow the exact same rules, cooldowns, resource costs, animation montages, and execution logic as the player character. If an ability costs 50 Energy, the AI must have 50 Energy to use it.

## 7. Personality System Depth

Personality in W.H.A.T. I.F. is quantified into multi-dimensional axes that directly feed into the Utility AI weights, creating distinct archetypes that act fundamentally differently even with the same abilities.

*   **Aggression (0.0 - 1.0)**: High aggression modifies willingness to engage in combat, reduces the threshold for fleeing, and increases the utility of high-damage, high-risk attacks.
*   **Caution (0.0 - 1.0)**: High caution heavily weights the utility of finding cover, maintaining distance, and using healing or defensive abilities early in engagements.
*   **Loyalty (0.0 - 1.0)**: Modifies willingness to use abilities that buff or protect allies, even at the cost of personal safety. High loyalty agents will prioritize reviving downed squadmates.
*   **Curiosity (0.0 - 1.0)**: Modifies the tendency to investigate unknown stimuli (sounds, visual disturbances). High curiosity agents are easily distracted but harder to sneak past completely.

## 8. The Threat Assessment Model

Threat evaluation is not simply "who did the most damage." It is a dynamic, multi-variable calculation updated constantly to reflect tactical reality:

`Threat = (Base_DPS * Distance_Factor) + (Recent_Damage_Dealt_To_Me * Grudge_Multiplier) + (Role_Priority) + (Transformation_Fear_Factor)`

*   **Distance Factor**: Closer targets are generally more threatening, scaling exponentially as distance decreases.
*   **Grudge Multiplier**: Increases significantly if the target recently damaged the evaluator or its squadmates. Causes enemies to focus fire on whoever hurt them most recently.
*   **Role Priority**: Specific tags (e.g., `Role.Medic`, `Role.Heavy`, `Role.Sniper`) have innate priority boosts. AI will naturally try to eliminate Medics first if possible.
*   **Transformation Fear Factor**: A unique variable to W.H.A.T. I.F. If the player assumes a highly monstrous, glowing, or terrifying form, it artificially inflates their threat level. This might cause low-courage enemies to flee immediately rather than fight, while high-courage enemies will focus all their fire on the "monster."

## 9. Future Architectural Implementations

*   **Goal-Oriented Action Planning (GOAP)**: While the current Utility AI is excellent at reactionary decision making, future iterations will implement GOAP to allow agents to string together complex sequences of actions to achieve a long-term goal across the Simulation State (e.g., "Find keys -> unlock door -> retrieve weapon -> hunt player").
*   **Squad Brain Coordination**: The introduction of a hierarchical 'Squad Brain' AI director. This system will be able to override individual agent utility scores to coordinate complex group tactics, such as flanking maneuvers, suppressive fire setups, or synchronized room breaches, rather than relying purely on emergent behavior.
*   **Long-Term Memory Synthesis**: Using the LLM backend to periodically summarize an agent's experiences into a compact "Memory" block, which is then fed back into future context prompts, allowing persistent NPCs to remember the player's specific tactics across multiple encounters and timeline branches.

## 10. Advanced Threat Analysis Mechanics

The AI evaluates threat based on historical, predictive, and environmental data.
*   **Historical Threat (Grudges)**: If the player character frequently attacks a specific faction across multiple timeline branches, the AI might receive a global meta-knowledge prompt that increases the player's base threat. 
*   **Predictive Threat (Telegraphing)**: If the player begins charging a high-damage area-of-effect ability (telegraphed via GAS effects), the AI's threat assessment for that location spikes instantly, causing them to prioritize evasion utilities over attack utilities.
*   **Environmental Threat Assessment**: AI constantly samples the NavMesh for environmental hazards (e.g., radiation zones, fire patches). These zones artificially lower the utility of moving through them unless the agent possesses an immunity tag (e.g., `Status.Immune.Fire`).

## 11. LLM Prompt Construction Strategies

When constructing the JSON prompt for the narrative LLM, efficiency and determinism are prioritized over extreme verbosity.

### 11.1 Example Prompt Structure:
```json
{
  "SystemInstruction": "You are the narrator of W.H.A.T. I.F. Describe the consequence of the following event in one dramatic sentence, focusing on the sensory details.",
  "SimulationEvent": {
    "Type": "AbilityExecution",
    "Source": "Player",
    "Target": "Security_Drone_Alpha",
    "Ability": "Kinetic_Slam",
    "Result": "Target Destroyed, Collateral Damage to Nav_Sector_4"
  }
}
```

### 11.2 Output Parsing rules:
The LLM output is parsed directly into the UI's dialogue system. If the LLM generates multiple paragraphs when only one sentence was requested, the parser aggressively truncates it to prevent UI overflow. If the LLM output violates tone constraints (e.g., becomes overly comedic in a grimdark setting), it is discarded and replaced with a static string.

## 12. Performance Budgets

The AI architecture operates under strict performance budgets per frame.
*   **Perception Budget**: Raycasts for sight are staggered across multiple frames. Not every AI checks sight lines every tick.
*   **Utility Calculation Budget**: Utility scores are cached and only recalculated when significant state changes occur (e.g., taking damage, seeing a new enemy) or on a low-frequency timer (e.g., every 0.5 seconds), rather than every tick.
*   **Pathfinding Budget**: Long-distance pathfinding requests are queued and processed asynchronously on background threads to prevent hitches.

## 13. Testing and Debugging Tools

A suite of tools exists for debugging AI behavior:
*   **Utility Visualizer**: An editor overlay that shows floating numbers above each agent's head, indicating their current highest-scored utility and the active target.
*   **Perception Cone Visualizer**: Draws the sight and hearing radii of selected agents.
*   **LLM Mock Server**: A local tool that mocks LLM responses instantly to test the narrative parsing logic without making expensive cloud API calls.
*   **Timeline State Editor**: Allows developers to forcibly inject custom state data into the simulation to observe how the AI reacts to extreme edge cases.

## 14. Combat AI Behavior Trees vs Utility Systems

While we primarily rely on Utility AI, we do use traditional Behavior Trees (BTs) for very specific, tightly-controlled sub-tasks.
*   **When to use Utility AI**: The overarching decision of *what* to do (e.g., "Should I attack or flee?"). This is the high-level brain.
*   **When to use Behavior Trees**: The specific execution of *how* to do a complex, multi-step action. For example, if the Utility AI decides the best action is to "Reload Weapon", it might trigger a small BT that handles: 
    1. Check if in cover. 
    2. If not, move to nearest cover. 
    3. Play reload animation. 
    4. Wait for animation to finish. 
    5. Update ammo count.
*   **The Handshake**: The Utility Engine operates on a high frequency, updating its scores constantly. When an action is selected, it passes control to the BT. The Utility Engine continues to evaluate in the background. If a new action suddenly outscores the current one by a large margin (e.g., a grenade lands at the agent's feet while they are reloading), it issues an interrupt signal to the BT, aborting the reload and forcing an evasion maneuver.

## 15. The Role of the Navigation Mesh

The AI relies heavily on Unreal's dynamic Navigation Mesh (NavMesh) for spatial reasoning.
*   **Dynamic Generation**: Because transformations can alter the environment (e.g., creating craters, destroying walls), the NavMesh is set to dynamically rebuild at runtime in areas of active conflict.
*   **NavModifiers**: We use custom NavModifier volumes to influence pathing. Areas of high danger (like a burning room) are dynamically painted with high-cost NavModifiers. The pathfinding algorithm will attempt to route agents around these areas unless the agent possesses an immunity (like Fire Resistance), in which case the cost is ignored.
*   **Verticality**: The NavMesh is configured to support complex vertical movement. AI agents with specific movement tags (e.g., `Movement.Jump.Enhanced`, `Movement.Fly`) can evaluate and traverse NavLinks that standard ground units cannot, allowing for true 3D combat encounters.

## 16. Performance Metrics and Fallback
If the simulation state becomes too complex and the game detects a sustained frame drop below 60fps (or 30fps on minimum spec hardware), the AI Director will automatically cull less important background AI behaviors (e.g. idle chatter, distant patrol recalculations) and prioritize processing power exclusively for AI currently engaged in combat with the player.



