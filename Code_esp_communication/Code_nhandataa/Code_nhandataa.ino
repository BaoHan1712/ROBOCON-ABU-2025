#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Cấu hình máy chủ web
ESP8266WebServer server(80);
uint16_t value1 = 0; 
uint16_t value2 = 0; 
uint16_t value3 = 0;
// Cấu hình chân LED
const int led = 2;

void setup() {
  // Cấu hình LED
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH); 

  // Cấu hình UART
  Serial.begin(115200);

  // Cấu hình Wi-Fi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP("TranBaoHoanHao", "04082003");

  // Cấu hình các endpoint cho máy chủ web
  server.on("/DATA", []() {
    if (server.hasArg("value1") && server.hasArg("value2") && server.hasArg("value3")) {
      // Lấy dữ liệu từ request và chuyển đổi sang số nguyên
      value1 = server.arg("value1").toInt();
      value2 = server.arg("value2").toInt();
      value3 = server.arg("value3").toInt();
      //Serial.printf("[SERVER] Nhận dữ liệu: value1=%d, value2=%d, value3=%d\n", value1, value2, value3);
  
      // Bật LED để chỉ thị dữ liệu được nhận
      digitalWrite(led, LOW); 
      delay(500);
      digitalWrite(led, HIGH); 
  
      server.send(200, "text/plain", "Dữ liệu đã nhận");
    } else {
      server.send(400, "text/plain", "Thiếu tham số");
    }
  });

  server.on("/OFFBAO", []() {
    digitalWrite(led, HIGH); // Tắt LED
    server.send(200, "text/plain", "off led");
  });

  // Khởi động máy chủ
  server.begin();
}

void xuLyVaTruyen(uint16_t distance1, uint16_t distance2, uint16_t distance3) {
  uint8_t packet[7];
  packet[0] = 0x02;                  // Start byte
  packet[1] = (distance1 >> 8);      // High byte distance1
  packet[2] = distance1;             // Low byte distance1
  packet[3] = (distance2 >> 8) ;     // High byte distance2
  packet[4] = distance2 ;            // Low byte distance2 
  packet[5] = (distance3 >> 8) ;     // High byte distance3
  packet[6] = distance3 ;            // Low byte distance3
  // Gửi mảng xuống STM32
  Serial.write(packet, 7);
  //Serial.println("[DEBUG] Gửi dữ liệu xuống STM32:");
  //Serial.printf("Start Byte: %02X, Distance1: %d, Distance2: %d, Distance3: %d\n", packet[0], distance1, distance2, distance3);
}

void loop() {
  // Xử lý các yêu cầu của máy chủ web  
  server.handleClient();

  // Truyền dữ liệu xuống STM32 nếu dữ liệu đã được cập nhật
  xuLyVaTruyen(value1, value2, value3);
}
