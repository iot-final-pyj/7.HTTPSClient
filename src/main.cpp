#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

const char* ssid = "IoT518";
const char* password = "iot123456";

String              ca = ""
    "-----BEGIN CERTIFICATE-----\n"
    "MIIETjCCAzagAwIBAgINAe5fIh38YjvUMzqFVzANBgkqhkiG9w0BAQsFADBMMSAw\n"
    "HgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMzETMBEGA1UEChMKR2xvYmFs\n"
    "U2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjAeFw0xODExMjEwMDAwMDBaFw0yODEx\n"
    "MjEwMDAwMDBaMFAxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9iYWxTaWduIG52\n"
    "LXNhMSYwJAYDVQQDEx1HbG9iYWxTaWduIFJTQSBPViBTU0wgQ0EgMjAxODCCASIw\n"
    "DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKdaydUMGCEAI9WXD+uu3Vxoa2uP\n"
    "UGATeoHLl+6OimGUSyZ59gSnKvuk2la77qCk8HuKf1UfR5NhDW5xUTolJAgvjOH3\n"
    "idaSz6+zpz8w7bXfIa7+9UQX/dhj2S/TgVprX9NHsKzyqzskeU8fxy7quRU6fBhM\n"
    "abO1IFkJXinDY+YuRluqlJBJDrnw9UqhCS98NE3QvADFBlV5Bs6i0BDxSEPouVq1\n"
    "lVW9MdIbPYa+oewNEtssmSStR8JvA+Z6cLVwzM0nLKWMjsIYPJLJLnNvBhBWk0Cq\n"
    "o8VS++XFBdZpaFwGue5RieGKDkFNm5KQConpFmvv73W+eka440eKHRwup08CAwEA\n"
    "AaOCASkwggElMA4GA1UdDwEB/wQEAwIBhjASBgNVHRMBAf8ECDAGAQH/AgEAMB0G\n"
    "A1UdDgQWBBT473/yzXhnqN5vjySNiPGHAwKz6zAfBgNVHSMEGDAWgBSP8Et/qC5F\n"
    "JK5NUPpjmove4t0bvDA+BggrBgEFBQcBAQQyMDAwLgYIKwYBBQUHMAGGImh0dHA6\n"
    "Ly9vY3NwMi5nbG9iYWxzaWduLmNvbS9yb290cjMwNgYDVR0fBC8wLTAroCmgJ4Yl\n"
    "aHR0cDovL2NybC5nbG9iYWxzaWduLmNvbS9yb290LXIzLmNybDBHBgNVHSAEQDA+\n"
    "MDwGBFUdIAAwNDAyBggrBgEFBQcCARYmaHR0cHM6Ly93d3cuZ2xvYmFsc2lnbi5j\n"
    "b20vcmVwb3NpdG9yeS8wDQYJKoZIhvcNAQELBQADggEBAJmQyC1fQorUC2bbmANz\n"
    "EdSIhlIoU4r7rd/9c446ZwTbw1MUcBQJfMPg+NccmBqixD7b6QDjynCy8SIwIVbb\n"
    "0615XoFYC20UgDX1b10d65pHBf9ZjQCxQNqQmJYaumxtf4z1s4DfjGRzNpZ5eWl0\n"
    "6r/4ngGPoJVpjemEuunl1Ig423g7mNA2eymw0lIYkN5SQwCuaifIFJ6GlazhgDEw\n"
    "fpolu4usBCOmmQDo8dIm7A9+O4orkjgTHY+GzYZSR+Y0fFukAj6KYXwidlNalFMz\n"
    "hriSqHKvoflShx8xpfywgVcvzfTO3PYkz6fiNJBonf6q8amaEsybwMbDqKWwIX7e\n"
    "SPY=\n"
    "-----END CERTIFICATE-----\n"
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDXzCCAkegAwIBAgILBAAAAAABIVhTCKIwDQYJKoZIhvcNAQELBQAwTDEgMB4G\n"
    "A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjMxEzARBgNVBAoTCkdsb2JhbFNp\n"
    "Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDkwMzE4MTAwMDAwWhcNMjkwMzE4\n"
    "MTAwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMzETMBEG\n"
    "A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\n"
    "hvcNAQEBBQADggEPADCCAQoCggEBAMwldpB5BngiFvXAg7aEyiie/QV2EcWtiHL8\n"
    "RgJDx7KKnQRfJMsuS+FggkbhUqsMgUdwbN1k0ev1LKMPgj0MK66X17YUhhB5uzsT\n"
    "gHeMCOFJ0mpiLx9e+pZo34knlTifBtc+ycsmWQ1z3rDI6SYOgxXG71uL0gRgykmm\n"
    "KPZpO/bLyCiR5Z2KYVc3rHQU3HTgOu5yLy6c+9C7v/U9AOEGM+iCK65TpjoWc4zd\n"
    "QQ4gOsC0p6Hpsk+QLjJg6VfLuQSSaGjlOCZgdbKfd/+RFO+uIEn8rUAVSNECMWEZ\n"
    "XriX7613t2Saer9fwRPvm2L7DWzgVGkWqQPabumDk3F2xmmFghcCAwEAAaNCMEAw\n"
    "DgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFI/wS3+o\n"
    "LkUkrk1Q+mOai97i3Ru8MA0GCSqGSIb3DQEBCwUAA4IBAQBLQNvAUKr+yAzv95ZU\n"
    "RUm7lgAJQayzE4aGKAczymvmdLm6AC2upArT9fHxD4q/c2dKg8dEe3jgr25sbwMp\n"
    "jjM5RcOO5LlXbKr8EpbsU8Yt5CRsuZRj+9xTaGdWPoO4zzUhw8lo/s7awlOqzJCK\n"
    "6fBdRoyV3XpYKBovHd7NADdBj+1EbddTKJd+82cEHhXXipa0095MJ6RMG3NzdvQX\n"
    "mcIfeg7jLQitChws/zyrVQ4PkX4268NXSb7hLi18YIvDQVETI53O9zJrlAGomecs\n"
    "Mx86OyXShkDOOyyGeMlhLxS67ttVb9+E7gUJTb0o2HLO02JQZR7rkpeDMdmztcpH\n"
    "WD9f\n"
    "-----END CERTIFICATE-----";

WiFiClientSecure client;

void setup(void){
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to "); Serial.println(ssid);
    Serial.print("IP address: "); Serial.println(WiFi.localIP());

    MDNS.begin("miniwifi");

    client.setCACert(ca.c_str());
}

void loop(void){
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "https://www.tukorea.ac.kr:443/tukorea/index.do")) {  // HTTP

        Serial.print("[HTTP] GET...\n");  // start connection and send HTTP header
        int httpCode = http.GET();

        if (httpCode > 0) {                // httpCode will be negative on error
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                String payload = http.getString();
                Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
        Serial.printf("[HTTP} Unable to connect\n");
    }

    delay(30000);
}
