# W.H.A.T. I.F. - Character System Document

## 1. Character Data Model Hierarchy

The character system is built on a robust, data-driven hierarchy utilizing Unreal Engine's architecture.

### Base Classes
1. **`UCharacterData` (UPrimaryDataAsset)**: The immutable blueprint defining a character archetype.
2. **`AWhatIfCharacter` (ACharacter)**: The in-world actor instance.
3. **`UCharacterComponent` (UActorComponent)**: Manages the runtime state, linking the actor to the GAS system and dynamic data.
4. **`UWhatIfAttributeSet` (UAttributeSet)**: The GAS-specific container for all numerical stats.

### Field-by-Field Breakdown of `UCharacterData`
* `FName CharacterID`: Unique identifier.
* `FText DisplayName`: Localized character name.
* `FText Description`: Lore summary.
* `TSoftObjectPtr<USkeletalMesh> BaseMesh`: The visual representation.
* `TSubclassOf<UGameplayAbility> InnateAbilities`: Default abilities.
* `FCharacterBaseStats InitialStats`: The starting attributes.
* `FPersonalityData BasePersonality`: The AI archetype.
* `TArray<FCharacterConstraint> Constraints`: Rules for what modifications can be applied.

## 2. UPrimaryDataAsset Pattern

We utilize `UPrimaryDataAsset` for all character definitions, transformations, and items.
* **Why**: It allows designers to create and balance characters entirely in the editor without touching C++.
* **Memory Management**: These assets are not loaded into memory until explicitly requested by the AssetManager.

## 3. AssetManager Integration

The `UWhatIfAssetManager` (derived from `UAssetManager`) handles the lifecycle of Character Data.
* **Async Loading**: When a timeline is loaded or a character is spawned, the AssetManager asynchronously loads the required `UCharacterData` via its PrimaryAssetId to prevent hitches.
* **Registry**: It maintains a registry of all available characters, mutations, and items for easy querying (e.g., "Get all valid mutations for this character class").

## 4. TSoftObjectPtr Usage Rules

* **Hard References are Forbidden**: `UCharacterData` must NEVER use hard pointers (e.g., `UTexture2D*` or `USkeletalMesh*`) for bulky assets.
* **Rule**: Always use `TSoftObjectPtr<T>`.
* **Reasoning**: If Character A has a hard reference to a 50MB texture, loading Character A loads that texture. In a multiverse game with thousands of variants, this will instantly cause out-of-memory crashes. Assets are only resolved to hard pointers when the character is actively spawned in the simulation.

## 5. Full Attribute List (`UWhatIfAttributeSet`)

* **Strength**: Modifies melee damage and physical carrying capacity/physics interactions.
* **Speed**: Base movement velocity and jump height.
* **Durability**: Innate damage reduction (Armor) against physical kinetic attacks.
* **Energy**: Current resource for casting abilities/powers.
* **MaxEnergy**: The upper limit of the Energy pool.
* **Intelligence**: Modifies the effectiveness of tech-based abilities and hacking speeds.
* **DefenseRating**: General modifier against all incoming damage types.
* **AttackRating**: General modifier to all outgoing damage types.
* **MobilityRating**: Affects dodge chance and the speed of evasion animations.
* **ReactionTime**: Reduces the delay before AI reacts; increases parry/counter windows for players.
* **Accuracy**: Reduces weapon spread and increases critical hit chance.
* **RecoveryRate**: Passive regeneration of Health and Energy per tick.
* **TacticalCapability**: Increases utility of squad commands and expands AI perception radius.
* **ResourceEfficiency**: Reduces the Energy/Ammo cost of all abilities.

## 6. Modifier Stack Explanation

Character stats are never permanently overwritten. The system uses a Modifier Stack (handled by GAS GameplayEffects).
* **Base Value**: Defined by `UCharacterData`.
* **Current Value**: The calculated total.
* **Modifiers**: When a character gains a "Cybernetic Arm", it applies an infinite-duration GameplayEffect that adds +20 Strength. Removing the arm removes the effect, reverting the strength flawlessly.

## 7. GAS AttributeSet Design

We use a monolithic `UWhatIfAttributeSet` to centralize calculations.
* Every attribute is defined as an `FGameplayAttributeData`.
* `PreAttributeChange` is overridden to enforce clamping (e.g., preventing Health from dropping below 0 before damage is finalized).
* `PostGameplayEffectExecute` is used to handle systemic reactions (e.g., if Health hits 0, trigger the Death sequence).

## 8. ATTRIBUTE_ACCESSORS Macro

To reduce boilerplate, we use the standard GAS macro:
```cpp
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
```
This is placed under every attribute in the header file to automatically generate getters, setters, and init functions required by GAS.

## 9. Relationship System

Relationships are tracked via a dedicated component (`URelationshipComponent`).
* Uses a map of `FName` (Faction/Character ID) to `float` (-100 to 100).
* **Dynamic Adjustment**: Actions in the Simulation State (healing an ally, damaging a neutral) fire events that this component listens to, adjusting the scores dynamically.

## 10. AI Personality Struct

```cpp
USTRUCT(BlueprintType)
struct FPersonalityData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere) float Aggression; // 0.0 to 1.0
    UPROPERTY(EditAnywhere) float Caution;
    UPROPERTY(EditAnywhere) float Loyalty;
    UPROPERTY(EditAnywhere) float Curiosity;
};
```
This struct is passed to the AI Controller upon possession to initialize the Utility AI weights.

## 11. Character Identity vs. Character Instance

* **Identity**: The meta-concept of the character across the multiverse (e.g., "Subject Delta"). Stored in the save data, tracking branches.
* **Instance**: The specific physical actor currently running in the engine in a specific timeline branch.
* When jumping timelines, the Identity is preserved, but the Instance is destroyed and reconstructed based on the data in that specific branch.

## 12. Constraints and Incompatibilities System

Transformations use a tag-based constraint system (`FGameplayTagContainer`).
* A "Biological Mutation" might apply the tag `State.Mutated`.
* A "Cybernetic Skeleton" might have a requirement tag `State.PureBio` and an incompatibility tag `State.Mutated`.
* The system queries these tags before allowing a transformation in the Character Laboratory, preventing logical conflicts.

## 13. How to Add a New Character (Step-by-Step)

1. **Create DataAsset**: In the Editor, right-click -> Data Asset -> Select `CharacterData`.
2. **Name it**: e.g., `CD_Scavenger`.
3. **Populate Fields**: Assign the Skeletal Mesh (soft ref), set base stats, and define the personality.
4. **Assign Abilities**: Add default GAS abilities to the `InnateAbilities` array.
5. **Register**: Ensure the folder containing the asset is scanned by the PrimaryAssetTypesToScan array in `Project Settings -> Asset Manager`.
6. **Spawn**: The character is now available in the UI and can be spawned via the AssetManager.

## 14. JSON Schema Reference

For modders or external tool generation, character data maps to this JSON structure:
```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "type": "object",
  "properties": {
    "CharacterID": { "type": "string" },
    "DisplayName": { "type": "string" },
    "BaseStats": {
      "type": "object",
      "properties": {
        "Strength": { "type": "number" },
        "Speed": { "type": "number" }
      }
    },
    "Personality": {
      "type": "object",
      "properties": {
        "Aggression": { "type": "number" }
      }
    }
  }
}
```
