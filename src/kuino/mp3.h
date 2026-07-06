#pragma once
#include <Arduino.h>

namespace kuino {
namespace mp3 {

// Driver for the YX5300 / YX6300 serial MP3 module (Catalex). Communicates over
// a HardwareSerial UART at 9600 baud using 8-byte command frames
// (7E FF 06 <cmd> 00 <paramH> <paramL> EF, no checksum). Command sends are
// fire-and-forget; query()/poll() read the module's response/event frames.
class YX5300 {
public:
  // Unsolicited event command bytes the module emits on its own (read via
  // poll()). Values follow the common Catalex/YX5300 protocol.
  static constexpr uint8_t EVT_SD_INSERTED    = 0x3A; // TF card plugged in
  static constexpr uint8_t EVT_SD_REMOVED     = 0x3B; // TF card pulled out
  static constexpr uint8_t EVT_TRACK_FINISHED = 0x3D; // param = finished index

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

  // --- reading the module (query + events) ------------------------------
  // Read one response/event frame within `timeoutMs`. On success sets `cmd`
  // (the frame's command byte) and `param` (its 16-bit data) and returns true.
  // Tolerates both 8-byte (no-checksum) and 10-byte (checksum) frame variants.
  bool readFrame(uint8_t &cmd, uint16_t &param, uint32_t timeoutMs = 200);

  // Send request command `reqCmd` and wait for the matching response frame,
  // returning its 16-bit value in `out`. Unsolicited frames arriving meanwhile
  // are skipped. Returns false on timeout (e.g. no module / no SD card).
  bool query(uint8_t reqCmd, uint16_t &out, uint32_t timeoutMs = 250);

  bool queryFileCount(uint16_t &count);   // total tracks on the TF card (0x48)
  bool queryCurrentIndex(uint16_t &index); // current track index (0x4C)
  bool queryStatus(uint16_t &status);      // playback status (0x42)

  // Non-blocking: if a complete frame is already waiting, read it into
  // `cmd`/`param` and return true. Call each loop() to catch EVT_* events.
  bool poll(uint8_t &cmd, uint16_t &param);

private:
  HardwareSerial *serial_ = nullptr;
};

} // namespace mp3
} // namespace kuino
