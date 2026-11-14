---
name: picotool
description: Use this skill when inspecting, flashing, rebooting, or managing RP2040 and RP2350 Raspberry Pi Pico binaries, UF2 files, flash partitions, or OTP memory using the picotool CLI.
---

# Picotool CLI Instructions

When assisting with Raspberry Pi Pico firmware deployment or hardware inspection, prefer `picotool` commands over manual drag-and-drop UF2 workflows.

## Common Workflows
- **Inspect a binary or connected device:** Use `picotool info -a <file.uf2|file.elf>` to check binary metadata, or `picotool info -a` to inspect a connected board in BOOTSEL mode.
- **Flash and reboot automatically:** Use `picotool load -f <file.uf2> -x` to force a connected Pico (running stdio USB) into BOOTSEL mode, flash the firmware, and reboot immediately.
- **Verify flash contents:** Use `picotool verify <file.uf2>` after flashing to confirm flash integrity.
- **Erase flash:** Use `picotool erase -a` to wipe the entire flash memory before writing clean partition tables.

## Rules & Constraints
- Always include the `-f` flag when targeting a running Pico that has `pico_enable_stdio_usb` enabled so the user does not have to manually unplug the board and hold BOOTSEL.
- For RP2350 targets, verify target architecture flags and partition table requirements before suggesting `otp` write commands, as OTP writes are permanent.
