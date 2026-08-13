# Contributing to W.H.A.T. I.F.

First off, thank you for considering contributing to the W.H.A.T. I.F. (What Happens After Transformation: Infinite Futures) engine! This document outlines the process for contributing to our repository, from setting up your environment to getting your Pull Request (PR) merged.

## Table of Contents
1. [Code of Conduct](#code-of-conduct)
2. [Setting Up Development Environment](#setting-up-development-environment)
3. [Git Workflow](#git-workflow)
4. [Commit Message Format](#commit-message-format)
5. [Pull Request (PR) Checklist](#pull-request-pr-checklist)
6. [ADR (Architecture Decision Record) Process](#adr-architecture-decision-record-process)
7. [Issue Reporting](#issue-reporting)

---

## 1. Code of Conduct

We are committed to providing a welcoming and inspiring community for all. 
- Be respectful and considerate of other contributors.
- Provide constructive feedback during code reviews.
- Harassment or exclusionary behavior is not tolerated.
- Focus on what is best for the project and the technical goals of W.H.A.T. I.F.

---

## 2. Setting Up Development Environment

W.H.A.T. I.F. is built on Unreal Engine 5.4. Ensure your environment matches the following specifications:

### Prerequisites
- **Unreal Engine 5.4**: Installed via the Epic Games Launcher or built from source.
- **IDE**: Visual Studio 2022 (Windows), Rider for Unreal (Windows/Mac), or Xcode (Mac).
- **Git LFS**: Required for handling binary assets (`.uasset`, `.umap`).

### Setup Steps
1. **Clone the repository**:
   ```bash
   git clone git@github.com:your-organization/what-if.git
   cd what-if
   ```
2. **Initialize Git LFS**:
   ```bash
   git lfs install
   git lfs pull
   ```
3. **Generate Project Files**: Right-click the `.uproject` file and select "Generate Visual Studio project files" (or equivalent for your OS/IDE).
4. **Build the Project**: Open the generated solution and build the `Development Editor` configuration.
5. **Launch the Editor**: Run the `.uproject` file.

---

## 3. Git Workflow

We utilize a strict branching model to ensure stability in the `main` branch.

### Branch Naming
Branch names should be descriptive and follow this format:
- `feature/issue-number-short-description` (e.g., `feature/102-add-timeline-graph`)
- `bugfix/issue-number-short-description` (e.g., `bugfix/204-fix-gas-attribute-crash`)
- `docs/short-description` (e.g., `docs/update-adr-005`)
- `refactor/short-description`

### Main Branch Rules
- The `main` branch must *always* be stable and compile successfully.
- Direct pushes to `main` are prohibited.
- All changes must go through a Pull Request and require at least one approval.

### Linear History
We enforce a linear commit history to make debugging via `git bisect` easier.
- **Rebase, don't merge**: When updating your branch with changes from `main`, use `git rebase main` instead of `git merge main`.
- **Squash commits**: PRs will be squash-merged into `main` by the repository maintainers.

---

## 4. Commit Message Format

We follow the Conventional Commits specification. This helps us generate changelogs automatically.

**Format**:
```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types**:
- `feat`: A new feature.
- `fix`: A bug fix.
- `docs`: Documentation only changes.
- `style`: Changes that do not affect the meaning of the code (white-space, formatting, etc).
- `refactor`: A code change that neither fixes a bug nor adds a feature.
- `perf`: A code change that improves performance.
- `test`: Adding missing tests or correcting existing tests.

**Example**:
```
feat(gas): add baseline transformation attribute set

Implemented UTransformationAttributeSet with max energy, corruption level, and stability stats.

Fixes #145
```

---

## 5. Pull Request (PR) Checklist

Before submitting a PR, ensure you have completed the following:

- [ ] **Coding Standards**: Verified code against `CODING_STANDARDS.md`.
- [ ] **C++ Classes**: Placed headers correctly in `Public/Private` folders and used appropriate prefixes.
- [ ] **Interfaces**: Ensured new systems are properly abstracted behind `UInterface` contracts.
- [ ] **Data Schema**: Kept data definitions in C++ and instantiated them in `UPrimaryDataAsset` or `DataTable` representations.
- [ ] **Testing**: Added unit or functional tests where applicable, and manually verified changes in the Editor.
- [ ] **Documentation**: Updated necessary documentation (header comments, wikis).
- [ ] **DECISIONS.md**: If architectural changes were made, created or updated an ADR.
- [ ] **Clean History**: Rebased against the latest `main` branch and resolved any conflicts.

---

## 6. ADR (Architecture Decision Record) Process

Architectural Decisions are documented in `Docs/DECISIONS.md`. They capture the *why* behind significant technical choices.

### When to write an ADR
- Selecting a new core technology or engine subsystem.
- Defining a major structural pattern (e.g., how the Narrative LLM interacts with the game state).
- Changing an existing architectural standard.

### ADR Format
Use the established format found in `DECISIONS.md`:
- **ADR-XXX**: [Title]
- **Status**: Proposed / Accepted / Deprecated / Superseded
- **Date**: YYYY-MM-DD
- **Context**: The problem space and requirements.
- **Decision**: The specific choice being made.
- **Rationale**: Why this choice was made over others.
- **Alternatives Considered**: Other options and why they were rejected.
- **Consequences**: What becomes easier or harder due to this decision.

### Where to put it
All ADRs are appended to the `/Docs/DECISIONS.md` file sequentially. Do not create separate files for each ADR.

---

## 7. Issue Reporting

When reporting a bug or requesting a feature, please use the issue templates provided in the repository.

- **Bug Reports**: Must include replication steps, expected behavior, actual behavior, and error logs/callstacks.
- **Feature Requests**: Must clearly describe the problem being solved and proposed solution. Consider writing a pre-ADR proposal for large features.

Thank you for contributing to the infinite futures of W.H.A.T. I.F.!
