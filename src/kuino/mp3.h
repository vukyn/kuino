#pragma once
#include <Arduino.h>

namespace kuino {
namespace mp3 {

// Driver for the YX5300 / YX6300 serial MP3 module (Catalex). Communicates over
// a HardwareSerial UART at 9600 baud using 8-byte command frames
// (7E FF 06 <cmd> 00 <paramH> <paramL> EF, no checksum).
class YX5300 {
public:
  // Bind to a HardwareSerial (e.g. Serial2) on the given RX/TX pins and start
  // it at `baud` (module default 9600). Call once in setup().
  void begin(HardwareSerial &serial, int rxPin, int txPin, long baud = 9600);

  void play();                    // resume / start playback
  void pause();                   // pause
  void stop();                    // stop
  void next();                    // next track
  void prev();                    // previous track
  void playIndex(uint16_t index); // play the Nth track on the medium (1-based)
  void volume(uint8_t vol);       // set volume, clamped to 0..30
  void volumeUp();
  void volumeDown();

  // Send a raw command frame with a 16-bit parameter. Public for commands not
  // wrapped above.
  void sendCmd(uint8_t cmd, uint16_t param = 0);

private:
  HardwareSerial *serial_ = nullptr;
};

} // namespace mp3
} // namespace kuino
