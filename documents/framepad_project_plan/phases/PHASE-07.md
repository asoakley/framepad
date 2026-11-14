# Phase 7 --- User Experience, Recovery, and Product Hardening

## Objective

Make the firmware and companion application robust under normal user
behavior, errors, disconnects, and interrupted operations.

## Firmware work

-   Explicit connection states: disconnected, connecting, synchronizing,
    connected, error/recovering.
-   Safe standalone behavior.
-   Last-known-good configuration.
-   SD mount failure and card removal handling.
-   Corrupt config and corrupt image handling.
-   USB receive overflow protection.
-   Transfer timeout and cleanup.
-   Bounded retry/backoff.
-   Watchdog only if recovery behavior is designed and tested.
-   No unbounded heap growth.
-   Track stack/heap high-water marks where feasible.
-   Ensure LVGL calls occur in the correct context.

## Windows application work

-   System tray operation and optional startup launch.
-   Clear device connection status.
-   Reconnect without duplicate serial-port ownership.
-   Cancellation and timeout support for transfers.
-   User-visible error messages.
-   Configuration backup and restore.
-   Photo library management.
-   Macro validation and safe execution.
-   Application logging with rotation and privacy-conscious content.
-   Avoid logging clipboard contents, secrets, or sensitive macro
    parameters.

## Compatibility

-   Protocol version negotiation.
-   Firmware version and software version display.
-   Schema migration strategy.
-   Clear unsupported-version error.
-   Capability negotiation rather than assuming every firmware supports
    every optional feature.

## Testing matrix

Test at minimum:

  Condition                       Expected behavior
  ------------------------------- -----------------------------------------
  USB unplug during idle          Pico continues standalone
  USB unplug during transfer      Temp file discarded or safely resumable
  PC app exits unexpectedly       Pico remains responsive
  SD card absent at boot          Non-storage features still start
  SD card removed while running   Graceful storage error; no crash
  Invalid config                  Last valid config/defaults
  Bad CRC                         Reject frame/file
  Oversized message               Reject without overflow
  Unknown message type            Return error or safely ignore
  No media session                Music page shows no-session state
  Empty slideshow                 Helpful empty state
  Corrupt photo                   Skip/report without wedging UI

## Acceptance criteria

-   All test matrix cases pass or have documented hardware limitations.
-   No known unbounded buffers or unsafe path handling.
-   Reconnection does not require reboot.
-   Device remains usable when the PC app is unavailable.
