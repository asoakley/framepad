# Framepad Project Plan

**Purpose:** A phased, implementation-ready roadmap for GitHub Copilot
in VS Code to build the Framepad firmware and Windows companion
application.

## Project summary

Framepad is a Raspberry Pi Pico (RP2040) device using a Waveshare
3.5-inch ILI9488 TFT, XPT2046 touch controller, and microSD card. It has
three user-facing pages:

1.  **Macros:** Up to six programmable buttons. The Windows companion
    application owns macro definitions and executes actions.
2.  **Slideshow:** Displays locally stored photos and works when USB is
    disconnected.
3.  **Music:** Sends playback commands to Windows and displays media
    metadata supplied by the Windows companion application.

The firmware is an existing Pico SDK C/C++ project built with CMake and
containing SquareLine-generated LVGL code. Preserve working hardware
support and generated UI assets while refactoring incrementally.

## Fixed project decisions

-   MCU: Original Raspberry Pi Pico / RP2040, 264 KB SRAM.
-   Firmware: C++ with Raspberry Pi Pico SDK and CMake.
-   Scheduling: Cooperative main loop; do not introduce an RTOS without
    a demonstrated need.
-   GUI: Existing LVGL/SquareLine project, refactored behind
    application-facing controllers.
-   USB: TinyUSB CDC virtual COM port.
-   Host application: C# / .NET WPF, MVVM-oriented.
-   Host communication: `System.IO.Ports` or a well-maintained serial
    abstraction.
-   Filesystem: Reuse the filesystem implementation already present if
    sound; otherwise integrate FatFs.
-   Photo pipeline: Convert images on Windows to a documented device
    format before transfer.
-   Initial photo format: fixed-dimension RGB565, nominally 480x320,
    subject to confirmation against the actual panel orientation and
    display driver.
-   Music: Windows Global System Media Transport Controls (GSMTC) first.
    Spotify Web API is optional and must not be required for basic
    playback or metadata.
-   Macro definitions and actions: Windows application is the source of
    truth. Pico sends macro IDs only.
-   Device-facing configuration and assets: stored on SD card,
    synchronized from the PC.
-   Firmware and Windows application may be separate repositories. Keep
    protocol and format specifications versioned and synchronized.

## How Copilot should use this plan

1.  Read this README and the relevant phase file before changing code.
2.  Inspect the existing repository and identify the actual Pico SDK,
    LVGL, SquareLine, display, touch, SD, and USB versions before
    assuming APIs.
3.  Do not replace working hardware drivers merely to match an example
    in this plan.
4.  Work only on the current phase unless explicitly asked to continue.
5.  Before editing, summarize the existing architecture, files to
    change, and risks.
6.  Implement in small, buildable increments. Build and run available
    tests after each increment.
7.  Do not invent GPIO pin assignments, SPI topology, display timing,
    controller settings, or filesystem configuration. Derive them from
    the existing project, board schematic, and verified hardware.
8.  Keep generated SquareLine files separate from hand-written
    application logic. Never put durable application behavior in
    generated files.
9.  Do not silently change protocol fields, file formats, or
    configuration schemas. Update the relevant specification and
    compatibility tests.
10. Avoid dynamic allocation in time-critical or memory-constrained
    firmware paths where practical. Use bounded buffers and explicit
    maximum sizes.
11. Never block the main loop for long SD operations, file transfers, or
    image rendering. Use incremental processing.
12. Keep all USB callbacks short: copy validated bytes into bounded
    buffers or queues; do not perform filesystem operations or call LVGL
    from USB callbacks.
13. Treat all host-provided paths, filenames, and macro-related data as
    untrusted input. Validate and constrain paths to approved SD
    directories.
14. Do not execute arbitrary commands received from the Pico. The PC
    executes only macro actions explicitly configured by the user in the
    companion application.
15. Add or update tests and documentation for every externally
    observable behavior.

## Repository strategy

Recommended repositories:

-   `Framepad-Firmware`: Pico SDK firmware.
-   `Framepad-Software`: WPF companion application.
-   `Framepad-Protocol`: shared protocol, configuration, and
    image-format specifications, or a versioned `docs/protocol`
    directory copied into both repositories.

If using only one repository, preserve equivalent project separation:

``` text
Framepad/
├── firmware/
├── software/
├── protocol/
└── docs/
```

## Phase sequence

  Phase   Deliverable
  ------- ------------------------------------------------------
  0       Existing-project audit and baseline
  1       Firmware modularization without feature regression
  2       USB CDC protocol and device connection
  3       Windows application foundation and macro execution
  4       Device configuration and asset synchronization
  5       Standalone slideshow and PC image conversion
  6       Windows media integration and music page
  7       UX, recovery, and product hardening
  8       Release, installation, and maintenance documentation

See `phases/PHASE-00.md` through `phases/PHASE-08.md`. Each phase has
entry criteria, tasks, acceptance criteria, and Copilot instructions.

## Global definition of done

A feature is not complete merely because code compiles. It is complete
when:

-   The behavior is implemented and tested at the appropriate level.
-   Errors and disconnected states are handled.
-   Memory, buffer, and file-size limits are explicit.
-   Documentation and protocol/schema definitions are updated.
-   Existing functionality has not regressed.
-   The relevant acceptance criteria in its phase file pass.

## Important hardware constraints

The RP2040 has 264 KB SRAM. A 480x320 RGB565 frame is 307,200 bytes (300
KiB), larger than total SRAM. Do not allocate a full-frame RGB565 buffer
in RAM. Use bounded tile/row buffers and an appropriate display transfer
strategy. Verify the ILI9488 driver's color-depth and byte-order
expectations; many ILI9488 modules use an 18-bit panel interface
internally even when the software source format is RGB565.

The Waveshare board may share SPI resources between display and SD.
Verify the exact board revision and schematic. Ensure chip-select
handling, SPI reconfiguration, and bus ownership are correct. Do not
assume display and SD can be accessed concurrently.

## Initial implementation priority

The first end-to-end vertical slice is:

1.  Pico starts and retains the existing display/touch functionality.
2.  Pico enumerates as USB CDC.
3.  PC application detects and connects to it.
4.  Pico sends a `MACRO_PRESS` event with an ID.
5.  PC receives the event and executes a user-configured test action.
6.  PC sends a message back and Pico updates a visible status label.

Do not start photo transfer or GSMTC integration until this vertical
slice is reliable.
