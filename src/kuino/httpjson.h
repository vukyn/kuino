#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

namespace kuino {
namespace httpjson {

// HTTPS GET `url` and deserialize the body into `doc`. When `filter` is non-null
// it is applied as an ArduinoJson deserialization filter (parses only wanted
// fields, keeping RAM small). Returns true on a 2xx response that parsed
// cleanly. Retries up to `retries` times with a 500ms backoff — fly.dev
// cold-starts can EOF the TLS handshake and hotspot DNS can transiently miss,
// both of which usually succeed on retry. Certificate validation is skipped
// (setInsecure) — intended for hobby devices, not sensitive endpoints.
bool getJson(const String &url, JsonDocument &doc,
             const JsonDocument *filter = nullptr, int retries = 3);

} // namespace httpjson
} // namespace kuino
