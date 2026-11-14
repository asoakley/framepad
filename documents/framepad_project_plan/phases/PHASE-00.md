# Phase 0 --- Existing Project Audit and Baseline

## Objective

Understand and preserve the current working Pico project before
refactoring. No major architecture changes in this phase.

## Copilot instructions

Inspect the repository and report:

-   Pico SDK version and toolchain configuration.
-   CMake targets, compile definitions, linker settings, and build
    commands.
-   RP2040 board configuration.
-   GPIO assignments and SPI peripherals used by ILI9488, XPT2046, and
    SD card.
-   Whether display and SD share an SPI bus.
-   LVGL version, configuration, draw-buffer sizes, color depth, and
    flush callback.
-   SquareLine-generated files and regeneration workflow.
-   Existing SD/filesystem implementation and supported file formats.
-   Existing USB/TinyUSB configuration, if any.
-   Current memory usage from the linker map/build output.
-   Current startup sequence and main loop.
-   Existing known defects and hardware dependencies.

Do not infer pin assignments from generic Waveshare examples. Use the
repository and the exact board documentation.

## Tasks

1.  Build the unmodified project using the documented toolchain.
2.  Record a baseline build result and any warnings.
3.  Run on hardware and verify display, touch, SD access, and existing
    macro behavior.
4.  Identify generated versus hand-written source files.
5.  Create `docs/HARDWARE.md` with verified board details and pin
    mapping.
6.  Create `docs/BASELINE.md` with build instructions, versions, current
    functionality, and known issues.
7.  Record approximate static RAM and flash usage.

## Deliverables

-   `docs/HARDWARE.md`
-   `docs/BASELINE.md`
-   A concise architecture inventory.
-   A reproducible baseline build.

## Acceptance criteria

-   Existing project builds before modifications.
-   Existing display and touch behavior is documented and verified where
    hardware is available.
-   Pin assignments and bus topology are evidence-based.
-   No hardware behavior has been changed.

## Stop condition

Do not proceed to refactoring until the baseline is understood. If the
project does not build, first diagnose and document the baseline
failure.
