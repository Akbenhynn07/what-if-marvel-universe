# W.H.A.T. I.F. (What Happens After Transformation: Infinite Futures)

> "Every choice, every transformation, births a new reality."

## Vision Statement
W.H.A.T. I.F. is an AI-powered character transformation and alternate timeline simulation engine built on Unreal Engine 5.4. It empowers creators and players to define base characters, subject them to transformative events, and rigorously simulate the cascading, systemic effects of those changes across infinite possible futures. 

## What Makes It Unique
Unlike traditional text-based AI story generators, W.H.A.T. I.F. relies on a **Deterministic Simulation Core** paired with an **AI Narrative Interpreter**.
1. **Structured Simulation**: Characters have discrete stats, abilities, and states managed by Unreal's Gameplay Ability System (GAS). When a transformation occurs (e.g., a character acquires a new power source), the engine recalculates their capabilities systemically.
2. **AI Narrative Translation**: The LLM does not dictate the character's stats. Instead, the simulation engine provides the resulting data state to the LLM, which translates these hard numbers into compelling, context-aware narrative beats, dialogues, and scenario descriptions.
3. **True Branching Timelines**: Because the core is a simulation, every decision point creates a persistent, reproducible world state that can branch infinitely.

## Technology Stack
- **Engine**: Unreal Engine 5.4
- **Language**: C++ (Core Systems), Blueprint (Content definition & rapid prototyping)
- **Frameworks**: 
  - Gameplay Ability System (GAS) for attributes, modifiers, and abilities.
  - Modular Gameplay & Game Feature Plugins for extending systems without modifying the core.
  - Common UI for cross-platform interface development.
- **AI Integration**: Custom HTTP REST client for communicating with external LLM APIs (e.g., OpenAI, Anthropic, or local models).

## Quick Start Guide

### Prerequisites
- Windows 10/11 or macOS 13+
- Unreal Engine 5.4.x installed via Epic Games Launcher
- Visual Studio 2022 (Windows) or Xcode 15 (macOS) with C++ game development workloads
- Git LFS installed

### Installation & Setup
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/WHAT-IF.git
   cd WHAT-IF
   ```
2. **Initialize Git LFS**:
   ```bash
   git lfs install
   git lfs pull
   ```
3. **Generate Project Files**:
   - **Windows**: Right-click `WhatIf.uproject` and select "Generate Visual Studio project files".
   - **macOS**: Right-click `WhatIf.uproject` -> Services -> Generate Xcode Project.
4. **Compile the Editor**:
   - Open the generated solution/workspace.
   - Set the build configuration to `Development Editor`.
   - Build the project.
5. **Launch**:
   - Open `WhatIf.uproject` or run from your IDE.

## Project Structure Overview
- `Source/`: Contains all C++ source code.
  - `WhatIf/`: The main runtime module.
  - `WhatIfEditor/`: Editor-only extensions and tools.
- `Content/`: Unreal Engine assets (Blueprints, UI, Materials, Audio).
- `Docs/`: Detailed system documentation and design documents.
- `Tests/`: Automation scripts and unit tests.
- `Config/`: Engine configuration files (DefaultEngine.ini, etc.).

## Phase Roadmap Summary
The development of W.H.A.T. I.F. is divided into 15 phases:
- **Phase 0**: Project Architecture & Documentation
- **Phase 1**: Character Data Model
- **Phase 2**: Transformation System
- **Phase 3**: Attribute & Modifier System
- **Phase 4**: Basic Combat Simulator
- **Phase 5**: AI Agent Decision System
- **Phase 6**: Scenario Framework
- **Phase 7**: Timeline Branching
- **Phase 8**: Narrative Generation
- **Phase 9**: Unreal Engine Integration
- **Phase 10**: Character Customization UI
- **Phase 11**: Playable Combat Prototype
- **Phase 12**: Visual Polish
- **Phase 13**: Advanced AI Features
- **Phase 14**: Multiplayer & Community Integration

For a detailed breakdown, please see the [Roadmap](Docs/ROADMAP.md).

## Documentation Links
- [System Architecture](Docs/ARCHITECTURE.md)
- [Project Roadmap](Docs/ROADMAP.md)

## License
This project is licensed under the MIT License - see the LICENSE.md file for details.

## Contributing
Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.
