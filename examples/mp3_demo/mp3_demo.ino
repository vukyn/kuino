#include <Arduino.h>
#include <kuino/mp3.h>

kuino::mp3::YX5300 player;

void setup() {
  Serial.begin(115200);
  player.begin(Serial2, 16, 17);  // RX=16, TX=17
  player.volume(20);
  player.play();
}

void loop() {
  delay(5000);
  player.next();
}
