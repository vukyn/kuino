#include "kuino/display.h"

namespace kuino {
namespace display {

String fitWidth(U8G2 &oled, const String &s, int maxW) {
  if (oled.getUTF8Width(s.c_str()) <= maxW) return s;
  int limit = maxW - oled.getUTF8Width("..");
  String out;
  int i = 0, n = s.length();
  while (i < n) {
    int j = i + 1;
    while (j < n && ((uint8_t)s[j] & 0xC0) == 0x80) j++; // skip continuation
    String cand = out + s.substring(i, j);
    if (oled.getUTF8Width(cand.c_str()) > limit) break;
    out = cand;
    i = j;
  }
  return out + "..";
}

void drawHeader(U8G2 &oled, const String &s, const uint8_t *vnFont, int width) {
  bool ascii = true;
  for (size_t i = 0; i < s.length(); i++)
    if ((uint8_t)s[i] & 0x80) { ascii = false; break; }

  const uint8_t *font = vnFont;
  if (ascii) {
    const uint8_t *ladder[] = {vnFont, u8g2_font_7x13B_tr, u8g2_font_6x12_tr,
                               u8g2_font_5x8_tr};
    font = ladder[3]; // smallest as fallback
    for (auto cand : ladder) {
      oled.setFont(cand);
      if (oled.getUTF8Width(s.c_str()) <= width) { font = cand; break; }
    }
  }
  oled.setFont(font);
  oled.drawUTF8(0, 13, fitWidth(oled, s, width).c_str());
}

void drawScroll(U8G2 &oled, int y, const String &text, const uint8_t *vnFont,
                int marqueeOffset, int width) {
  oled.setFont(vnFont);
  int w = oled.getUTF8Width(text.c_str());
  if (w <= width) {
    oled.drawUTF8((width - w) / 2, y, text.c_str()); // fits: center
  } else {
    int span = w + 24; // marquee with gap
    int x = -(marqueeOffset % span);
    oled.drawUTF8(x, y, text.c_str());
    oled.drawUTF8(x + span, y, text.c_str());
  }
}

void toast(U8G2 &oled, const uint8_t *vnFont, const uint8_t *smallFont,
           const char *line1, const char *line2) {
  oled.clearBuffer();
  oled.setFont(vnFont);
  oled.drawUTF8(0, 30, line1);
  if (line2) {
    oled.setFont(smallFont);
    oled.drawStr(0, 48, line2);
  }
  oled.sendBuffer();
}

} // namespace display
} // namespace kuino
