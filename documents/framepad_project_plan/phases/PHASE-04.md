# Phase 4 --- Device Configuration and Asset Synchronization

## Objective

Synchronize the device-facing configuration and macro icons from the PC
to the Pico SD card. The PC retains the authoritative macro actions; the
Pico receives only display-related data.

## Data ownership

  Data                                                   Authoritative location
  ------------------------------------------------------ -------------------------------
  Macro actions and executable parameters                PC
  Device-facing labels, enabled flags, icon references   PC
  Icon files currently on device                         SD card, synchronized from PC
  Slideshow images                                       PC library and SD card copy

## Device configuration

Define a versioned schema, for example:

``` json
{
  "schema_version": 1,
  "device": {
    "brightness": 100
  },
  "macros": [
    {
      "id": 0,
      "name": "VS Code",
      "enabled": true,
      "icon": "macros/macro_0.rgb565"
    }
  ]
}
```

The actual schema must define maximum string lengths, allowed filenames,
required fields, default values, and migration behavior. Do not use
unbounded dynamic strings on the Pico.

## SD directory layout

``` text
/
  system/
    device_config.json
    manifest.json
  macros/
    macro_0.rgb565
    ...
    macro_5.rgb565
  slideshow/
  temp/
```

All paths must be normalized and constrained to the expected root
directories. Reject path traversal, absolute host paths, invalid
characters, and names exceeding limits.

## Transfer protocol

Define:

-   FILE_BEGIN: transfer ID, target logical path, total size, CRC32,
    content type.
-   FILE_CHUNK: transfer ID, offset, data length, bytes.
-   FILE_END: transfer ID.
-   ACK/ERROR: transfer status and useful error code.

Use bounded chunk sizes chosen after testing USB throughput and Pico
RAM. Support retry of a failed chunk. Resume support is optional;
correctness is mandatory.

Write to a temporary file. Verify exact byte count and CRC32. Only then
rename to final filename and update the manifest/configuration. Never
overwrite a valid asset with a partial transfer.

## Manifest and synchronization

The host keeps a local asset library and manifest containing logical
filename, size, checksum, and format version. On connection:

1.  Host requests device file list/manifest.
2.  Host compares expected assets with device assets.
3.  Transfer only missing or changed files.
4.  Remove obsolete assets only after a successful synchronization plan
    and explicit safe policy.
5.  Confirm configuration is applied.

Do not automatically delete user-uploaded slideshow files unless the
user explicitly requests mirroring/deletion semantics.

## Tasks

1.  Finalize device config schema.
2.  Implement bounded config parser/validator on Pico.
3.  Implement atomic config replacement and last-known-good fallback.
4.  Implement safe file transfer and temporary files.
5.  Implement file listing and manifest exchange.
6.  Implement PC-side asset library and sync planner.
7.  Implement icon transfer and GUI refresh.
8.  Add tests for invalid paths, interrupted transfer, CRC mismatch, and
    stale manifest.

## Acceptance criteria

-   PC can update all six macro labels, enabled states, and icons.
-   Device retains last valid configuration after failed update.
-   Interrupted transfers do not corrupt existing assets.
-   User slideshow content is not deleted unexpectedly.
-   Device can display its last synchronized macro layout while
    disconnected.
