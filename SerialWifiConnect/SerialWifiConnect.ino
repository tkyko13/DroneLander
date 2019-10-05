#include "WiFi.h"

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");

  printHelp();


  while (!serialWifi()) {
    delay(100);
  }
}

void loop() {
  // gonyogonyo
  Serial.println("loooop");

  delay(1000);
}
