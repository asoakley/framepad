# Framepad Baseline

**Audit date:** 2026-11-14  
**Scope:** Software/build baseline for Phase 0. Physical hardware verification was intentionally skipped at the project owner's request. GPIO and controller details below describe the repository configuration, not verified board wiring.

## Project architecture

The active target is a cooperative Raspberry Pi Pico / RP2040 firmware project built with CMake and the Pico SDK. The root build compiles `main.c`, the task modules, `StreamDeck.c`, and libraries for the LCD, SD card, FatFs, USB, and related Waveshare-derived drivers. `main.cpp`, LVGL, TFT_eSPI, and the SquareLine-generated `GUI` target are present in the repository but are not included in the root build.

Startup in `main.c` initializes the platform, mounts the SD card, initializes the LCD and touch controller, loads and draws the stream-deck grid, and initializes TinyUSB. The main loop services TinyUSB, LED, touch, HID, LCD, and sleep tasks cooperatively.

## Hardware configuration in source

The root CMake configuration selects `PICO_BOARD pico`. The configured SPI and GPIO assignments in `config/DEV_Config.h` and `config/DEV_Config.c` are:

| Function | GPIO |
| --- | ---: |
| SPI1 SCK / MOSI / MISO | 10 / 11 / 12 |
| LCD DC / CS / reset / backlight | 8 / 9 / 15 / 13 |
| Touch CS / IRQ | 16 / 17 |
| SD CS | 22 |

LCD, touch, and SD use SPI1, so they share the bus. The bus is initialized at 4 MHz; touch code changes its baud rate. The SD low/high speed functions are empty. Confirm bus timing, chip-select behavior, actual board revision, and wiring against the physical device before treating this map as hardware-verified.

The LCD driver configures a 480x320 display. Its header labels the driver ILI9486, while the project plan names an ILI9488 panel. The touch source identifies the XPT2046 controller. The actual display controller and board revision remain unverified.

## Dependencies and tool versions

| Component | Version / configuration | Use |
| --- | --- | --- |
| Raspberry Pi Pico SDK | 2.2.0 | Active firmware SDK |
| Arm GNU Toolchain | GCC 14.2.1, toolchain bundle `14_2_Rel1` | Active firmware compiler |
| TinyUSB | 0.18.0, supplied by the Pico SDK | Active USB HID device stack |
| FatFs | R0.10 | Active SD filesystem; configured read/write, no long filenames, non-reentrant |
| Waveshare-derived LCD/touch/SD drivers | Source comments identify v1.0 / 2018 lineage; no package version found | Active hardware drivers |
| CMake / Ninja | 3.31.5 / 1.12.1 in the tested setup | Configure and build |
| picotool | 2.2.0-a4 in the VS Code setup | Flash/load tooling |
| LVGL | 9.5.0 | Vendored, inactive in root build |
| TFT_eSPI | 2.4.61 | Vendored, inactive in root build |

The vendored LVGL configuration sets 32-bit color depth, 16-bit swap off, a 64 KiB LVGL memory pool, and no OS. These settings do not describe the active firmware because the LVGL target is not built.

## Build baseline

The root `CMakeLists.txt` sets `PICO_BOARD` to `pico`, initializes the Pico SDK, builds the `main` target, enables USB stdio, disables UART stdio, and calls `pico_add_extra_outputs`. The VS Code build task runs Ninja in `build/`.

In the tested Windows environment, the project configured and built successfully with CMake 3.31.5, Ninja 1.12.1, Pico SDK 2.2.0, GCC 14.2.1, and Python 3.15.2. CMake required `Python3_EXECUTABLE` to be set to the actual interpreter path; the initial configure attempt could not use the WindowsApps Python alias.

Example PowerShell commands (replace the Python path with the installed interpreter path):

```powershell
$base = Join-Path $env:USERPROFILE '.pico-sdk'
$env:PICO_SDK_PATH = Join-Path $base 'sdk\2.2.0'
$env:PICO_TOOLCHAIN_PATH = Join-Path $base 'toolchain\14_2_Rel1'
$python = 'C:\path\to\python.exe'
cmake -S . -B build -G Ninja "-DPython3_EXECUTABLE=$python"
if ($LASTEXITCODE -eq 0) { ninja -C build }
```

The build completed with one warning from Pico SDK USB stdio: USB stdio is enabled while TinyUSB device mode is in use but CDC is disabled. No automated test target or test suite was found. The build produced `build/main.elf`; the root CMake configuration also requests standard Pico extra outputs.

### ELF section sizes

Measured with `arm-none-eabi-size -A build/main.elf`:

| Section | Bytes |
| --- | ---: |
| `.boot2` | 256 |
| `.text` | 45,868 |
| `.rodata` | 5,076 |
| `.binary_info` | 32 |
| `.data` | 5,624 |
| `.bss` | 5,028 |
| `.ram_vector_table` | 192 |
| `.heap` | 2,048 |
| `.stack_dummy` | 2,048 |

The flash load image is approximately 56,856 bytes when `.data`'s flash load image is included. The listed RAM sections total approximately 14,940 bytes, including the linker-reported heap and stack sections. These are linker/ELF figures, not a measurement of runtime heap use. No linker map file was configured.

## Existing functionality

- `StreamDeck.c` reads SD-root entries in the form `image.bin command`, lays out up to six icons in a 2x3 grid, and draws raw RGB565 image data. The current repository `config.txt` has six entries. The image buffer is allocated dynamically while drawing.
- `TouchTask.c` polls touch about every 50 ms, applies a 200 ms debounce, and maps the resulting position to an active icon.
- `HIDTask.c` sends Win+R, waits 500 ms, types supported alphabetic/numeric characters, then sends Enter. Unsupported characters are skipped.
- TinyUSB is configured for HID only. Its report descriptor includes keyboard, mouse, consumer-control, and gamepad reports. CDC is disabled; there is no CDC protocol or Windows companion application in this repository.
- LED behavior changes on USB mount, unmount, and suspend callbacks.
- A separate legacy BMP/FatFs display implementation exists, but the active startup path uses the raw RGB565 stream-deck icon loader.
- SquareLine-generated GUI source contains macro, slideshow, and music screen mockups with static content, but it is not part of the active build. No regeneration project or workflow was identified in the repository.

## Known issues and unresolved verification

- The project plan calls for an ILI9488 panel; the active LCD driver header labels ILI9486. Resolve this from the actual board/controller documentation before changing the driver.
- USB stdio is enabled but CDC is disabled, producing the baseline build warning. Current host interaction is HID keyboard emulation, not a serial protocol.
- The source-derived GPIO map and shared SPI topology have not been compared with the physical board or its schematic.
- Display output, touch calibration/coordinates, SD mounting and icon loading, macro behavior, and LED indication were not tested on hardware. These checks were intentionally skipped per the project owner's request and must not be treated as passed.
- No baseline documentation or code change here alters the active hardware implementation.
