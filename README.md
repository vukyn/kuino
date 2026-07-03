# kuino

Shared ESP32/Arduino helpers for vukyn IoT firmware. The firmware equivalent of
the Go platform's [`kuery`](https://github.com/vukyn/kuery): reusable device code
lives here once, is versioned, and is imported — never copied per project.

## Install

Pin a tag in your project's `platformio.ini`:

```ini
lib_deps =
	https://github.com/vukyn/kuino.git#v0.1.0
	olikraus/U8g2@^2.35.30
	bblanchon/ArduinoJson@^7.2.0
```

Include modules as `#include <kuino/wifi.h>` (etc.).

## Modules

| Module | Header | API |
|--------|--------|-----|
| WiFi | `kuino/wifi.h` | `connect(ssid, pass, onTick=nullptr)` — headless STA connect, blocks until DHCP-bound; forces public DNS. `connected()`, `assocMs()`, `dhcpMs()`. Pass `onTick(frame)` to draw your own connect spinner. |
| HTTP+JSON | `kuino/httpjson.h` | `getJson(url, doc, filter=nullptr, retries=3)` — HTTPS GET + ArduinoJson (optional filtered) parse; retries with backoff. Returns true on 2xx+parsed. |
| Display | `kuino/display.h` | `fitWidth(oled, s, maxW)`, `drawHeader(oled, s, vnFont)`, `drawScroll(oled, y, text, vnFont, marqueeOffset)`, `toast(oled, vnFont, smallFont, line1, line2=nullptr)` — U8g2 text helpers; the `U8G2&` and fonts are injected. |
| Button | `kuino/button.h` | `pressed(pin, reading, stable, lastEdge, debounceMs=50)` — debounced falling-edge detector; caller owns the state vars (one set per button). |

## Diagnostics

Add `-DKUINO_DIAG` to your `build_flags` for verbose serial logs (WiFi timing,
HTTP results). Compiled out entirely otherwise.

## Development

```bash
pio ci examples/wifi_hello/wifi_hello.ino -l . -b esp32-s3-devkitc-1
pio ci examples/poll_display/poll_display.ino -l . -b esp32-s3-devkitc-1
```

Both must end in `[SUCCESS]` — the example sketches are the compile gate for the
library (firmware modules can't be unit-tested off-hardware).

> IDE clang may report false `Arduino.h not found` / undeclared-identifier
> errors — it lacks the PlatformIO ESP32 toolchain include paths. Trust
> `pio ci` / `pio run`, not the editor's clang.

## Versioning

Git tags, minor bump per change. **Keep only the 5 newest tags** (mirrors the
kuery retention rule); delete older tags local + remote after a bump. Consumers
pin `#vX.Y.Z`.
