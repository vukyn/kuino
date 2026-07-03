#include "kuino/button.h"

namespace kuino {
namespace button {

bool pressed(int pin, int &reading, int &stable, unsigned long &lastEdge,
             unsigned long debounceMs) {
  int r = digitalRead(pin);
  if (r != reading) {
    reading = r;
    lastEdge = millis();
  }
  if (millis() - lastEdge > debounceMs && r != stable) {
    stable = r;
    if (stable == LOW) return true;
  }
  return false;
}

} // namespace button
} // namespace kuino
