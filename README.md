# W.H.A.T. I.F.
## What Happens After Transformation: Infinite Futures

> *"Every choice, every transformation, births a new reality."*

[![UE5.4](https://img.shields.io/badge/Unreal%20Engine-5.4-blue)](https://www.unrealengine.com/)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-orange)](https://isocpp.org/)
[![GAS](https://img.shields.io/badge/Gameplay%20Ability%20System-Enabled-green)]()
[![Phase](https://img.shields.io/badge/Phase-0%20Complete-brightgreen)]()

---

## Vision Statement

**W.H.A.T. I.F.** is a large-scale AI-powered **character transformation and alternate timeline simulation engine** built on Unreal Engine 5.4. It empowers creators and players to take any character, subject them to deep transformation — swapping armor, powers, weapons, energy sources, AI behavior, and tactical preferences — and then rigorously simulate the cascading consequences of those changes across a branching tree of infinite possible futures.

This is not a story generator. It is a **simulation engine** that generates stories.

---

## What Makes It Different

Most "What If?" AI tools ask an LLM to write alternate history fiction. W.H.A.T. I.F. does something fundamentally different:

| Feature | Traditional AI Story Tool | W.H.A.T. I.F. |
|---------|--------------------------|----------------|
| Character stats | LLM decides arbitrarily | Deterministic GAS AttributeSet |
| Combat outcomes | LLM narrates freely | Seeded deterministic simulator |
| Timeline branching | Linear story rewrites | Persistent state graph, replayable |
| LLM role | Full author | Narrative translator only |
| Reproducibility | None | Full — same seed = same outcome |
| Mod support | Not applicable | Game Feature Plugin architecture |

The **deterministic simulation core** computes the truth. The **LLM narrative engine** translates that truth into compelling story.

---

## Core Gameplay Loop

```
CREATE character
     ↓
CUSTOMIZE (swap armor, powers, weapons, AI behavior)
     ↓
SIMULATE (deterministic combat + event resolution)
     ↓
PLAY (observe or participate)
     ↓
DECIDE (intervene at key moments)
     ↓
CONSEQUENCE (attributes, relationships, world state updated)
     ↓
BRANCH (new timeline node created)
     ↓
EXPLORE (navigate the alternate future)
```

---

## Technology Stack

| Component | Technology | Purpose |
|-----------|-----------|---------|
| Game Engine | Unreal Engine 5.4 | Rendering, physics, asset management, gameplay framework |
| Primary Language | C++17 | All simulation, data model, interface, and performance-critical logic |
| Secondary Language | Blueprints | Designer-facing config, animation logic, UI binding, rapid prototyping |
| Attribute System | Gameplay Ability System (GAS) | Networked attributes, effects, modifiers, ability management |
| Character Architecture | Modular Game Framework | Runtime component injection without base class modification |
| Content Extensibility | Game Feature Plugins | Swappable character packs, scenario packs, modification packs |
| Data Model | UPrimaryDataAsset | Async-loadable character and modification definitions |
| Input | Enhanced Input System | Context-aware input mapping |
| UI Data Binding | MVVM Plugin | Clean separation of UI state from game state |
| Polymorphic Data | StructUtils / FInstancedStruct | Heterogeneous data without deep inheritance |
| Version Control | Git + Git LFS | Source code (Git) + binary assets (LFS) |
| LLM Integration | INarrativeProvider (abstracted) | Swappable: OpenAI, Gemini, Claude, local model |

---

## Quick Start

### Prerequisites

- **OS**: Windows 10/11 or macOS 13+
- **Unreal Engine 5.4.x** — installed via Epic Games Launcher
- **IDE**: Visual Studio 2022 with C++ Game Development workload (Windows) OR Xcode 15+ (macOS)
- **Git** with **Git LFS** installed (`git lfs install`)
- **Python 3.8+** (for running schema tests, no UE5 required)

### Clone and Setup

```bash
# 1. Clone
git clone https://github.com/your-username/WHAT-IF.git
cd "WHAT-IF"

# 2. Initialize LFS (pulls binary assets)
git lfs install
git lfs pull

# 3. Generate project files (choose your platform)
# Windows: right-click WhatIf.uproject → "Generate Visual Studio project files"
# macOS:   right-click WhatIf.uproject → Services → Generate Xcode Project
# Linux:   ./GenerateProjectFiles.sh

# 4. Build (Development Editor configuration)
# Open WhatIf.sln (Windows) or WhatIf.xcworkspace (macOS) and build

# 5. Launch
# Open WhatIf.uproject in UE5 Editor
```

### Run Schema Tests (no UE5 required)

```bash
cd Tests/Unit
pip3 install jsonschema
python3 test_character_schema.py
# Expected: 9 passed, 0 failed
```

---

## Project Structure

```
WhatIf/
├── WhatIf.uproject             ← UE5 5.4 project definition
├── .gitignore / .gitattributes ← Git + LFS configuration
│
├── Source/
│   ├── WhatIf/                 ← Runtime module (C++)
│   │   ├── Public/             ← Exported headers (interfaces, data models)
│   │   │   ├── Core/           ← Shared types and base simulation interface
│   │   │   ├── Character/      ← UWhatIfCharacterData (UPrimaryDataAsset)
│   │   │   ├── Attributes/     ← UWhatIfAttributeSet (GAS)
│   │   │   ├── Abilities/      ← UWhatIfAbilityData
│   │   │   ├── Equipment/      ← UWhatIfEquipmentData
│   │   │   ├── Transformation/ ← UWhatIfModificationData + ITransformationSystem
│   │   │   ├── Combat/         ← ICombatSimulator interface
│   │   │   ├── AI/             ← FWhatIfAgentState + FWhatIfAgentAction
│   │   │   ├── Decision/       ← IDecisionEngine interface
│   │   │   ├── Timeline/       ← FWhatIfTimelineNode + UWhatIfTimelineGraph
│   │   │   ├── Narrative/      ← INarrativeProvider interface (LLM abstraction)
│   │   │   ├── Scenario/       ← UWhatIfScenarioData
│   │   │   └── World/          ← UWhatIfEnvironmentData
│   │   └── Private/            ← Implementation files
│   └── WhatIfEditor/           ← Editor-only module (custom tools, validators)
│
├── Content/                    ← UE5 assets (managed by Git LFS)
├── Config/                     ← Engine/Game/Input .ini files
├── Plugins/                    ← Game Feature Plugin home
├── Docs/                       ← All design and architecture documentation
│   └── Schemas/                ← JSON Schema definitions for data validation
└── Tests/
    ├── Unit/                   ← Python schema tests (run without UE5)
    └── Integration/            ← UE5 Automation Tests (require engine)
```

---

## Documentation

| Document | Purpose |
|----------|---------|
| [ARCHITECTURE.md](Docs/ARCHITECTURE.md) | Full system map, layered architecture, data flow |
| [ROADMAP.md](Docs/ROADMAP.md) | 15-phase development plan with success criteria |
| [GAME_DESIGN.md](Docs/GAME_DESIGN.md) | Gameplay loop, UI screens, design pillars |
| [AI_ARCHITECTURE.md](Docs/AI_ARCHITECTURE.md) | AI layer design, LLM role and constraints |
| [CHARACTER_SYSTEM.md](Docs/CHARACTER_SYSTEM.md) | Character data model, GAS attributes, modifier stack |
| [COMBAT_SYSTEM.md](Docs/COMBAT_SYSTEM.md) | Deterministic combat design, ICombatSimulator contract |
| [TIMELINE_SYSTEM.md](Docs/TIMELINE_SYSTEM.md) | State graph design, branch creation, replay |
| [NARRATIVE_SYSTEM.md](Docs/NARRATIVE_SYSTEM.md) | LLM integration, INarrativeProvider, guardrails |
| [SCENARIO_SYSTEM.md](Docs/SCENARIO_SYSTEM.md) | Scenario data model, event system, modularity |
| [CODING_STANDARDS.md](Docs/CODING_STANDARDS.md) | UE5 C++ conventions, interface rules, Blueprint guidelines |
| [CONTRIBUTING.md](Docs/CONTRIBUTING.md) | Git workflow, branch strategy, ADR process |
| [DECISIONS.md](Docs/DECISIONS.md) | Architectural Decision Records (ADR-001 through ADR-012) |

---

## Development Phases

| Phase | Name | Scope | Status |
|-------|------|-------|--------|
| 0 | Architecture & Documentation | S | ✅ Complete |
| 1 | Character Data Model | M | ⏳ Next |
| 2 | Transformation System | L | — |
| 3 | Attribute & Modifier System | M | — |
| 4 | Basic Combat Simulator | L | — |
| 5 | AI Agent Decision System | M | — |
| 6 | Scenario Framework | M | — |
| 7 | Timeline Branching | XL | — |
| 8 | Narrative Generation | L | — |
| 9 | UE5 Integration | L | — |
| 10 | Character Customization UI | M | — |
| 11 | Playable Combat Prototype | XL | — |
| 12 | Visual Polish | L | — |
| 13 | Advanced AI | L | — |
| 14 | Multiplayer / Community | XL | Future |

See [ROADMAP.md](Docs/ROADMAP.md) for full details on deliverables, dependencies, and success criteria.

---

## Design Philosophy

**Never optimize for finishing quickly. Optimize for:**

> maintainability + modularity + correctness + scalability

Before implementing any feature, ask:
> *"Will this architecture still work when we have thousands of characters, abilities, scenarios, and timeline branches?"*

---

## Contributing

Please read [CONTRIBUTING.md](Docs/CONTRIBUTING.md) for the Git workflow, branch naming strategy, PR checklist, and the ADR process for recording architectural decisions.

---

## License

Copyright 2026 W.H.A.T. I.F. Project. All Rights Reserved.  
*(License TBD — to be specified before public release.)*
