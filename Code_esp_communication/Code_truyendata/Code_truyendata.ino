#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const int LED_PIN = 2;
ESP8266WiFiMulti WiFiMulti;

// Cấu hình mạng WiFi
const char* SSID = "TranBaoHoanHao";
const char* PASSWORD = "04082003";

// Thời gian timeout để tắt LED khi không nhận được dữ liệu
const unsigned long TIMEOUT_MS = 100;
unsigned long lastDataTime = 0; // Thời gian cuối cùng nhận dữ liệu

uint16_t value1 = 0; // Biến toàn cục để lưu giá trị value1
uint16_t value2 = 0; // Biến toàn cục để lưu giá trị value2
uint16_t value3 = 0; // Biến toàn cục để lưu giá trị value3 
void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.begin(115200);
  //Serial.println("\n[SETUP] Khởi động ESP8266...");

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID, PASSWORD);

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
  }
  //Serial.println("[SETUP] Kết nối WiFi thành công!");
}

void handleUART() {
  if (Serial.available() >= 7) {
    uint8_t startByte = Serial.read();
    //Serial.print("Đã vô hàm truyền uart");
    if (startByte == 0x02) {
      uint8_t high1 = Serial.read();
      uint8_t low1 = Serial.read();
      uint8_t high2 = Serial.read();
      uint8_t low2 = Serial.read();
      uint8_t high3 = Serial.read();
      uint8_t low3 = Serial.read();
      value1 = (high1 << 8) | low1;
      value2 = (high2 << 8) | low2;
      value3 = (high3 << 8) | low3;
      //Serial.printf("[UART] Nhận được value1: %d, value2: %d, value3:%d\n", value1, value2, value3);

      lastDataTime = millis();
      digitalWrite(LED_PIN, LOW);

      // Gửi dữ liệu đến ESP8266 thứ 2
      sendHTTP(value1, value2, value3);
    }
  }

  if (millis() - lastDataTime > TIMEOUT_MS) {
    digitalWrite(LED_PIN, HIGH);
  }
}

void sendHTTP(uint16_t value1, uint16_t value2, uint16_t value3) {
  if (WiFiMulti.run() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // Xây dựng URL với tham số giá trị
    String url = "http://192.168.4.1/DATA?value1=" + String(value1) + "&value2=" + String(value2)+ "&value3=" + String(value3);

    if (http.begin(client, url)) {
      int httpCode = http.GET(); // Gửi GET request

      if (httpCode > 0) {
        //Serial.printf("[HTTP] Mã phản hồi: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          //Serial.printf("[HTTP] Nội dung phản hồi: %s\n", payload.c_str());
        }
      } else {
        //Serial.printf("[HTTP] Lỗi: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      //Serial.println("[HTTP] Không thể kết nối");
    }
  } else {
    //Serial.println("[HTTP] WiFi không kết nối");
  }
}

void loop() {
  handleUART();
  delay(100);
}
