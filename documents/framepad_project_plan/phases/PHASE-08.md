# Phase 8 --- Release, Installation, and Maintenance

## Objective

Prepare repeatable firmware and Windows application releases.

## Firmware release

-   Reproducible CMake build instructions.
-   Pin dependency versions.
-   Document bootloader/UF2 flashing process.
-   Include firmware version and protocol version.
-   Produce release artifact with checksum.
-   Document SD card layout and formatting requirements.
-   Document supported display/board revision.

Do not implement USB firmware update unless there is a defined, tested
bootloader/update design. UF2 drag-and-drop can remain the initial
update method.

## Windows release

-   Choose a deployment approach (MSIX, self-contained installer, or
    another appropriate method).
-   Ensure required .NET runtime behavior is documented.
-   Install/uninstall cleanly.
-   Optional startup integration must be user-controlled.
-   Preserve user configuration during application upgrades.
-   Provide logs and diagnostics.
-   Sign binaries if a signing certificate becomes available.

## Documentation

Create:

-   `README.md`: project overview and quick start.
-   `docs/BUILD_FIRMWARE.md`
-   `docs/BUILD_SOFTWARE.md`
-   `docs/USER_GUIDE.md`
-   `docs/TROUBLESHOOTING.md`
-   `docs/PROTOCOL.md` or link to shared protocol repository.
-   `docs/IMAGE_FORMAT.md`
-   `docs/RELEASE_PROCESS.md`

## Release checklist

-   Clean firmware build.
-   Clean Windows build.
-   Protocol compatibility verified.
-   Macro actions verified.
-   Slideshow verified with USB disconnected.
-   Music controls and metadata verified.
-   Interrupted transfer tested.
-   SD card failure tested.
-   Version numbers and release notes updated.
-   Installation and recovery instructions reviewed.
