#include <M5Stack.h>
#include "WiFi.h"

const char * ssidPrefix = "TELLO-";
const char * udpAddress = "192.168.10.1";
const int udpPort = 8889;
const int resUdpPort = 8890;
WiFiUDP udp;

#define SSID_MAX_NUM 10

void setup() {
  M5.begin();

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();


  Serial.println("Setup done");
  drawWellcome();
}

void drawWellcome() {
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.println("Wellcome to");
  M5.Lcd.setTextSize(4);
  M5.Lcd.setTextColor(TFT_RED, TFT_BLUE);
  M5.Lcd.println("Tello Drone Lander");
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.println("Please Push Left Button");

}

void loop() {
  if (M5.BtnA.wasPressed()) {
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE);
    Serial.println("scan start");
    M5.Lcd.println("scan start");

    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
      //wifiが一つも見つからない FINISH
      Serial.println("not found Wifi");
      M5.Lcd.println("not found Wifi");
      delay(1000);
    } else {
      Serial.print(n);
      Serial.println(" networks found");
      M5.Lcd.print(n);
      M5.Lcd.println(" networks found");]

      for (int i = 0; i < n; ++i) {
        //ドローンのwifiかどうか
        //        try {
        if (WiFi.SSID(i).indexOf(ssidPrefix) == 0) {

          M5.Lcd.fillScreen(TFT_BLACK);
          M5.Lcd.setCursor(0, 0);
          M5.Lcd.setTextSize(2);
          M5.Lcd.setTextColor(TFT_WHITE);
          //ドローンに接続
          if (connectToWiFi(WiFi.SSID(i))) {
            //ドローンに着陸命令送信
            landExec();
          }
          else {
            continue;
          }
        }
        else {
          //wifiの情報
          //          M5.Lcd.setTextColor(TFT_WHITE);
          printWifi(i);
        }
        //      }
        //        catch (Exception e) {
        //          M5.Lcd.setTextColor(TFT_RED);
        //          Serial.print(e);
        //          M5.Lcd.print(e);
        //        }

        delay(100);
      }
    }

    // FINISH
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);
    Serial.println("FINISH!");
    M5.Lcd.println("FINISH!");
    delay(3000);
    drawWellcome();
  }
  
  M5.update();
}

void printWifi(int i) {
  Serial.print(i + 1);
  Serial.print(": ");
  Serial.print(WiFi.SSID(i));
  Serial.print(" (");
  Serial.print(WiFi.RSSI(i));
  Serial.println(")");
  //  printEncryptionType(WiFi.encryptionType(i));

  //  M5.Lcd.print(i + 1);
  //  M5.Lcd.print(": ");
  //  M5.Lcd.print(WiFi.SSID(i));
  //  M5.Lcd.print(" (");
  //  M5.Lcd.print(WiFi.RSSI(i));
  //  M5.Lcd.println(")");
}

boolean connectToWiFi(String ssid) {
  WiFi.disconnect(true);
  //  WiFi.onEvent(WiFiEvent);
  //  char * pass = "";
  WiFi.begin(ssid.c_str());
  int count = 0;
  Serial.print("connect to ");
  M5.Lcd.print("connect to ");
  Serial.println(ssid);
  M5.Lcd.println(ssid);

  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    Serial.println("Connected!");
    M5.Lcd.println("Connected!");
    return true;
  }
  Serial.println("Connection Failed!");
  M5.Lcd.println("Connection Failed!");
  return false;

  //
  //  while (true) {
  //    if (WiFi.status() == WL_CONNECTED) {
  //      Serial.println();
  //      M5.Lcd.println();
  //      Serial.println("Connected!");
  //      M5.Lcd.println("Connected!");
  //      return true;
  //    }
  //    if (count > 40) {
  //      M5.Lcd.setTextColor(TFT_RED);
  //      Serial.println();
  //      M5.Lcd.println();
  //      Serial.println("Time over");
  //      M5.Lcd.println("Time over");
  //      return false;
  //    }
  //    delay(500);
  //    Serial.print(".");
  //    M5.Lcd.print(".");
  //    count++;
  //  }
  //  return false;
}


//ドローンのwifiに接続されている状態で実行する
void landExec() {
  Serial.print(WiFi.localIP());
  M5.Lcd.print(WiFi.localIP());
  Serial.print(" to ");
  M5.Lcd.print(" to ");
  Serial.print(udpAddress);
  M5.Lcd.print(udpAddress);
  Serial.print(":");
  M5.Lcd.print(":");
  Serial.print(udpPort);
  M5.Lcd.print(udpPort);

  // 受信設定
  udp.begin(WiFi.localIP(), resUdpPort);

  // 送信
  String cmd = "command";
  udp.beginPacket(udpAddress, udpPort);
  udp.printf(cmd.c_str());
  udp.endPacket();
  Serial.println("send command");
  M5.Lcd.println("send command");

  delay(2000);

  //  udp.beginPacket(udpAddress, udpPort);
  //  cmd = "takeoff";
  //  udp.printf(cmd.c_str());// land
  //  udp.endPacket();
  //  Serial.println("send takeoff");
  //  M5.Lcd.println("send takeoff");
  //  delay(5000);

  udp.beginPacket(udpAddress, udpPort);
  cmd = "land";
  udp.printf(cmd.c_str());
  udp.endPacket();
  Serial.println("send land");
  M5.Lcd.println("send land");

  // recive wait
  for (int i = 0; i < 400; i++) {
    int recSize = udp.parsePacket();
    if (recSize > 0) {
      char receivedBuffer[255];
      int len = udp.read(receivedBuffer, 255);
      if (len > 0) {
        Serial.println(receivedBuffer);
        M5.Lcd.println(receivedBuffer);
        delay((400 - i) * 10);
        break;
      }
    }
    delay(10);
  }

  //land送信後4秒後に緊急着陸
  udp.beginPacket(udpAddress, udpPort);
  cmd = "emergency";
  udp.printf(cmd.c_str());
  udp.endPacket();
  Serial.println("send emergency");
  M5.Lcd.println("send emergency");

  // recive wait
  for (int i = 0; i < 400; i++) {
    int recSize = udp.parsePacket();
    if (recSize > 0) {
      char receivedBuffer[255];
      int len = udp.read(receivedBuffer, 255);
      if (len > 0) {
        Serial.println(receivedBuffer);
        M5.Lcd.println(receivedBuffer);
        delay((400 - i) * 10);
        break;
      }
    }
    delay(10);
  }
}


