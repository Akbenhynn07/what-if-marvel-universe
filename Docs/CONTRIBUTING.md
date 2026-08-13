# Contributing to W.H.A.T. I.F.

First off, thank you for considering contributing to the W.H.A.T. I.F. (What Happens After Transformation: Infinite Futures) engine! This project relies heavily on community and team collaboration to build out its complex branching timeline simulations and AI-driven narrative systems. 

This document outlines the entire process for contributing to our repository, from setting up your development environment to getting your Pull Request (PR) reviewed and merged. By following these guidelines, you help us maintain a high standard of code quality and architectural integrity.

## Table of Contents
1. [Code of Conduct](#code-of-conduct)
2. [Setting Up Development Environment](#setting-up-development-environment)
3. [Git Workflow and Branching Strategy](#git-workflow-and-branching-strategy)
4. [Commit Message Format](#commit-message-format)
5. [Pull Request (PR) Lifecycle](#pull-request-pr-lifecycle)
6. [ADR (Architecture Decision Record) Process](#adr-architecture-decision-record-process)
7. [Issue Reporting Guidelines](#issue-reporting-guidelines)
8. [Debugging and Profiling](#debugging-and-profiling)
9. [Community and Communication](#community-and-communication)

---

## 1. Code of Conduct

We are committed to providing a welcoming, inclusive, and inspiring community for all developers, designers, and testers. 

- **Be Respectful**: Treat everyone with respect. Differing technical opinions are natural, but they must be discussed professionally.
- **Constructive Feedback**: During code reviews, focus on the code, not the person. Suggest improvements rather than just pointing out flaws.
- **No Harassment**: Harassment or exclusionary behavior of any kind is strictly prohibited and will result in immediate removal from the project.
- **Goal Oriented**: Focus on what is best for the project and the technical goals of W.H.A.T. I.F. Keep the project's vision in mind during all discussions.
- **Diversity and Inclusion**: We actively encourage participation from people of all backgrounds. Diverse teams build better simulations.

---

## 2. Setting Up Development Environment

W.H.A.T. I.F. is built on Unreal Engine 5.4. Ensure your environment matches the following specifications to avoid compatibility issues.

### Prerequisites
- **Unreal Engine 5.4**: Installed via the Epic Games Launcher or built directly from source (preferred for advanced debugging).
- **IDE**: 
  - Windows: Visual Studio 2022 (with "Game development with C++" workload) or JetBrains Rider for Unreal.
  - macOS: Xcode.
- **Git LFS**: Required for handling binary assets (`.uasset`, `.umap`). Without Git LFS, the repository will fail to load assets correctly.

### Setup Steps
1. **Install Git LFS**: Ensure Git LFS is installed globally on your machine (`git lfs install`).
2. **Clone the repository**:
   ```bash
   git clone git@github.com:your-organization/what-if.git
   cd what-if
   ```
3. **Pull LFS Files**:
   ```bash
   git lfs pull
   ```
4. **Generate Project Files**: 
   - Right-click the `WhatIf.uproject` file.
   - Select "Generate Visual Studio project files" (or your OS equivalent).
5. **Build the Project**: 
   - Open the generated `.sln` or `.xcworkspace`.
   - Set your build configuration to `Development Editor`.
   - Compile the solution.
6. **Launch the Editor**: 
   - Run the `.uproject` file or launch via your IDE.

---

## 3. Git Workflow and Branching Strategy

We utilize a strict branching model to ensure absolute stability in the `main` branch. Broken builds block the entire team, so extreme care must be taken.

### Branch Naming Conventions
Branch names must be descriptive and follow this standard format:
- `feature/issue-number-short-description` (e.g., `feature/102-add-timeline-graph`)
- `bugfix/issue-number-short-description` (e.g., `bugfix/204-fix-gas-attribute-crash`)
- `docs/short-description` (e.g., `docs/update-adr-005`)
- `refactor/short-description` (e.g., `refactor/cleanup-character-components`)
- `test/short-description` (e.g., `test/add-timeline-unit-tests`)

### Main Branch Rules
- The `main` branch must *always* be stable, compile successfully, and pass all automated tests.
- Direct pushes to `main` are strictly prohibited. The repository is configured to block them.
- All changes must go through a Pull Request and require at least one approval from a lead developer.
- Force pushes (`git push -f`) to the main branch are completely disabled.

### Linear History Requirement
We enforce a linear commit history to make debugging via `git bisect` trivial and to keep the repository history clean and readable.
- **Rebase, don't merge**: When updating your branch with the latest changes from `main`, you must rebase your work. 
  ```bash
  git fetch origin
  git rebase origin/main
  ```
- **Resolve Conflicts Locally**: If you encounter conflicts during rebase, resolve them locally, `git add` the files, and run `git rebase --continue`.
- **Squash Commits**: We use squash-merging for PRs. This means all your commits on a branch will be squashed into a single commit when merged into `main`.

---

## 4. Commit Message Format

We follow the Conventional Commits specification. This helps us generate automated release notes and changelogs.

**Format structure**:
```
<type>(<scope>): <subject>

<body>

<footer>
```

**Allowed Types**:
- `feat`: A new gameplay feature or engine subsystem.
- `fix`: A bug fix.
- `docs`: Documentation only changes.
- `style`: Changes that do not affect the meaning of the code (white-space, formatting, etc).
- `refactor`: A code change that neither fixes a bug nor adds a feature.
- `perf`: A code change that improves performance or memory usage.
- `test`: Adding missing tests or correcting existing tests.
- `chore`: Maintenance tasks like updating dependencies or build scripts.

**Example**:
```
feat(gas): add baseline transformation attribute set

Implemented UTransformationAttributeSet with max energy, corruption level, and stability stats.
Exposed these variables to blueprints to allow designer tuning.

Fixes #145
```

---

## 5. Pull Request (PR) Lifecycle

Before submitting a PR, you must ensure your code is completely ready for review.

### Pre-PR Checklist
- [ ] **Coding Standards**: Verified code against `Docs/CODING_STANDARDS.md`.
- [ ] **C++ Classes**: Placed headers correctly in `Public/Private` folders and used appropriate Unreal prefixes.
- [ ] **Interfaces**: Ensured new systems are properly abstracted behind `UInterface` contracts.
- [ ] **Data Schema**: Kept data definitions in C++ and instantiated them in `UPrimaryDataAsset` or `DataTable` representations.
- [ ] **Testing**: Added unit or functional tests where applicable, and manually verified changes in the Editor.
- [ ] **Clean History**: Rebased against the latest `main` branch and resolved any conflicts.

### During the Review
1. **Open the PR**: Provide a clear description, attach screenshots or videos for visual changes, and explicitly list the steps to test the feature manually.
2. **Respond to Feedback**: Reviewers will leave comments. Address them constructively. If you push new commits to fix issues, notify the reviewers.
3. **Approval**: Once you receive an approval, a maintainer will perform a squash-merge into `main`.

---

## 6. ADR (Architecture Decision Record) Process

Architectural Decisions are documented in `Docs/DECISIONS.md`. They capture the *why* behind significant technical choices. W.H.A.T. I.F. is a complex project, and understanding historical context is vital.

### When to write an ADR
- Selecting a new core technology or engine subsystem (e.g., opting to use GAS over a custom system).
- Defining a major structural pattern (e.g., how the Narrative LLM interacts with the deterministic game state).
- Changing an existing architectural standard.

### ADR Format
Use the established format found in `DECISIONS.md`:
- **ADR-XXX**: [Title]
- **Status**: Proposed / Accepted / Deprecated / Superseded
- **Date**: YYYY-MM-DD
- **Context**: The problem space, constraints, and requirements.
- **Decision**: The specific choice being made.
- **Rationale**: Why this choice was made over others.
- **Alternatives Considered**: Other options and specifically why they were rejected.
- **Consequences**: What becomes easier or harder due to this decision.

### Where to put it
All ADRs are appended to the `/Docs/DECISIONS.md` file sequentially. Do not create separate files for each ADR.

---

## 7. Issue Reporting Guidelines

When reporting a bug or requesting a feature, please use the issue templates provided in the repository.

- **Bug Reports**: Must include replication steps, expected behavior, actual behavior, Unreal engine version, and error logs/callstacks. Use `git bisect` if possible to find the exact commit that introduced the bug.
- **Feature Requests**: Must clearly describe the problem being solved and proposed solution. Consider writing a pre-ADR proposal for large, sweeping features.

---

## 8. Debugging and Profiling

When contributing code, especially code that runs frequently (like timeline graph traversal), you are expected to ensure it performs well.

- **Unreal Insights**: Use Unreal Insights to profile CPU and Memory usage. Do not guess where bottlenecks are; prove it with data.
- **Console Variables (CVars)**: Expose debug tools via CVars so testers can toggle your systems on and off at runtime without recompiling.
- **Draw Debug Helpers**: Use standard Unreal draw debug tools to visualize spatial algorithms, trace lines, and collision queries.

---

## 9. Community and Communication

- **Discord**: Join our Discord server to discuss architecture, ask questions, or just hang out with the team.
- **Weekly Sync**: We hold a weekly sync meeting every Friday to discuss ongoing work and upcoming features.
- **Wiki**: Check out our project Wiki for in-depth tutorials on using specific engine features.

Thank you again for contributing to the infinite futures of W.H.A.T. I.F.! Your dedication to clean code and robust architecture makes this project possible.
