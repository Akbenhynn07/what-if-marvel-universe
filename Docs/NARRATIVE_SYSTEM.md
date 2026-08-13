# W.H.A.T. I.F. Narrative System Documentation

## 1. Role of Narrative

The Narrative System is the bridge between raw, deterministic simulation data and human-readable, engaging storytelling. Its primary role is to ingest structured arrays of logs, state changes, and events, and convert them into rich prose or dialogue using Large Language Models (LLMs) via external API integrations.

It does **not** make decisions that affect gameplay mechanics. It only describes what has already happened in the simulation engine.

---

## 2. INarrativeProvider Interface

To remain agnostic to the specific LLM backend (OpenAI, Anthropic, local models, etc.), the engine uses an interface for requesting narratives.

```cpp
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WhatIfNarrativeTypes.h"
#include "INarrativeProvider.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UNarrativeProvider : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface for generating story text from simulation data.
 */
class WHATIF_API INarrativeProvider
{
    GENERATED_BODY()

public:
    /**
     * Initiates an asynchronous request to generate narrative text.
     * 
     * @param Request The structured data payload describing the events.
     * @param OnComplete Delegate fired when text is generated.
     */
    virtual void RequestNarrativeAsync(const FWhatIfNarrativeRequest& Request, const FOnNarrativeGenerated& OnComplete) = 0;

    /**
     * Validates if the provider is currently online/available.
     */
    virtual bool IsAvailable() const = 0;
};
```

---

## 3. FWhatIfSimulationResult Struct

This struct is the payload sent from the Scenario or Combat systems into the Narrative System.

```cpp
USTRUCT(BlueprintType)
struct FWhatIfSimulationResult
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Narrative")
    FString ScenarioID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Narrative")
    TArray<FWhatIfCombatLogEntry> CombatLogs;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Narrative")
    TArray<FString> KeyEvents;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Narrative")
    TMap<FName, FString> CharacterProfiles; // Context on who is involved.

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Narrative")
    FString EnvironmentDescription;
};
```

---

## 4. Prompt Engineering Strategy

The system constructs prompts dynamically by assembling several components.

### 4.1 System Prompt
Defines the persona and rules for the LLM.
> "You are the narrator of a dark fantasy simulation. Describe the events provided in the data strictly as they occurred. Do not invent new outcomes, resurrect dead characters, or change the results of the combat. Use evocative, visceral language."

### 4.2 User Prompt (Data Injection)
Converts the `FWhatIfSimulationResult` into a JSON or formatted text block.
> "ENVIRONMENT: A decaying ruin.
> CHARACTERS: Knight (HP: 10%), Bandit (HP: 0 - Dead).
> EVENTS: Knight used 'Heavy Slash' for 45 damage on Bandit. Bandit is defeated."

### 4.3 Output Format
Strictly requests the output in a JSON format so the engine can parse dialogue vs. narration.
> "Format your output as a JSON object: { \"narration\": \"string\", \"dialogue_lines\": [ { \"speaker\": \"string\", \"text\": \"string\" } ] }"

---

## 5. Output Validation

Because LLMs can hallucinate, the output must be validated before being presented to the user.
- **JSON Parsing:** Ensure the output is valid JSON matching the requested schema.
- **Constraint Checking:** The system runs a quick keyword check. If the LLM output says a character is "alive" but the simulation data says they are "Dead", the validation fails.
- **Retry Logic:** If validation fails, the provider automatically issues a retry request with an appended correction prompt (e.g., "Error: Do not say the Bandit survived. Retry."). Max retries is typically 2.

---

## 6. Async Narrative Request Pattern

Narrative generation requires network requests and takes time (0.5s - 5s). 
The architecture uses asynchronous delegate callbacks.
1. Simulation finishes. UI shows a "Writing History..." loading state.
2. `RequestNarrativeAsync` is called.
3. Thread is yielded. The UI remains responsive.
4. HTTP response returns. Validation runs on a background thread.
5. `OnComplete` delegate fires on the Game Thread. UI is updated with the generated text.

---

## 7. Narrative Types

Depending on the context, different prompt templates are used:
- **Combat Recap:** High-action, focus on the blows traded and the physical toll.
- **Travel Log:** Atmospheric, focus on the environment and passage of time.
- **Social Encounter:** Focus on body language, tone of voice, and tension.
- **Death/Ending:** Somber, conclusive, wrapping up the timeline branch's legacy.

---

## 8. Dialogue System Overview

When the JSON output includes `dialogue_lines`, the Narrative System pushes these to the UI Dialogue Subsystem.
- Speakers are matched to in-game 3D models or 2D portraits.
- The UI can present these sequentially with "Next" button prompts, rather than dumping a wall of text.
- If text-to-speech (TTS) is enabled, the dialogue lines are sent to the audio provider asynchronously.

---

## 9. Fallback Narrative

If the player is offline, or the API is unreachable, or the API key is out of quota, the system must gracefully degrade.
- The Fallback Provider is a local, templated text generator.
- It uses MAD-LIB style string replacement.
- Example: `[CharacterA] defeated [CharacterB] in [Environment].`
- It is dry and lacks flair, but ensures the game is still playable.

---

## 10. Provider Abstraction

Developers can swap providers by implementing `UNarrativeProvider`.
- `UOpenAINarrativeProvider`: Implements REST API calls to OpenAI endpoints.
- `ULocalNarrativeProvider`: Interfaces with a local executable running LLaMA.
- `UMockNarrativeProvider`: Used for unit testing. Immediately returns hardcoded text without network calls.

---

## 11. Rate Limiting and Caching Strategy

To prevent API spam and reduce costs:
- **Hash Caching:** The serialized `FWhatIfSimulationResult` is hashed (MD5/SHA). The hash is checked against a local SQLite database. If this exact simulation result has been generated before, the cached narrative is returned instantly.
- **Rate Limiting:** A token bucket algorithm prevents the system from making more than X requests per minute. If exceeded, the system falls back to the Fallback Narrative temporarily.

---

## 12. Context Window Management

LLMs have finite context windows. To narrate a late-game scenario without losing the thread of the overall story, the system employs Summarization and Sliding Windows.

### 12.1 The Summarizer Node
After every 5 significant Timeline Nodes, the Narrative System executes a background summarization request.
> "Summarize the following 5 events into a concise 3-sentence paragraph."
This summary is then cached. When requesting a new narrative for the 6th node, the prompt includes the summary of the first 5 nodes, rather than the raw data of all 5 nodes, saving tokens and keeping the LLM focused.

### 12.2 Sliding Window
Only the immediate preceding node's full state is passed into the prompt as "Immediate Context".
```json
{
  "ChronicleSummary": "The Knight entered the dungeon and slew the guards.",
  "ImmediateContext": "Knight is currently standing in the armory. Health: 50%.",
  "NewEvent": "Knight opens the cursed chest."
}
```

---

## 13. Localization and Tone Overrides

Because the text generation is dynamic, localization (L10N) is fundamentally different from traditional games. 
Instead of translating pre-written strings, the System Prompt itself is modified.

- `SystemPrompt_EN`: "You are a narrator..."
- `SystemPrompt_ES`: "Eres un narrador..."
- `SystemPrompt_FR`: "Vous êtes un narrateur..."

Additionally, the Narrative System supports **Tone Profiles**. 
By swapping the Tone Profile in the settings menu, the exact same deterministic combat log can be narrated wildly differently.
- **Tone_Grimdark:** "The blade tore through flesh, leaving a ruin of bone and blood in its wake."
- **Tone_Comedic:** "The bandit zigged when he should have zagged, catching a sword with his face in a spectacular display of poor decision-making."
- **Tone_Clinical:** "Combatant A struck Combatant B for lethal damage."

---

## 14. Error Handling and Safe Mode

When relying on cloud-based LLM APIs, network failures are inevitable. The `UNarrativeManager` implements a robust state machine for fault tolerance.

1. **State_Active:** API is responsive.
2. **State_Degraded:** API is timing out or rate-limited. The system increases the timeout duration and relies on cache heavily.
3. **State_Offline:** The system swaps to `ULocalNarrativeProvider` or the Fallback MAD-LIB system.

If the returned text contains flagged content (e.g., if the LLM hallucinates inappropriate material), a client-side regex filter intercepts the response before it reaches the UI, replacing the output with a generic summary of the event (e.g., "[A violent encounter occurred, resulting in the defeat of the Bandit]").

---

## 15. The "Inner Monologue" Feature

Beyond standard narration, the Narrative System can generate "Inner Monologue" lines for the active protagonist.
When a Timeline Node is reached where a difficult choice must be made, the UI queries the `INarrativeProvider` with an `EMode::InnerMonologue` request.
The prompt asks the LLM to hypothesize what the character is thinking, based on their `CharacterProfile` and current stress level. This output is rendered on-screen in italics, giving the player deep psychological insight into their character before they make their next branch decision.
