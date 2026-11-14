# GitHub Copilot Workflow for Framepad

Use this file as project-level guidance when asking Copilot Chat or
Agent mode to implement the plan.

## Before each task

Ask Copilot to:

1.  Read `README.md` and the current phase document.
2.  Inspect relevant existing source files and dependencies.
3.  State the proposed changes and any assumptions.
4.  Identify hardware facts that must be verified rather than guessed.
5.  Identify tests/build commands to run.

## Task sizing

Give Copilot one coherent task at a time, preferably small enough to
review in one pull request. Examples:

-   "Implement the protocol frame encoder and decoder described in
    `protocol/PROTOCOL.md`."
-   "Add tests for fragmented CDC frame input."
-   "Create the C# serial connection service and handshake state
    machine."
-   "Implement RGB565 conversion and golden-vector tests."

Avoid asking Copilot to implement an entire phase in one prompt.

## Required implementation behavior

-   Follow existing code style and conventions unless there is a clear
    reason to change them.
-   Do not fabricate SDK APIs, GPIO assignments, or hardware behavior.
-   Prefer small interfaces and explicit ownership.
-   Keep firmware buffers bounded and document capacities.
-   Use cancellation tokens for cancellable host-side asynchronous
    operations.
-   Keep UI thread work short.
-   Do not run long file operations in USB callbacks or LVGL callbacks.
-   Do not put business logic in generated SquareLine files.
-   Do not change protocol/schema/image formats without updating
    documentation and tests.
-   Do not add dependencies without explaining why and checking
    compatibility/licensing.
-   Do not remove existing functionality without explicit approval.

## After each task

Copilot should:

1.  Summarize files changed.
2.  Explain design decisions and any deviations from the plan.
3.  Run the relevant build and tests.
4.  Report tests not run and why.
5.  List hardware verification still required.
6.  Update phase acceptance criteria only when evidence supports
    completion.

## Prompt template

``` text
Read README.md, COPILOT_WORKFLOW.md, and the current phase file.

Implement only this task:
[Describe one specific task.]

First inspect the existing code and report the files and interfaces involved.
Do not guess hardware details or APIs. Preserve existing working behavior.
Implement the smallest complete change, add/update tests and documentation,
then build and run the relevant tests.

At the end, report:
- Files changed
- Behavior implemented
- Build/test results
- Assumptions and unresolved questions
- Hardware verification required
- Remaining acceptance criteria
```

## Phase completion

Do not mark a phase complete because Copilot generated code. Mark it
complete only after the acceptance criteria have been verified,
including physical hardware tests where required.
