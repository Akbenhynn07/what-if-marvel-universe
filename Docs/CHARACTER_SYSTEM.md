# W.H.A.T. I.F. - Character System Document

## 1. Character Data Model Hierarchy

The character system in W.H.A.T. I.F. is built upon a robust, data-driven hierarchy utilizing Unreal Engine 5's architecture. It is designed to handle thousands of unique variants, mutations, and permutations across the multiverse without incurring massive performance or memory overhead.

### Base Classes
1.  **`UCharacterData` (UPrimaryDataAsset)**: The immutable, data-only blueprint defining a character archetype. This exists purely in data and defines *what* a character is.
2.  **`AWhatIfCharacter` (ACharacter)**: The in-world actor instance. This is the physical representation running around in the physics engine.
3.  **`UCharacterComponent` (UActorComponent)**: The central nervous system of the character. It manages the runtime state, linking the physical actor to the Gameplay Ability System (GAS), managing the inventory, and tracking dynamic data (like current timeline variations).
4.  **`UWhatIfAttributeSet` (UAttributeSet)**: The GAS-specific container for all numerical stats, handling all complex mathematical interactions and modifications.

### Field-by-Field Breakdown of `UCharacterData`
*   `FName CharacterID`: Unique, persistent identifier used for save data and database lookups (e.g., "Char_Protagonist_V1").
*   `FText DisplayName`: Localized character name presented in the UI.
*   `FText Description`: The lore summary or biography of the archetype.
*   `TSoftObjectPtr<USkeletalMesh> BaseMesh`: The visual representation. (Crucially a Soft Pointer, see Section 4).
*   `TSubclassOf<UAnimInstance> AnimClass`: The animation blueprint associated with this archetype.
*   `TSubclassOf<UGameplayAbility> InnateAbilities`: An array of default abilities granted upon spawning.
*   `FCharacterBaseStats InitialStats`: The starting numerical attributes before any mutations are applied.
*   `FPersonalityData BasePersonality`: The AI archetype defining initial behavior weights.
*   `TArray<FCharacterConstraint> Constraints`: Rules defining what transformations or items can be applied to this character (e.g., "Cannot equip Cybernetics").

## 2. The UPrimaryDataAsset Pattern

W.H.A.T. I.F. heavily utilizes `UPrimaryDataAsset` for all definitions of characters, transformations, weapons, and items.
*   **Why**: It allows game designers to create, balance, and iterate on characters entirely within the Unreal Editor without needing a C++ programmer to recompile the project.
*   **Memory Management**: By using Primary Data Assets, these objects are not automatically loaded into memory. They are merely indexed by the engine. They only consume memory when explicitly requested by the AssetManager.
*   **Inheritance**: Designers can create a Base `UCharacterData` (e.g., "Standard Guard") and then create child Data Assets that inherit and slightly modify those stats (e.g., "Elite Guard", "Sniper Guard"), promoting reuse and consistency.

## 3. AssetManager Integration

The `UWhatIfAssetManager` (a custom class derived from Unreal's `UAssetManager`) handles the entire lifecycle of Character Data.
*   **Async Loading Excellence**: When a timeline branch is loaded, or a new character needs to spawn, the AssetManager asynchronously loads the required `UCharacterData` via its `FPrimaryAssetId`. This prevents framerate hitches or stutters during gameplay.
*   **Global Registry**: It maintains a global registry of all available characters, mutations, and items. This allows the UI and the Simulation Engine to easily query data, such as: "Get a list of all valid, unlockable biological mutations for the current player class."
*   **Memory Unloading**: The AssetManager is also responsible for garbage collecting assets that are no longer needed when jumping between drastically different timeline branches.

## 4. TSoftObjectPtr Usage Rules

Memory management is the biggest hurdle in a multiverse game.
*   **Hard References are Strictly Forbidden**: `UCharacterData` must NEVER use hard pointers (e.g., `UTexture2D*`, `USkeletalMesh*`, `UParticleSystem*`) for bulky visual or audio assets.
*   **The Rule**: Always use `TSoftObjectPtr<T>`.
*   **The Reasoning**: If Character A has a hard reference to a 50MB texture, simply loading the `UCharacterData` for Character A loads that 50MB texture into RAM. In a game with potentially thousands of character variants and mutations, this will instantly cause out-of-memory crashes. Assets are only resolved from Soft Pointers to Hard Pointers when the character is actively spawned into the physics simulation.

## 5. Full Attribute List (`UWhatIfAttributeSet`)

The monolithic Attribute Set defines every numerical value a character possesses.

*   **Strength**: Modifies melee physical damage, dictates carrying capacity, and influences physics interactions (e.g., how far an enemy is thrown).
*   **Speed**: Base movement velocity, sprint multiplier, and jump height calculations.
*   **Durability**: Innate damage reduction (Armor) specifically against physical kinetic attacks.
*   **Energy**: The current resource pool for casting abilities, powers, or powering cybernetics.
*   **MaxEnergy**: The upper limit of the Energy pool.
*   **Intelligence**: Modifies the effectiveness of tech-based abilities, hacking speeds, and experience gain rates.
*   **DefenseRating**: A general modifier against all incoming damage types (Kinetic, Thermal, Arcane, etc.).
*   **AttackRating**: A general multiplier to all outgoing damage types.
*   **MobilityRating**: Affects dodge chance calculations, the speed of evasion animations, and fall damage thresholds.
*   **ReactionTime**: Reduces the delay before AI reacts to new stimuli; for players, it increases the timing windows for parries and counters.
*   **Accuracy**: Reduces weapon spread cones and increases the baseline critical hit chance.
*   **RecoveryRate**: The passive regeneration amount of Health and Energy per second.
*   **TacticalCapability**: Increases the utility score of squad commands and expands the AI's perception radius.
*   **ResourceEfficiency**: Reduces the Energy, Ammo, or Stamina cost of all activated abilities.

## 6. Modifier Stack Explanation

Character stats are never permanently overwritten. The system uses a robust Modifier Stack (handled by GAS GameplayEffects) to ensure data integrity, especially during complex transformations.
*   **Base Value**: The core number defined by the `UCharacterData`.
*   **Current Value**: The final calculated total after all modifiers are applied.
*   **The Stack**: When a character gains a "Cybernetic Arm", it applies an infinite-duration `UGameplayEffect`. This effect might add +20 to Strength and -5 to Mobility. If the arm is destroyed or unequipped, the `UGameplayEffect` is removed, and the stats revert flawlessly to their previous state. This prevents "stat bloat" and calculation errors during complex build changes.

## 7. GAS AttributeSet Design

We use a monolithic `UWhatIfAttributeSet` to centralize calculations rather than splitting them into multiple sets.
*   Every attribute is defined as an `FGameplayAttributeData`.
*   **`PreAttributeChange`**: Overridden to enforce strict clamping rules. (e.g., preventing Health from dropping below 0 before damage is finalized, or ensuring CurrentEnergy never exceeds MaxEnergy).
*   **`PostGameplayEffectExecute`**: Used to handle systemic reactions to stat changes. For example, if the Health attribute hits exactly 0 after an effect executes, this function triggers the Death sequence or "Downed" state tag.

## 8. ATTRIBUTE_ACCESSORS Macro

To reduce massive amounts of boilerplate code in the header file, we utilize the standard GAS macro pattern:
```cpp
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
```
This macro is placed under every single attribute definition in `UWhatIfAttributeSet.h`. It automatically generates the standard getters, setters, and initialization functions required by the Unreal Engine reflection system and the Gameplay Ability System.

## 9. Relationship System

Relationships are tracked via a dedicated component attached to the character (`URelationshipComponent`).
*   **Data Structure**: Uses a `TMap<FName, float>`, mapping a Faction ID or Character ID to a relationship score ranging from -100 (Nemesis) to 100 (Blood Brother).
*   **Dynamic Adjustment**: Actions in the Simulation State fire events. The `URelationshipComponent` listens for these events. Healing an ally increases the score. Damaging a neutral faction member severely decreases it. These scores directly influence AI target selection and dialogue options generated by the LLM.

## 10. AI Personality Struct

This struct is the DNA of the AI behavior for a specific character archetype.
```cpp
USTRUCT(BlueprintType)
struct FPersonalityData
{
    GENERATED_BODY()
    
    // Values range from 0.0 to 1.0
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Personality") 
    float Aggression; 
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Personality") 
    float Caution;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Personality") 
    float Loyalty;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Personality") 
    float Curiosity;
};
```
This struct is read by the AI Controller upon possessing the character, initializing the Utility AI weights to ensure the character behaves according to their lore description.

## 11. Character Identity vs. Character Instance

Understanding this distinction is vital for the Multiverse/Timeline system.
*   **Identity**: The meta-concept of the character across the entire multiverse (e.g., "The Player", or a persistent rival like "Subject Delta"). This data is stored globally in the save system, tracking their overarching journey and branches.
*   **Instance**: The specific physical actor currently running in the engine in one specific timeline branch at this exact moment.
*   **The Process**: When jumping timelines, the Identity's data is preserved, but the physical Instance is destroyed. A new Instance is reconstructed in the new timeline based on the specific historical data recorded for that specific branch.

## 12. Constraints and Incompatibilities System

Transformations use a robust, tag-based constraint system utilizing `FGameplayTagContainer` to prevent logical or mechanical conflicts.
*   **Application**: A "Biological Mutation" ability might apply the state tag `State.BodyType.Mutated`.
*   **Requirement/Incompatibility**: A "Cybernetic Skeleton" item might have a requirement tag `State.BodyType.PureBio` and an incompatibility tag `State.BodyType.Mutated`.
*   **Validation**: The system queries these tags before allowing a transformation in the UI, physically preventing the player from equipping incompatible upgrades and ensuring narrative consistency.

## 13. How to Add a New Character (Step-by-Step for Designers)

1.  **Create DataAsset**: In the Unreal Editor Content Browser, right-click -> Miscellaneous -> Data Asset -> Select `CharacterData` as the base class.
2.  **Naming Convention**: Name it clearly, e.g., `CD_Scavenger_Heavy`.
3.  **Populate Core Fields**: Open the asset. Assign the Skeletal Mesh (remember: it's a soft reference, so it won't load the mesh immediately). Set the Base Stats and define the AI Personality.
4.  **Assign Abilities**: Add default GAS abilities to the `InnateAbilities` array (e.g., `GA_Shoot`, `GA_Reload`).
5.  **Asset Manager Registration**: Ensure the folder containing this new asset is scanned by the `PrimaryAssetTypesToScan` array located in `Project Settings -> Game -> Asset Manager`.
6.  **Spawn and Test**: The character is now fully integrated. They will appear in debug menus, can be spawned via the AssetManager, and can be used in scenario generation.

## 14. JSON Schema Reference for Modders

To facilitate external tool generation and modding, core character data maps cleanly to this JSON structure:
```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "Character Definition",
  "type": "object",
  "properties": {
    "CharacterID": { 
      "type": "string",
      "description": "Unique identifier for the character."
    },
    "DisplayName": { 
      "type": "string" 
    },
    "BaseStats": {
      "type": "object",
      "properties": {
        "Strength": { "type": "number", "default": 10.0 },
        "Speed": { "type": "number", "default": 10.0 },
        "Durability": { "type": "number", "default": 10.0 },
        "Energy": { "type": "number", "default": 100.0 }
      }
    },
    "Personality": {
      "type": "object",
      "properties": {
        "Aggression": { "type": "number", "minimum": 0, "maximum": 1 },
        "Caution": { "type": "number", "minimum": 0, "maximum": 1 }
      }
    }
  },
  "required": ["CharacterID", "DisplayName", "BaseStats"]
}
```

## 15. The Evolution of Attributes over Timelines

A key feature of the character system is how attributes permanently alter based on timeline jumps. 
*   **Resonance Mechanic**: If a player heavily invests in the `Strength` attribute across 5 consecutive timelines, their *Base* Strength attribute in the 6th timeline (even on a fresh character) might start slightly higher, simulating a meta-evolution or "Resonance" across the multiverse.
*   **Attribute Decay**: Conversely, relying entirely on technological enhancements (which don't modify base stats) might lead to minor biological atrophy over multiple timelines, slightly reducing base `Durability` if not exercised.

## 16. Detailed Breakdown of the Relationship Component

The `URelationshipComponent` is the backbone of the game's social simulation.

### 16.1 Faction Hierarchies
Factions are not monolithic. The system supports nested factions:
*   `Faction.Corporate.Security`
*   `Faction.Corporate.Research`
Damaging a Security officer might decrease standing with `Faction.Corporate` by -5, but decrease standing with `Faction.Corporate.Security` by -20, while having no effect on `Faction.Corporate.Research`.

### 16.2 Action Events & Score Modifiers
Every action has an associated relationship modifier:
*   **Healing Ally**: +10 to target's faction.
*   **Stealing Resource**: -15 to owner's faction.
*   **Lethal Takedown**: -50 to target's faction.
*   **Non-Lethal Takedown**: -10 to target's faction (shows restraint).

## 17. Save Data Serialization Rules

Because characters can be immensely complex after 100 timeline jumps, save data serialization must be extremely efficient.

### 17.1 Delta Saving
We only save the *deltas* (changes) from the base `UCharacterData`. 
If a character starts with 10 Strength and ends the timeline with 15 Strength due to 5 `GameplayEffects`, the save file does not store "Strength = 15". It stores the list of the 5 applied `GameplayEffects` (using their unique IDs). 

### 17.2 Why Delta Saving?
This ensures that if a patch changes the base strength of the archetype from 10 to 12, the player's saved character will correctly calculate their new strength as 17 (12 + 5), rather than being hardcoded to the outdated 15. This is critical for live-service balance updates and preventing saved games from breaking when base data assets are tweaked.

