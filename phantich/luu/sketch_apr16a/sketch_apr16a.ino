#include <SPI.h>
#include "DW1000Ranging.h"
#include "DW1000.h"
#include <U8g2lib.h>

// Số lượng mẫu để tính trung bình khoảng cách
#define NUMBER_OF_DISTANCES 10

// Các thông số cho bộ lọc
#define MAX_VALID_DISTANCE 6.0  // Khoảng cách tối đa hợp lệ (mét)
#define MIN_VALID_DISTANCE 0.1   // Khoảng cách tối thiểu hợp lệ (mét)

// Cấu trúc cho bảng hiệu chỉnh Adelay
struct AdelayCalibration {
    float distance;  // Khoảng cách (mét)
    uint16_t adelay; // Giá trị Adelay tương ứng
};

// Bảng hiệu chỉnh Adelay cho các khoảng cách
const AdelayCalibration ADELAY_TABLE[] = {
    {1.0, 16541}, // 1m
    {1.5, 16549}, // 1.5m
    {2.0, 16532}, // 2m
    {2.5, 16534}, // 2.5m
    {3.0, 16542}, // 3m
    {3.5, 16558}, // 3.5m
    {4.0, 16553}, // 4m
    {4.5, 16544}, // 4.5m
    {5.0, 16554}  // 5m
};

const int NUM_CALIBRATIONS = sizeof(ADELAY_TABLE) / sizeof(ADELAY_TABLE[0]);

// Biến lưu giá trị đo trước đó
float prev_distance = 0;
bool first_measurement = true;

// leftmost two bytes below will become the "short address"
char anchor_addr[] = "84:00:5B:D5:A9:9A:E2:9C"; //#4
 
//calibrated Antenna Delay setting for this anchor
uint16_t Adelay = 16542;
 
// previously determined calibration results for antenna delay
// #1 16630
// #2 16610
// #3 16607
// #4 16580
 
// calibration distance
float dist_m = 1; //meters
 
#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4
 
// connection pins
const uint8_t PIN_RST = 26; // reset pin
const uint8_t PIN_IRQ = 15; // irq pin
const uint8_t PIN_SS = 5;   // spi select pin

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
void updateLCD(uint16_t distance, float rxPower, float quality) {
    char buffer[32];
    char signalLevel[8];  // Buffer ngắn hơn cho trạng thái tín hiệu
    
    // Xác định mức tín hiệu dựa trên Power và Quality
    if (rxPower > -60) {
        strcpy(signalLevel, "Qua gan"); 
    }
    else if (rxPower > -75 && quality > 0.90) {
        strcpy(signalLevel, "Tot");   
    }
    else if (rxPower > -80 && quality > 0.75) {
        strcpy(signalLevel, "Tuong doi");     
    }
    else if (rxPower > -85 && quality > 0.60) {
        strcpy(signalLevel, "Tru bi");       
    }
    else if (rxPower > -88 && quality > 0.40) {
        strcpy(signalLevel, "Kem");    
    }
    else {
        strcpy(signalLevel, "Yeu");     
    }

    // Hiển thị trên OLED
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    
    // Hiển thị Distance
    u8g2.drawStr(0, 12, "Range:");
    sprintf(buffer, "%d mm", distance);
    u8g2.drawStr(45, 12, buffer);
    
    // Hiển thị Power
    u8g2.drawStr(0, 28, "Power:");
    sprintf(buffer, "%+.1f dBm", rxPower);
    u8g2.drawStr(45, 28, buffer);
    
    // Hiển thị Quality
    u8g2.drawStr(0, 44, "Quality:");
    sprintf(buffer, "%.2f", quality);
    u8g2.drawStr(46, 44, buffer);
    
    // Hiển thị mức tín hiệu
    u8g2.drawStr(0, 60, "Muc tin hieu: ");
    u8g2.drawStr(85, 60, signalLevel);
    
    u8g2.sendBuffer();
}
void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Anchor config and start");
    Serial.print("Antenna delay ");
    Serial.println(Adelay);
    Serial.print("Calibration distance ");
    Serial.println(dist_m);
    
    // Khởi tạo OLED
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 10, "Welc TFT, BibiTran13");
    u8g2.drawStr(0, 20, "Starting reveive ...");
    u8g2.sendBuffer();
    delay(1000);
    
    //init the configuration
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ);
    
    // Cấu hình cơ bản cho DW1000
    DW1000.setDefaults();
    DW1000.setChannel(5);            // Channel 5
    DW1000.setPreambleLength(DW1000.TX_PREAMBLE_LEN_2048);  // Tăng độ dài preamble
    DW1000.setPreambleCode(9);       // Mã preamble cho 64MHz
    DW1000.useSmartPower(true);      // Smart power
    
    // set antenna delay for anchors only
    DW1000.setAntennaDelay(Adelay);
    
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.attachNewDevice(newDevice);
    DW1000Ranging.attachInactiveDevice(inactiveDevice);
    
    //start the module as an anchor
    DW1000Ranging.startAsAnchor(anchor_addr, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);
}
 
void loop()
{
  DW1000Ranging.loop();
}
 
// Hàm lọc khoảng cách
float filterDistance(float raw_distance) {
    // Kiểm tra giá trị hợp lệ
    if (raw_distance < MIN_VALID_DISTANCE || raw_distance > MAX_VALID_DISTANCE) {
        return prev_distance; // Giữ nguyên giá trị cũ nếu giá trị mới không hợp lệ
    }
    
    // Nếu là lần đo đầu tiên
    if (first_measurement) {
        first_measurement = false;
        prev_distance = raw_distance;
        return raw_distance;
    }
    
    // Cập nhật giá trị trước đó và trả về giá trị mới
    prev_distance = raw_distance;
    return raw_distance;
}

// Hàm tìm giá trị Adelay phù hợp cho khoảng cách
uint16_t findBestAdelay(float distance) {
    // Phân loại khoảng cách và trả về giá trị Adelay tương ứng
    if (distance <= 1.0) {
        return 16541;  // Dưới 1m
    }
    else if (distance <= 1.5) {
        return 16549;  // 1m - 1.5m
    }
    else if (distance <= 2.0) {
        return 16532;  // 1.5m - 2m
    }
    else if (distance <= 2.5) {
        return 16534;  // 2m - 2.5m
    }
    else if (distance <= 3.0) {
        return 16542;  // 2.5m - 3m
    }
    else if (distance <= 3.5) {
        return 16558;  // 3m - 3.5m
    }
    else if (distance <= 4.0) {
        return 16553;  // 3.5m - 4m
    }
    else if (distance <= 4.5) {
        return 16544;  // 4m - 4.5m
    }
    else {
        return 16554;  // Trên 4.5m
    }
}

// Định nghĩa các byte đặc biệt cho giao thức UART
#define START_BYTE 0xAA
#define END_BYTE 0x55

// Hàm tính checksum
uint8_t calculateChecksum(uint8_t high_byte, uint8_t low_byte) {
    return START_BYTE ^ high_byte ^ low_byte ^ END_BYTE;
}

// Hàm gửi dữ liệu theo giao thức an toàn
void sendUARTData(uint16_t value) {
    uint8_t high_byte = (value >> 8) & 0xFF;
    uint8_t low_byte = value & 0xFF;
    uint8_t checksum = calculateChecksum(high_byte, low_byte);
    
    Serial.write(START_BYTE);
    Serial.write(high_byte);
    Serial.write(low_byte);
    Serial.write(checksum);
    Serial.write(END_BYTE);
}

void newRange() {
    // Mảng lưu các mẫu hợp lệ
    float valid_samples[NUMBER_OF_DISTANCES];
    int valid_count = 0;
    
    // Thu thập các mẫu hợp lệ
    for (int i = 0; i < NUMBER_OF_DISTANCES; i++) {
        float sample = DW1000Ranging.getDistantDevice()->getRange();
        if (sample >= MIN_VALID_DISTANCE && sample <= MAX_VALID_DISTANCE) {
            valid_samples[valid_count] = sample;
            valid_count++;
        }
    }
    
    // Chỉ tính nếu có đủ mẫu hợp lệ
    if (valid_count > NUMBER_OF_DISTANCES/2) {
        // Sắp xếp mảng để tính trung vị
        for (int i = 0; i < valid_count - 1; i++) {
            for (int j = i + 1; j < valid_count; j++) {
                if (valid_samples[i] > valid_samples[j]) {
                    float temp = valid_samples[i];
                    valid_samples[i] = valid_samples[j];
                    valid_samples[j] = temp;
                }
            }
        }
        
        // Tính trung vị
        float dist;
        if (valid_count % 2 == 0) {
            dist = (valid_samples[valid_count/2 - 1] + valid_samples[valid_count/2]) / 2.0;
        } else {
            dist = valid_samples[valid_count/2];
        }
        
        // Áp dụng bộ lọc
        dist = filterDistance(dist);
        
        // Cập nhật Adelay dựa trên khoảng cách
        uint16_t newAdelay = findBestAdelay(dist);
        DW1000.setAntennaDelay(newAdelay);
        
        // Chuyển đổi khoảng cách sang mm và làm tròn
        uint16_t distance_mm = (uint16_t)(round(dist * 1000)-200);
        
        // Lấy các thông số về chất lượng tín hiệu
        float rxPower = DW1000Ranging.getDistantDevice()->getRXPower();
        float quality = DW1000Ranging.getDistantDevice()->getQuality();
        
        // Hiển thị lên OLED
        updateLCD(distance_mm, rxPower, quality);
        
        // Gửi dữ liệu qua UART theo giao thức an toàn
        sendUARTData(distance_mm);
        
        // Thêm delay nhỏ để tránh nghẽn UART
        delay(100);
    }
}
 
void newDevice(DW1000Device *device)
{
  Serial.print("Device added: ");
  Serial.println(device->getShortAddress(), HEX);
}
 
void inactiveDevice(DW1000Device *device) {
    Serial.print("Delete inactive device: ");
    Serial.println(device->getShortAddress(), HEX);
    //showNoConnection();
}