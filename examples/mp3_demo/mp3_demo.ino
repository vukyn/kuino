#include <Arduino.h>
#include <kuino/mp3.h>

kuino::mp3::YX5300 player;

void setup() {
  Serial.begin(115200);
  player.begin(Serial2, 16, 17);  // RX=16, TX=17
  delay(500);                     // let the module boot before querying

  uint16_t count = 0;
  if (player.queryFileCount(count))
    Serial.printf("SD: %u tracks\n", count);
  else
    Serial.println("SD: not detected");

  player.volume(20);
  player.play();
}

void loop() {
  // Log unsolicited events (track finished, SD in/out).
  uint8_t cmd;
  uint16_t param;
  if (player.poll(cmd, param)) {
    if (cmd == kuino::mp3::YX5300::EVT_TRACK_FINISHED)
      Serial.printf("track %u finished\n", param);
    else if (cmd == kuino::mp3::YX5300::EVT_SD_INSERTED)
      Serial.println("SD inserted");
    else if (cmd == kuino::mp3::YX5300::EVT_SD_REMOVED)
      Serial.println("SD removed");
  }
  delay(20);
}
