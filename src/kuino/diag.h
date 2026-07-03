#pragma once
#include <Arduino.h>

// Verbose serial diagnostics, compiled out entirely unless -DKUINO_DIAG is set
// in the consumer's build_flags. Mirrors rainybox's DLOG.
#ifdef KUINO_DIAG
  #define KLOG(...) Serial.printf(__VA_ARGS__)
#else
  #define KLOG(...) do {} while (0)
#endif
