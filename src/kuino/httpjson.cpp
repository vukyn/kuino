#include "kuino/httpjson.h"
#include "kuino/diag.h"

#include <HTTPClient.h>
#include <WiFiClientSecure.h>

namespace kuino {
namespace httpjson {

bool getJson(const String &url, JsonDocument &doc, const JsonDocument *filter,
             int retries) {
  for (int attempt = 0; attempt < retries; attempt++) {
    WiFiClientSecure client;
    client.setInsecure();           // skip cert validation (hobby device)
    client.setHandshakeTimeout(15); // seconds; cold-start handshakes are slow
    HTTPClient https;
    https.setConnectTimeout(8000);
    https.setTimeout(8000);
    if (https.begin(client, url)) {
      int code = https.GET();
      KLOG("GET %s -> %d (%s) try=%d heap=%u\n", url.c_str(), code,
           HTTPClient::errorToString(code).c_str(), attempt, ESP.getFreeHeap());
      if (code >= 200 && code < 300) {
        DeserializationError err =
            filter ? deserializeJson(doc, https.getStream(),
                                     DeserializationOption::Filter(*filter))
                   : deserializeJson(doc, https.getStream());
        https.end();
        if (!err) return true;
      } else {
        https.end();
      }
    }
    delay(500); // brief backoff before retry
  }
  return false;
}

} // namespace httpjson
} // namespace kuino
