# Phase 3 --- Windows Application Foundation and Macro Execution

## Objective

Create the WPF companion application, connect to the Pico over USB CDC,
and implement six configurable macro slots whose actions execute on
Windows.

## Technology

-   C# and a currently supported .NET release.
-   WPF.
-   MVVM-oriented presentation layer.
-   `System.IO.Ports` or a vetted serial library.
-   `Microsoft.Extensions.Hosting` and dependency injection are optional
    but useful.
-   Keep core services independent of WPF.

## Suggested solution

``` text
Framepad.sln
  Framepad.App/             WPF views and view models
  Framepad.Core/            Models and interfaces
  Framepad.Communication/   Serial transport and protocol
  Framepad.Macros/          Macro definitions and execution
  Framepad.Storage/         Local configuration persistence
  Framepad.Tests/           Unit and integration tests
```

## Responsibilities

### Device connection service

-   Enumerate available serial ports.
-   Identify Framepad by USB VID/PID where known and verified; do not
    hardcode guessed identifiers.
-   Connect, handshake, and expose connection state.
-   Handle unplug, application shutdown, serial errors, and reconnect.
-   Avoid blocking the UI thread.
-   Ensure only one active connection owns the serial port.

### Macro model

A macro contains:

-   Stable slot ID 0--5.
-   Display name.
-   Enabled flag.
-   Icon asset reference.
-   Typed action definition.

The host is the source of truth for actions. The Pico sends only the
slot ID and never sends executable commands.

Initial action types:

-   Launch application (executable/path and arguments).
-   Open URL.
-   Keyboard shortcut.
-   Copy configured text to clipboard.
-   System volume/mute.
-   Microphone mute, if a reliable Windows endpoint API is available.
-   Media commands through the media service.

Do not implement arbitrary shell command execution as an implicit
default. Any custom command feature must be explicit, user-configured,
and validated.

### Persistence

-   Store user configuration in an application data directory.
-   Use a versioned JSON schema.
-   Write atomically (temporary file then replace).
-   Validate configuration on load.
-   Preserve unknown future fields if practical, or fail with a clear
    migration error.

## UI requirements

-   Six macro slots, no more than six.
-   Edit name, icon, enabled state, action type, and action parameters.
-   Validate required fields before saving.
-   Show connected/disconnected device state.
-   Show errors without crashing.
-   Allow the main window to close without silently terminating the
    device service if tray mode is enabled later.

## Tasks

1.  Create solution and projects.
2.  Implement protocol types and frame codec matching
    `protocol/PROTOCOL.md`.
3.  Implement serial connection service.
4.  Implement handshake and device info display.
5.  Implement macro model and versioned persistence.
6.  Implement macro editor.
7.  Implement macro executor and first action types.
8.  Dispatch received MACRO_PRESS events to the matching enabled macro.
9.  Add tests for configuration serialization, invalid macro
    definitions, and message parsing.

## Acceptance criteria

-   WPF app detects and connects to Pico.
-   Device state is visible.
-   Editing and saving six macros works.
-   Pressing a Pico macro button triggers the correct configured action.
-   No action is executed for unknown, disabled, or invalid macro IDs.
-   Serial reconnect and UI responsiveness are verified.
