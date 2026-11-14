# Phase 5 --- Standalone Slideshow and PC Image Conversion

## Objective

Implement a slideshow that reads images from the SD card and continues
operating without a PC connection. All expensive image processing occurs
on Windows.

## Initial image format

Use fixed-size 480x320 RGB565 files as the initial target, but confirm
panel orientation and actual active resolution from the existing
project. Document whether images are cropped, letterboxed, or stretched.

Define `protocol/IMAGE_FORMAT.md` with:

-   Width and height.
-   RGB565 bit layout.
-   Byte order on disk.
-   Row order and orientation.
-   Exact expected file size.
-   Any header/version (prefer a small documented header or manifest
    metadata).
-   Display rotation handling.
-   Color conversion and rounding rules.

Raw pixel data is 480 \* 320 \* 2 = 307,200 bytes (300 KiB). Never load
a complete image into RP2040 SRAM.

## PC image processor

Support common source formats through a maintained .NET imaging library.
Do not assume a library is already installed; inspect project
dependencies and choose one with appropriate licensing and maintenance.

Processing pipeline:

1.  Read source image.
2.  Apply EXIF orientation.
3.  Fit to device aspect ratio using a configurable crop/letterbox
    policy.
4.  Resize to target dimensions.
5.  Convert to RGB565 using documented channel packing and byte order.
6.  Write a temporary output.
7.  Verify exact output size and optionally compute CRC32.
8.  Add to the local asset library.

The UI should preview the crop before upload. Avoid destructive changes
to original photos.

## Pico rendering

-   Read bounded rows or tiles from SD.
-   Use a small static buffer sized from the memory budget.
-   Render through an approach compatible with the existing ILI9488
    driver.
-   Coordinate SD reads and display SPI writes if the bus is shared.
-   Do not block the main loop for an entire image.
-   Confirm LVGL flush behavior and display driver pixel format.
-   Handle missing, corrupt, or unsupported files gracefully.

A custom LVGL image decoder is an option, but a dedicated slideshow
renderer may be simpler and more memory predictable. Choose based on the
actual display driver and document the choice.

## Slideshow functionality

-   Enumerate supported files.
-   Next and previous image.
-   Automatic slideshow timer.
-   Configurable interval.
-   Optional shuffle.
-   Empty-library state.
-   Missing/corrupt file state.
-   Continue when USB is disconnected.
-   Manual SD-card upload works without PC application assumptions.

## PC photo manager

-   Import photos.
-   Preview and reorder.
-   Remove photos from the device with user confirmation.
-   Convert before upload.
-   Show transfer progress and failures.
-   Avoid deleting originals.
-   Report available device storage if the protocol exposes it.

## Acceptance criteria

-   At least a representative set of photos displays with correct colors
    and orientation.
-   No full-screen image allocation occurs in SRAM.
-   Slideshow works with USB disconnected.
-   Next/previous and automatic advance work.
-   Interrupted upload leaves existing images intact.
-   Image conversion output matches documented RGB565 test vectors.
