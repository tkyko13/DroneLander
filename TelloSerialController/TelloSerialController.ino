#include "WiFi.h"

//const char * ssidPrefix = "TELLO-";
const char * udpAddress = "192.168.10.1";
const int udpPort = 8889;
const int resUdpPort = 8890;
WiFiUDP udp;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  printHelp();
  while (!serialWifi()) {
    delay(100);
  }
  Serial.write("connect");

  Serial.print(WiFi.localIP());
  Serial.print(" to ");
  Serial.print(udpAddress);
  Serial.print(":");
  Serial.print(udpPort);

  // 受信設定
  udp.begin(WiFi.localIP(), resUdpPort);

  // commandコマンド送信
  String cmd = "command";
  udp.beginPacket(udpAddress, udpPort);
  udp.printf(cmd.c_str());
  udp.endPacket();
  Serial.println("send command");

  delay(2000);
}

void loop() {
  // 受信
  int recSize = udp.parsePacket();
  if (recSize > 0) {
    char receivedBuffer[255];
    int len = udp.read(receivedBuffer, 255);
    if (len > 0) {
      Serial.println(receivedBuffer);
      Serial.write(receivedBuffer);// \nいるかなぁ
    }
  }

  // シリアルでもらったコマンド送信
  Serial.println(Serial.available());
  if (Serial.available() > 0) {
    String inputString = "";
    while (Serial.available() > 0) {
      char inChar = (char)Serial.read();
      inputString += inChar;
      if (inChar == '\n') {
        inputString.trim();
        // send udp
        udp.beginPacket(udpAddress, udpPort);
        udp.printf(cmd.c_str());
        udp.endPacket();
        Serial.println("send command");
      }
    }
  }

  delay(100);
}
