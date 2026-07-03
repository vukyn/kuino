#pragma once
#include <Arduino.h>

namespace kuino {
namespace wifi {

// Connect in STA mode to ssid/pass and block until associated + DHCP-bound.
// Tunes the radio for fast/reliable association, forces public DNS
// (8.8.8.8 / 1.1.1.1) while keeping the DHCP IP/gateway, and never persists
// creds to NVS. Headless: draws nothing. Pass an optional `onTick` callback to
// render a spinner — it is invoked once per ~90ms wait iteration with an
// incrementing frame counter. Timing is logged under -DKUINO_DIAG.
void connect(const char *ssid, const char *pass,
             void (*onTick)(int frame) = nullptr);

// True when the station interface is connected.
bool connected();

// Association time (radio linked) and DHCP time (IP obtained) in ms from the
// most recent connect() — 0 until measured. For diagnostics/on-screen timing.
unsigned long assocMs();
unsigned long dhcpMs();

} // namespace wifi
} // namespace kuino
