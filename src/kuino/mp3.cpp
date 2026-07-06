#include "kuino/mp3.h"

namespace kuino {
namespace mp3 {

void YX5300::begin(HardwareSerial &serial, int rxPin, int txPin, long baud) {
  serial_ = &serial;
  serial.begin(baud, SERIAL_8N1, rxPin, txPin);
}

void YX5300::sendCmd(uint8_t cmd, uint16_t param) {
  if (serial_ == nullptr) return;
  uint8_t frame[8] = {0x7E,
                      0xFF,
                      0x06,
                      cmd,
                      0x00,
                      static_cast<uint8_t>((param >> 8) & 0xFF),
                      static_cast<uint8_t>(param & 0xFF),
                      0xEF};
  serial_->write(frame, 8);
}

void YX5300::play() { sendCmd(0x0D); }
void YX5300::pause() { sendCmd(0x0E); }
void YX5300::stop() { sendCmd(0x16); }
void YX5300::next() { sendCmd(0x01); }
void YX5300::prev() { sendCmd(0x02); }
void YX5300::playIndex(uint16_t index) { sendCmd(0x03, index); }

void YX5300::volume(uint8_t vol) {
  if (vol > 30) vol = 30;
  sendCmd(0x06, vol);
}

void YX5300::volumeUp() { sendCmd(0x04); }
void YX5300::volumeDown() { sendCmd(0x05); }

bool YX5300::readFrame(uint8_t &cmd, uint16_t &param, uint32_t timeoutMs) {
  if (serial_ == nullptr) return false;
  uint32_t start = millis();
  // Sync to the 0x7E start byte.
  while (millis() - start < timeoutMs) {
    if (!serial_->available()) continue;
    if (serial_->read() != 0x7E) continue;

    // Collect the rest of the frame up to the 0xEF terminator (8-byte
    // no-checksum or 10-byte checksum variants both fit in 10 bytes).
    uint8_t buf[10];
    buf[0] = 0x7E;
    int n = 1;
    while (n < 10 && millis() - start < timeoutMs) {
      if (!serial_->available()) continue;
      uint8_t b = static_cast<uint8_t>(serial_->read());
      buf[n++] = b;
      if (b == 0xEF) break;
    }
    // Accept only a structurally valid frame: >=8 bytes, version 0xFF +
    // length 0x06 header, and a 0xEF terminator. Validating the header rejects
    // misaligned reads (e.g. a 0x7E that was actually a data byte), which would
    // otherwise surface as phantom commands / spurious SD in-out events.
    if (n >= 8 && buf[1] == 0xFF && buf[2] == 0x06 && buf[n - 1] == 0xEF) {
      cmd = buf[3];
      param = (static_cast<uint16_t>(buf[5]) << 8) | buf[6];
      return true;
    }
    return false;
  }
  return false;
}

bool YX5300::query(uint8_t reqCmd, uint16_t &out, uint32_t timeoutMs) {
  if (serial_ == nullptr) return false;
  sendCmd(reqCmd);
  uint32_t start = millis();
  uint8_t cmd;
  uint16_t param;
  // Read frames until the one echoing our request arrives (skip unsolicited
  // events) or we run out of time.
  while (millis() - start < timeoutMs) {
    uint32_t remaining = timeoutMs - (millis() - start);
    if (readFrame(cmd, param, remaining) && cmd == reqCmd) {
      out = param;
      return true;
    }
  }
  return false;
}

bool YX5300::queryFileCount(uint16_t &count) { return query(0x48, count); }
bool YX5300::queryCurrentIndex(uint16_t &index) { return query(0x4C, index); }
bool YX5300::queryStatus(uint16_t &status) { return query(0x42, status); }

bool YX5300::poll(uint8_t &cmd, uint16_t &param) {
  if (serial_ == nullptr || !serial_->available()) return false;
  // A frame byte is waiting; give readFrame a short window to finish it.
  return readFrame(cmd, param, 30);
}

} // namespace mp3
} // namespace kuino
