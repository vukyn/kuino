#pragma once
#include <U8g2lib.h>

namespace kuino {
namespace display {

// Truncate UTF-8 `s` to fit `maxW` px under the font currently set on `oled`,
// appending ".." when truncated. Cuts on codepoint boundaries so multi-byte
// (e.g. Vietnamese) characters never split.
String fitWidth(U8G2 &oled, const String &s, int maxW);

// Draw `s` as a top header (baseline y=13). ASCII text steps a font ladder
// (vnFont -> 7x13B -> 6x12 -> 5x8) to fit `width`; non-ASCII always uses
// `vnFont` and truncates via fitWidth if still too wide.
void drawHeader(U8G2 &oled, const String &s, const uint8_t *vnFont,
                int width = 128);

// Draw `text` at baseline `y` using `vnFont`: centered if it fits `width`,
// otherwise marquee-scrolled by `marqueeOffset` px (caller owns/advances it).
void drawScroll(U8G2 &oled, int y, const String &text, const uint8_t *vnFont,
                int marqueeOffset, int width = 128);

// Clear the screen and show a one- or two-line toast, then send the buffer.
void toast(U8G2 &oled, const uint8_t *vnFont, const uint8_t *smallFont,
           const char *line1, const char *line2 = nullptr);

} // namespace display
} // namespace kuino
