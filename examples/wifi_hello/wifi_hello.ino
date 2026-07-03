#include <Arduino.h>
#include <kuino/wifi.h>

void setup() {
  Serial.begin(115200);
  kuino::wifi::connect("your-ssid", "your-pass");
  Serial.printf("online: assoc=%lums dhcp=%lums\n", kuino::wifi::assocMs(),
                kuino::wifi::dhcpMs());
}

void loop() { delay(1000); }
