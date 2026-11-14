# Phase 1 --- Firmware Modularization

## Objective

Refactor the existing Pico firmware into maintainable modules while
preserving current functionality. Do not implement the complete new
feature set yet.

## Target logical structure

Adapt to the existing repository rather than forcing a disruptive file
move:

``` text
src/
  app/
    FramepadApp.hpp/.cpp
    DeviceState.hpp/.cpp
    EventQueue.hpp/.cpp
  hardware/
    DisplayDriver.hpp/.cpp
    TouchDriver.hpp/.cpp
    SDDriver.hpp/.cpp
  gui/
    GuiManager.hpp/.cpp
    MacroPageController.hpp/.cpp
    SlideshowPageController.hpp/.cpp
    MusicPageController.hpp/.cpp
  storage/
    StorageManager.hpp/.cpp
    ConfigManager.hpp/.cpp
    AssetManager.hpp/.cpp
  features/
    MacroManager.hpp/.cpp
    SlideshowManager.hpp/.cpp
    MusicManager.hpp/.cpp
  communication/
    USBManager.hpp/.cpp
    Protocol.hpp/.cpp
```

## Design requirements

-   Generated SquareLine source remains isolated.
-   Hand-written logic must not be placed in generated files.
-   UI event callbacks call controller/application functions.
-   Feature managers do not directly manipulate low-level SPI or GPIO.
-   Hardware drivers expose narrow interfaces.
-   Use fixed-size data structures and bounded strings where practical.
-   No RTOS.
-   The main loop must service LVGL regularly.
-   Long-running work must be incremental.
-   USB callbacks and interrupt handlers must remain short.
-   LVGL access occurs from the main/UI context unless the chosen LVGL
    port explicitly supports another safe model.

## Suggested main-loop responsibilities

Use a cooperative loop with bounded work:

``` cpp
while (true)
{
    tud_task();                  // if TinyUSB is enabled
    usbManager.process();
    eventQueue.process();
    storageManager.process();
    slideshowManager.process();
    musicManager.process();
    lv_timer_handler();
    // Sleep/yield only as appropriate for the existing port.
}
```

Do not copy this blindly: integrate with the project's actual LVGL tick
and Pico SDK timing implementation.

## Tasks

1.  Introduce `FramepadApp` to own initialization and periodic
    processing.
2.  Introduce `DeviceState` for connection and feature state.
3.  Separate hardware initialization from GUI initialization.
4.  Move application behavior out of generated callbacks.
5.  Introduce page controllers for the three existing pages.
6.  Introduce a bounded event queue if asynchronous events need to cross
    module boundaries.
7.  Preserve current macro icon loading and current SD behavior during
    this phase.
8.  Add build-time checks or static assertions for fixed buffer sizes.
9.  Document module ownership and permitted dependencies.

## Acceptance criteria

-   Firmware builds.
-   Existing display, touch, and current macro behavior still work.
-   Generated SquareLine files are isolated from application logic.
-   No long blocking operation is introduced into the main loop.
-   Each module has a clear owner and public interface.
