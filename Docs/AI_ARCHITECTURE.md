# W.H.A.T. I.F. - AI Architecture Document

## 1. Overview of Layered AI Architecture

The AI in W.H.A.T. I.F. operates on a layered architecture that distinctly separates low-level engine execution from high-level decision making and narrative generation. This ensures systemic integrity while allowing for complex, emergent behavior.

### The Pipeline:
1. **Game State**: The objective, raw data within Unreal Engine (positions, HP, active tags, physics).
2. **Simulation State**: A filtered, abstracted view of the Game State relevant to AI agents.
3. **AI Perception**: The sensory input layer (sight, sound, damage taken, social cues) processed by the agent.
4. **Decision / Planning Layer**: The cognitive core where the agent evaluates goals against perceived reality using Utility Theory.
5. **Action Selection**: Choosing the optimal tactical or social action from the available pool.
6. **Game Engine (GAS)**: Executing the chosen action within the rigid rules of the Gameplay Ability System.
7. **Updated State**: The world changes, and the loop repeats.

## 2. The Agent Model

Every AI entity (and potentially the player, when under simulation control) is defined by a comprehensive struct of data.

* **Goals**: Hierarchical objectives (e.g., Primary: Survive; Secondary: Protect Objective; Tertiary: Eliminate Threat).
* **Personality**: Modifiers that affect how goals are weighed (e.g., 'Aggressive' increases the weight of 'Eliminate Threat').
* **Knowledge**: A memory bank of known facts about the world, the player, and other entities.
* **Perception**: Current sensory data (who they can see, what they hear).
* **Current State**: Internal health, energy, active buffs/debuffs.
* **Available Actions**: The subset of GAS abilities currently off cooldown and usable.
* **Resources**: Ammo, mana, stamina, environmental tools.
* **Threat Assessment**: A calculated value for all perceived entities, determining who is the most dangerous.
* **Tactical Preferences**: Preferred engagement ranges (melee, sniper) and cover utilization.
* **Relationships**: Affinity values towards other factions and specific characters, influencing target selection and support actions.

## 3. Decision Engine (Utility Scoring)

The Decision / Planning Layer uses a Utility AI approach rather than basic state machines or behavior trees.

* **Scoring Actions**: Every available action is evaluated against the current state and goals.
* **Example**: The ability "Use Health Pack" has a utility score calculated by: `(1.0 - CurrentHealthPercentage) * Weight_Survival`. If health is low, utility is high.
* **Goal Weighting**: Personality and situational context dynamically adjust the weights. A "Berserker" might have a low `Weight_Survival` and a high `Weight_Damage`, leading them to prioritize attacking even when near death.
* **Selection**: The agent executes the action with the highest resulting utility score.

## 4. Deterministic Boundary (Engine vs. AI Influence)

It is critical to maintain a hard boundary between what the AI *decides* and what the AI *determines*.

* **AI Influence**: The AI chooses *what* to try. (e.g., "I will shoot the player with my plasma rifle.")
* **Engine Determinism**: Unreal Engine determines *what happens*. (e.g., Did the shot hit? How much damage was dealt? Did the armor mitigate it?)
* The AI cannot bend the rules of the engine. If an agent wants to heal but has no healing ability in its GAS component, it cannot heal.

## 5. LLM Integration

Large Language Models are utilized purely for narrative flavor, dialogue generation, and summarizing complex state changes, isolated from gameplay mechanics.

### 5.1 INarrativeProvider Interface
An abstract interface defining how the game requests narrative context. This allows hot-swapping different LLM backends (local, cloud) without breaking game code.

### 5.2 Structured Input Format
LLMs are not fed raw game state. The simulation state is translated into a structured JSON or prompt format summarizing key context:
```json
{
  "Event": "Combat_End",
  "Winner": "Player",
  "Loser": "Faction_A_Squad",
  "Key_Action": "Player used 'Plasma Nova', causing massive collateral damage.",
  "Faction_A_Current_Relationship": "Hostile"
}
```

### 5.3 Output Validation
Responses from the LLM must adhere to strict formatting (often JSON). The system parses the response. If the response contains invalid data or hallucinates game mechanics (e.g., granting the player an item that doesn't exist), the engine rejects the output.

### 5.4 Async Request Pattern
LLM calls are notoriously slow. All requests are handled asynchronously. The game continues to run, and narrative text or dialogue is buffered and played when ready, ensuring no frame drops or stuttering.

### 5.5 Fallback Behavior
If the LLM backend fails, times out, or returns invalid data, the system falls back to a deterministic, pre-authored string table based on the event tags.

## 6. GAS Interaction with AI

The AI does not manually trigger animations or logic. It interacts exclusively through the Gameplay Ability System.
* The AI Decision Engine outputs a `GameplayTag` representing the desired action.
* The system attempts to activate the GAS ability associated with that tag on the agent.
* This ensures that AI agents follow the exact same cooldown, cost, and execution rules as the player.

## 7. Personality System

Personality is quantified into axes that directly feed into the Utility AI weights.
* **Aggression**: Modifies willingness to engage in combat vs flee.
* **Caution**: Modifies preference for cover and healing.
* **Loyalty**: Modifies willingness to buff/protect allies versus saving oneself.
* **Curiosity**: Modifies tendency to investigate unknown stimuli.

## 8. Threat Assessment Model

Threat is not just "who did the most damage." It is a dynamic calculation:
`Threat = (Base_DPS * Distance_Factor) + (Recent_Damage_Dealt_To_Me * Grudge_Multiplier) + (Role_Priority)`
* **Distance Factor**: Closer targets are generally more threatening.
* **Grudge Multiplier**: Increases if the target recently hurt the evaluator.
* **Role Priority**: Medics or high-value targets might have an innate priority boost.

## 9. Future Implementations

* **Planning Horizon**: Currently, utility AI is reactionary (evaluating current state). Future iterations will implement GOAP (Goal-Oriented Action Planning) to allow agents to string together sequences of actions to achieve a long-term goal.
* **Multi-Agent Coordination**: Introduction of a 'Squad Brain' that can override individual utility scores to coordinate flanking maneuvers, suppressive fire, or synchronized breaches.
