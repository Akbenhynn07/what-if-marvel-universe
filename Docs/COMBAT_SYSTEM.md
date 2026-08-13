# W.H.A.T. I.F. Combat System Documentation

## 1. Design Philosophy

The Combat System in W.H.A.T. I.F. is built upon three core pillars:
1. **Deterministic Execution:** Given the same initial state, combat parameters, and random seed, the combat simulator must produce the exact same outcome every single time.
2. **Reproducible Simulation:** Because the simulation relies on deterministic execution, it can be played back, rewound, or branched at any arbitrary point in the timeline.
3. **Seed-Based RNG:** All random number generation within combat operations derives from a centralized seeded PRNG. No system functions (like `FMath::Rand()`) should be used directly.

This philosophy ensures that as characters engage in combat scenarios, the results can be reliably serialized and fed into the Narrative Engine without fear of desynchronization between state and story.

---

## 2. ICombatSimulator Interface Contract

The `ICombatSimulator` interface defines the core contract for resolving combat encounters. Any system wishing to execute combat must do so through this interface.

```cpp
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WhatIfCombatTypes.h"
#include "ICombatSimulator.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UCombatSimulator : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface for simulating combat encounters deterministically.
 */
class WHATIF_API ICombatSimulator
{
    GENERATED_BODY()

public:
    /**
     * Executes a combat encounter between characters based on the provided setup.
     * 
     * @param Setup The configuration and initial state for the combat encounter.
     * @return The detailed result of the combat, including logs and state changes.
     */
    virtual FWhatIfCombatResult SimulateCombat(const FWhatIfCombatSetup& Setup) = 0;

    /**
     * Validates a combat setup before simulation begins.
     * 
     * @param Setup The setup to validate.
     * @return True if valid, false otherwise.
     */
    virtual bool ValidateSetup(const FWhatIfCombatSetup& Setup) const = 0;

    /**
     * Previews the potential outcome probabilities of an attack without committing state.
     */
    virtual FWhatIfCombatPreview PreviewAttack(const FWhatIfCombatSetup& Setup) const = 0;
};
```

---

## 3. Data Structures

### 3.1 `FWhatIfCombatSetup`

This struct contains everything needed to initialize a combat simulation deterministically.

```cpp
USTRUCT(BlueprintType)
struct FWhatIfCombatSetup
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Setup")
    int32 RandomSeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Setup")
    TArray<FWhatIfCombatantState> Attackers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Setup")
    TArray<FWhatIfCombatantState> Defenders;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Setup")
    FWhatIfEnvironmentData EnvironmentData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Setup")
    int32 MaxTurns = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Setup")
    float TimeDelta = 0.1f;
};
```

### 3.2 `FWhatIfCombatResult`

Contains the output of a simulation pass.

```cpp
USTRUCT(BlueprintType)
struct FWhatIfCombatResult
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat Result")
    EWhatIfCombatOutcome Outcome;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat Result")
    TArray<FWhatIfCombatantState> FinalState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat Result")
    TArray<FWhatIfCombatLogEntry> CombatLog;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat Result")
    float TotalDurationSeconds;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat Result")
    int32 TotalTurnsElapsed;
};
```

---

## 4. Combat Resolution Algorithm (High-Level)

The simulation runs in a locked-step turn or tick-based loop.

1. **Initialization:**
   - Seed the RNG using `Setup.RandomSeed`.
   - Copy initial combatant states.
   - Apply pre-combat buffs/debuffs from the environment.
2. **Tick/Turn Loop:**
   - Determine initiative (who acts this tick).
   - Resolve actions (Move, Attack, Use Ability).
   - Calculate hit chances.
   - If hit: Calculate damage, apply damage, calculate criticals.
   - Deduct energy/resources.
   - Apply status effects (DoT, stuns).
   - Log every distinct action into `CombatLog`.
3. **Termination Check:**
   - Check if all defenders are defeated, or all attackers are defeated.
   - Check if `MaxTurns` is reached.
4. **Finalization:**
   - Compile `FWhatIfCombatResult`.
   - Return result.

---

## 5. Damage Formula

Damage is calculated using a multi-step formula to account for base power, scaling, mitigation, and variance.

```
BaseDamage = (AbilityBasePower + (StatScalar * ScalingFactor))
Mitigation = (Armor / (Armor + 100.0)) * 0.5  // Max 50% physical mitigation from armor
Resistance = ElementalResistance / 100.0      // Direct percentage mitigation

DamageAfterMitigation = BaseDamage * (1.0 - Mitigation) * (1.0 - Resistance)

VarianceFactor = SeededRandomRange(0.9, 1.1)
FinalDamage = DamageAfterMitigation * VarianceFactor * CriticalMultiplier
```

**Note:** The VarianceFactor uses the seeded RNG to ensure reproducible damage spreads.

---

## 6. Accuracy and Hit Chance Model

Hit chance is determined by comparing the attacker's Accuracy rating against the defender's Evasion rating.

```
HitChancePercentage = BaseAbilityAccuracy + (AttackerAccuracy - DefenderEvasion) * 0.5
```
HitChancePercentage is clamped between 5.0% and 95.0%.
The engine then rolls a seeded integer between 1 and 100. If the roll is less than or equal to the HitChancePercentage, the attack lands.

---

## 7. Energy/Resource Consumption

Abilities cost resources (Stamina, Mana, Kinetic Charge, etc.).
- A combatant cannot execute an ability if they lack the resources.
- Resources regenerate passively per tick based on the character's `ResourceRegenRate`.
- Exhaustion mechanics: If Stamina drops below 0, the character enters a "Fatigued" state, lowering Evasion by 20% for 3 turns.

---

## 8. Range and Mobility Interaction

Combat accounts for 3D positioning (or simulated distances in abstract modes).
- **Melee:** Requires distance < 200 Unreal Units.
- **Ranged:** Requires line of sight and distance < MaximumAbilityRange.
- **Mobility:** Combatants expend movement points or time to close gaps. A combatant with high mobility will spend fewer ticks closing distance, granting them first-strike advantages against slower ranged combatants.

---

## 9. Environmental Modifiers

The `FWhatIfEnvironmentData` struct provides modifiers that affect the combat formula globally.

*Example Modifiers:*
- **High Gravity:** All mobility costs doubled.
- **Toxic Atmosphere:** All combatants suffer 1% Max HP damage per turn (bypasses armor).
- **Dense Fog:** Maximum line of sight reduced by 50%. Ranged accuracy drops by 15%.

---

## 10. Seeded RNG

The core of the reproducibility is the `FWhatIfSeededRNG` utility.
It wraps `FRandomStream`.
Every time combat starts, a new `FRandomStream` is instantiated with the `Setup.RandomSeed`. Every check (hit, crit, variance) must call `FWhatIfSeededRNG::GetFloat()` or `FWhatIfSeededRNG::GetInt()`.

---

## 11. Cooldown System (GAS-Backed)

The game utilizes Unreal's Gameplay Ability System (GAS) under the hood for real-time representations. For simulation:
- Abilities apply standard `UGameplayEffect` cooldowns.
- The simulator tracks elapsed simulation time.
- An ability is locked out if `LastUsedTime + CooldownTime > CurrentSimTime`.

---

## 12. Combat Logging for Narrative Engine Input

The simulation produces a sequence of `FWhatIfCombatLogEntry` structs. These are NOT simple strings. They are structured data meant to be parsed by the LLM later.

```cpp
struct FWhatIfCombatLogEntry {
    FName ActionID;          // e.g., "Attack_HeavyStrike"
    FName ActorID;           // e.g., "Char_A"
    FName TargetID;          // e.g., "Char_B"
    float DamageDealt;
    bool bWasCritical;
    bool bWasEvaded;
    FVector FinalPosition;
};
```
The Narrative System takes these logs and converts them into prose (e.g., "Character A lunged forward, landing a devastating blow on Character B...").

---

## 13. What the LLM is NOT Allowed to Influence in Combat

To maintain mechanical integrity, the LLM boundaries are strictly enforced:
- **No Stat Changes:** The LLM cannot decide a character suddenly has more health or does more damage.
- **No Outcome Overrides:** If the deterministic simulator says Character A dies, the LLM cannot write that Character A survived.
- **No Spontaneous Abilities:** The LLM cannot invent new combat abilities mid-fight. It can only describe abilities that were executed in the combat log.
- **Role:** The LLM is strictly a *narrator* of the combat results, not a referee or participant.
