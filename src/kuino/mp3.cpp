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

} // namespace mp3
} // namespace kuino
