#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <kuino/wifi.h>
#include <kuino/httpjson.h>
#include <kuino/display.h>
#include <kuino/button.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);
#define FONT_VN u8g2_font_unifont_t_vietnamese2
#define FONT_SMALL u8g2_font_6x12_tr

int btnReading = HIGH, btnStable = HIGH;
unsigned long btnEdge = 0;
int marquee = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  oled.begin();
  pinMode(4, INPUT_PULLUP);
  kuino::wifi::connect("your-ssid", "your-pass");
}

void loop() {
  if (kuino::button::pressed(4, btnReading, btnStable, btnEdge))
    kuino::display::toast(oled, FONT_VN, FONT_SMALL, ">> pressed");

  JsonDocument doc;
  if (kuino::httpjson::getJson(
          "https://rainy.fly.dev/api/v1/public/stations/live", doc)) {
    oled.clearBuffer();
    kuino::display::drawHeader(oled, "kuino demo", FONT_VN);
    kuino::display::drawScroll(oled, 48, "streaming ok", FONT_VN, marquee);
    oled.sendBuffer();
  }
  marquee += 1;
  delay(40);
}
