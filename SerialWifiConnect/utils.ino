// example setup
//#include "WiFi.h"
//
//void setup() {
//  Serial.begin(115200);
//
//  WiFi.mode(WIFI_STA);
//  WiFi.disconnect();
//  delay(100);
//
//  printHelp();
//  while (!serialWifi()) {
//    delay(100);
//  }
//}





bool serialWifi() {
  String inputString = "";
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      inputString.trim();
//      Serial.println(inputString);

      if (inputString == "@") {
        scanWifi();
      }
      else if (inputString == "!") {
        printHelp();
      }
      else {
        int i = inputString.indexOf(":");
        if (i < 0) {
          return connectToWiFi(inputString, "");
        }
        else {
          return connectToWiFi(inputString.substring(0, i), inputString.substring(i + 1));
        }
      }
    }
  }
  return false;
}

void printHelp() {
  Serial.println("---------");
  Serial.println("[!] - help");
  Serial.println("[@] - scan wifi");
  Serial.println("['your_ssid':'your_password'] - connect wifi");
}

void scanWifi() {
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
}

boolean connectToWiFi(String ssid, String pass) {
  WiFi.disconnect(true);
  WiFi.begin(ssid.c_str(), pass.c_str());
  int count = 0;
  Serial.print("connect to ");
  Serial.println(ssid);

  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    Serial.println("Connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  }
  Serial.println("Connection Failed!");
  return false;
}
