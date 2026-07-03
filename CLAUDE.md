# CLAUDE.md — kuino

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

Shared **firmware** library `github.com/vukyn/kuino` (C++/Arduino/PlatformIO).
The **first non-Go shared lib** in the platform, and the IoT equivalent of
`kuery`: reusable device code (WiFi, HTTPS+JSON, OLED display, buttons) lives
here, versioned, imported via `lib_deps` — never duplicated per firmware repo.

**Not a Go platform service** — the platform clean-arch template, gobuild Go
presets, DI, domains, `_test.go` conventions, the kuery shared-pkg rule, and the
code-review-graph MCP DO NOT apply here. Do not build a knowledge graph for this
repo.

## Stack
- PlatformIO, Arduino framework, `espressif32` platform (ESP32-S3 primary board)
- U8g2 (OLED), ArduinoJson v7 (declared in `library.json` `dependencies`)

## Layout
```
library.json           ; PlatformIO manifest (name, version, deps)
src/kuino/*.{h,cpp}     ; modules — src/ is the include path root, so headers
                         resolve as <kuino/wifi.h>
examples/<name>/*.ino   ; compile gates (one exercising each module)
```

Modules: `wifi` (headless STA connect + timing + public-DNS override),
`httpjson` (`getJson`), `display` (U8g2 text helpers — `U8G2&`+fonts injected),
`button` (debounced `pressed`). `diag.h` provides the `KLOG` macro
(`-DKUINO_DIAG`).

## Build / verify
```bash
pio ci examples/wifi_hello/wifi_hello.ino -l . -b esp32-s3-devkitc-1
pio ci examples/poll_display/poll_display.ino -l . -b esp32-s3-devkitc-1
```
Both must end `[SUCCESS]`. This is the acceptance test — no off-hardware unit
tests (modules depend on WiFi/U8g2/HTTPClient). IDE clang shows false
`Arduino.h not found` errors (no ESP32 toolchain paths) — trust `pio ci`.

## Versioning rule
Reusable firmware code across IoT repos goes HERE (mirror of the kuery rule,
scoped to firmware): add/extend a module → commit → tag (bump minor) → bump the
`#vX.Y.Z` pin in each consumer's `platformio.ini`. **Keep only the 5 newest
tags**; delete older tags local + remote after tagging.
