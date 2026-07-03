#pragma once
#include <Arduino.h>

namespace kuino {
namespace button {

// One debounced falling-edge (press) detector. The caller owns the three state
// variables — declare one set per physical button, initialising reading/stable
// to HIGH and lastEdge to 0. Returns true exactly once per debounced HIGH->LOW
// transition (active-low button to GND with INPUT_PULLUP).
bool pressed(int pin, int &reading, int &stable, unsigned long &lastEdge,
             unsigned long debounceMs = 50);

} // namespace button
} // namespace kuino
