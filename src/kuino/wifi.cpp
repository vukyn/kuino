#include "kuino/wifi.h"
#include "kuino/diag.h"

#include <WiFi.h>

namespace kuino {
namespace wifi {

namespace {
// Connect timing (ms since begin) — separates association vs DHCP.
volatile unsigned long tBegin = 0, tAssoc = 0, tGotIP = 0;
volatile int lastReason = 0; // last STA_DISCONNECTED reason code
} // namespace

void connect(const char *ssid, const char *pass, void (*onTick)(int frame)) {
  // persistent(false): never write creds/static-config to NVS. Persisting a
  // static IP/DNS previously caused reason-2 (AUTH_EXPIRE) loops that survived
  // reboots and network changes.
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);                          // no modem sleep during connect
  WiFi.setTxPower(WIFI_POWER_19_5dBm);           // max TX (helps the handshake)
  WiFi.setScanMethod(WIFI_ALL_CHANNEL_SCAN);     // see every AP...
  WiFi.setSortMethod(WIFI_CONNECT_AP_BY_SIGNAL); // ...join the strongest

  WiFi.onEvent([](arduino_event_id_t e) { tAssoc = millis(); },
               ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent([](arduino_event_id_t e) { tGotIP = millis(); },
               ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(
      [](arduino_event_id_t e, arduino_event_info_t info) {
        lastReason = info.wifi_sta_disconnected.reason;
        KLOG("\nWiFi disconnect reason=%d\n", lastReason);
      },
      ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  tBegin = millis();
  WiFi.begin(ssid, pass);

  int frame = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (onTick) onTick(frame);
    KLOG(".");
    frame++;
    delay(90);
  }

  // Force public DNS (keep DHCP IP/gateway). Router/hotspot DNS that fails to
  // resolve surfaces as WiFiClientSecure "start_ssl_client: -1".
  WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(),
              IPAddress(8, 8, 8, 8), IPAddress(1, 1, 1, 1));

  KLOG("\nWiFi ok: %s | assoc=%lums dhcp=%lums total=%lums\n",
       WiFi.localIP().toString().c_str(), assocMs(), dhcpMs(),
       (tGotIP > tBegin) ? tGotIP - tBegin : 0);
}

bool connected() { return WiFi.status() == WL_CONNECTED; }

unsigned long assocMs() { return (tAssoc > tBegin) ? tAssoc - tBegin : 0; }
unsigned long dhcpMs() { return (tGotIP > tAssoc) ? tGotIP - tAssoc : 0; }

} // namespace wifi
} // namespace kuino
