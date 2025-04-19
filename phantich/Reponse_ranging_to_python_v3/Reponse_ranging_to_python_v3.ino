#include <DW1000Ng.hpp>
#include <DW1000NgUtils.hpp>
#include <DW1000NgRanging.hpp>
#include <U8g2lib.h>
// Định nghĩa chân cho DW1000------------------------
const uint8_t PIN_RST = 26; // reset pin
const uint8_t PIN_IRQ = 15; // irq pin
const uint8_t PIN_SS = 5;   // spi select pin
const uint8_t LED_PIN = 4;  // Led_pin   


// ---------------------------- XỬ LÝ NHIỄU TÍN HIỆU --------------------------------
#define FILTER_SIZE 5              // Số lượng mẫu (Có thể tăng/giảm)
#define MIN_VALID_DISTANCE 60       // Khoảng cách tối thiểu (mm)
#define MAX_VALID_DISTANCE 5000     // Khoảng cách tối đa (mm)
#define OUTLIER_THRESHOLD 100        // Ngưỡng loại bỏ nhiễu 

class DistanceFilter {
private:
    uint16_t buffer[FILTER_SIZE];
    int index = 0;
    bool isFilled = false;
    
public:
    uint16_t filter(uint16_t newDistance) {
        // Kiểm tra giá trị hợp lệ
        if (newDistance < MIN_VALID_DISTANCE || newDistance > MAX_VALID_DISTANCE) {
            return 0;
        }
        
        buffer[index] = newDistance;
        index = (index + 1) % FILTER_SIZE;
        
        if (index == 0) {
            isFilled = true;
        }
        
        if (!isFilled) {
            return newDistance;
        }
        
        // Tính trung bình có loại bỏ giá trị ngoại lai
        uint32_t sum = 0;
        uint16_t validCount = 0;
        uint16_t median = findMedian();
        
        for (int i = 0; i < FILTER_SIZE; i++) {
            if (abs(buffer[i] - median) < OUTLIER_THRESHOLD) {
                sum += buffer[i];
                validCount++;
            }
        }
        
        return validCount > 0 ? (sum / validCount) : newDistance;
    }
    
private:
    uint16_t findMedian() {
        uint16_t temp[FILTER_SIZE];
        memcpy(temp, buffer, sizeof(buffer));
        
        // Sắp xếp mảng
        for (int i = 0; i < FILTER_SIZE-1; i++) {
            for (int j = i+1; j < FILTER_SIZE; j++) {
                if (temp[i] > temp[j]) {
                    uint16_t t = temp[i];
                    temp[i] = temp[j];
                    temp[j] = t;
                }
            }
        }
        
        return temp[FILTER_SIZE/2];
    }
};
// Tạo đối tượng bộ lọc toàn cục
DistanceFilter distanceFilter;
                            
// -- -----------------------  Khai báo LCD  ------------------------------

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// -------------------------------------------------------------------------

// -- -----------Định nghĩa chân cho UART -------------------------
#define TX2_PIN 17  // GPIO17 cho TX2 
#define RX2_PIN 16  // GPIO16 cho RX2

// -- -----------Định nghĩa message types -------------------------
#define POLL 0
#define POLL_ACK 1
#define RANGE 2
#define RANGE_REPORT 3
#define RANGE_FAILED 255
// -- -----------Định nghĩa message types -------------------------                                                              
// Định nghĩa các ngưỡng (<40% là rất yếu)
   #define SIGNAL_QUALITY_EXCELLENT 0.90  // >90%
   #define SIGNAL_QUALITY_GOOD     0.75  // 75-90%
   #define SIGNAL_QUALITY_MEDIUM   0.60  // 60-75%
   #define SIGNAL_QUALITY_POOR     0.40  // 40-60% 
                      
                    // Định nghĩa các ngưỡng công suất thu  (< -88 dBm là rất yếu)
                       #define POWER_EXCELLENT -75  // > -75 dBm
                       #define POWER_GOOD     -80  // -80 to -75 dBm
                       #define POWER_MEDIUM   -85  // -85 to -80 dBm
                       #define POWER_POOR     -88  // -88 to -85 dBm

// -- ----------- Các biến cho ranging protocol ------------------
volatile byte expectedMsgId = POLL;
volatile boolean sentAck = false;
volatile boolean receivedAck = false;
boolean protocolFailed = false;

// -- ----------- Timestamps (Thời gian lấy mẫu) ------------------
uint64_t timePollSent;
uint64_t timePollReceived;
uint64_t timePollAckSent;
uint64_t timePollAckReceived;
uint64_t timeRangeSent;
uint64_t timeRangeReceived;
uint64_t timeComputedRange;

// ----------------Buffer và các thông số khác -------------------
#define LEN_DATA 16
byte data[LEN_DATA];
uint32_t lastActivity;
uint32_t resetPeriod = 250;
uint16_t replyDelayTimeUS = 3000;
uint16_t successRangingCount = 0;
uint32_t rangingCountPeriod = 0;
float samplingRate = 0;

// ---------------- Cấu hình DW1000 -------------------------------
device_configuration_t DEFAULT_CONFIG = {
    false,
    true,
    true,
    true,
    false,
    SFDMode::STANDARD_SFD,
    Channel::CHANNEL_5,
    DataRate::RATE_850KBPS,
    PulseFrequency::FREQ_16MHZ,
    PreambleLength::LEN_1024,
    PreambleCode::CODE_3
};

interrupt_configuration_t DEFAULT_INTERRUPT_CONFIG = {
    true,
    true,
    true,
    false,
    true
};

// Thêm vào phần đầu file, sau các #define hiện có
#define OBSTACLE_DETECTION_ENABLED true
#define OBSTACLE_POWER_THRESHOLD -85.0f
#define OBSTACLE_QUALITY_THRESHOLD 0.6f
#define OBSTACLE_FP_POWER_RATIO 0.6f

// Thêm lớp ObstacleDetector
class ObstacleDetector {
private:
    static const int HISTORY_SIZE = 5;
    float powerHistory[HISTORY_SIZE];
    float qualityHistory[HISTORY_SIZE];
    int historyIndex = 0;
    
public:
    struct ObstacleInfo {
        bool detected;
        float confidence;
        String reason;
    };
    
    ObstacleInfo detectObstacle(float rxPower, float quality) {
        ObstacleInfo result = {false, 0.0f, ""};
        float confidence = 0.0f;
        
        // Lưu vào lịch sử
        powerHistory[historyIndex] = rxPower;
        qualityHistory[historyIndex] = quality;
        historyIndex = (historyIndex + 1) % HISTORY_SIZE;
        
        // Kiểm tra công suất thu
        if (rxPower < OBSTACLE_POWER_THRESHOLD) {
            confidence += 0.3f;
            result.reason += "Low Power; ";
        }
        
        // Kiểm tra chất lượng tín hiệu
        if (quality < OBSTACLE_QUALITY_THRESHOLD) {
            confidence += 0.3f;
            result.reason += "Poor Quality; ";
        }
        
        // Kiểm tra biến thiên công suất
        float powerStdDev = calculateStdDev(powerHistory);
        if (powerStdDev > 2.0f) {
            confidence += 0.2f;
            result.reason += "Power Fluctuation; ";
        }
        
        // Kiểm tra biến thiên chất lượng
        float qualityStdDev = calculateStdDev(qualityHistory);
        if (qualityStdDev > 0.1f) {
            confidence += 0.2f;
            result.reason += "Quality Fluctuation; ";
        }
        
        result.detected = (confidence > 0.5f);
        result.confidence = confidence;
        
        return result;
    }
    
private:
    float calculateStdDev(float* data) {
        float sum = 0.0f;
        float sumSq = 0.0f;
        
        for (int i = 0; i < HISTORY_SIZE; i++) {
            sum += data[i];
            sumSq += data[i] * data[i];
        }
        
        float mean = sum / HISTORY_SIZE;
        float variance = (sumSq / HISTORY_SIZE) - (mean * mean);
        
        return sqrt(variance);
    }
};

// Thêm hàm hiệu chỉnh khoảng cách
uint16_t applyDistanceCorrection(uint16_t distance, float rxPower, float quality) {
    // Hiệu chỉnh dựa trên công suất thu
    float powerCorrection = 1.0f;
    if (rxPower < -80.0f) {
        powerCorrection = 1.05f; // Tăng 5% khi công suất thấp
    } else if (rxPower < -70.0f) {
        powerCorrection = 1.02f; // Tăng 2% khi công suất trung bình
    }
    
    // Hiệu chỉnh dựa trên chất lượng tín hiệu
    float qualityCorrection = 1.0f;
    if (quality < 0.7f) {
        qualityCorrection = 1.03f; // Tăng 3% khi chất lượng thấp
    } else if (quality < 0.85f) {
        qualityCorrection = 1.01f; // Tăng 1% khi chất lượng trung bình
    }
    
    return (uint16_t)(distance * powerCorrection * qualityCorrection);
}

// Tạo đối tượng toàn cục
ObstacleDetector obstacleDetector;

//--------------------------------------------* VOID SETUP *-------------------------------------------
void setup() {
    Serial.begin(115200); // Khởi tạo Serial để debug
    Serial2.begin(115200, SERIAL_8N1, RX2_PIN, TX2_PIN); // Khởi tạo Serial2 để giao tiếp với STM32
    Serial2.setRxBufferSize(256);
    Serial2.setTxBufferSize(256);
    u8g2.begin(); // Khởi tạo LCD
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr); // Chọn font chữ hiển thị
    u8g2.drawStr(0, 10, "Welc TFT,BibiTran13");
    u8g2.drawStr(0, 20, "Starting reveive ...");
    u8g2.sendBuffer(); // Gửi để hiển thị
    delay(1000);
    Serial.println(F("### DW1000Ng-arduino-ranging-anchor ###"));
    
    // Khởi tạo DW1000
    DW1000Ng::initialize(PIN_SS, PIN_IRQ, PIN_RST);
    Serial.println(F("DW1000Ng initialized ..."));
    
    // Cấu hình chung
    DW1000Ng::applyConfiguration(DEFAULT_CONFIG);
    DW1000Ng::applyInterruptConfiguration(DEFAULT_INTERRUPT_CONFIG);
    DW1000Ng::setDeviceAddress(1);
    DW1000Ng::setAntennaDelay(16420);
    
    // In thông tin thiết bị
    char msg[128];
    DW1000Ng::getPrintableDeviceIdentifier(msg);
    Serial.print("Device ID: "); Serial.println(msg);
    
    DW1000Ng::attachSentHandler(handleSent);
    DW1000Ng::attachReceivedHandler(handleReceived);
    
    // Bắt đầu nhận
    receiver();
    noteActivity();
    rangingCountPeriod = millis();
    //------------ Setup Led ------------------
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
}
//----------------Hiển thị OLED ------------------
void updateLCD(uint16_t distance, float rxPower, float quality, ObstacleDetector::ObstacleInfo obstacleInfo) {
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
    
    // Hiển thị thông tin vật cản nếu có
    if (OBSTACLE_DETECTION_ENABLED && obstacleInfo.detected) {
        u8g2.drawStr(0, 60, "Obstacle:");
        sprintf(buffer, "%.0f%%", obstacleInfo.confidence * 100);
        u8g2.drawStr(60, 60, buffer);
    } else {
        u8g2.drawStr(0, 60, "Muc tin hieu: ");
        u8g2.drawStr(85, 60, signalLevel);
    }
    
    u8g2.sendBuffer();

    // Debug trên Serial Monitor
    //Serial.print("Range: ");
    //Serial.print(distance);
    //Serial.print(" mm, Power: ");
    //Serial.print(rxPower);
    //Serial.print(" dBm, Quality: ");
    //Serial.print(quality);
    //Serial.print(", Muc tin hieu: ");
    //Serial.println(signalLevel);
}
//----------- HÀM GIAO TIẾP STM32 ------------------

void sendDataToSTM32(uint16_t value) {
    static uint32_t lastSent = 0;
    uint8_t dataPacket[5];
    
    // Tăng delay giữa các lần gửi lên 100ms
    if (millis() - lastSent < 100) {
        delay(100 - (millis() - lastSent));
    }
    
    // Đảm bảo Serial2 đã sẵn sàng
    if (!Serial2) {
        Serial.println("Serial2 not ready!");
        return;
    }
    
    // Clear input buffer
    while(Serial2.available()) {
        Serial2.read();
    }
    
    // Clear output buffer
    Serial2.flush();
    delay(10);  // Đợi buffer được clear
    
    // Chuẩn bị gói dữ liệu
    dataPacket[0] = 0x02;  // Header
    dataPacket[1] = (value >> 8) & 0xFF;  // High byte
    dataPacket[2] = value & 0xFF;         // Low byte
    dataPacket[3] = (dataPacket[0] + dataPacket[1] + dataPacket[2]) % 256;  // Checksum
    dataPacket[4] = 0x03;  // End byte
    
    // Gửi từng byte một với delay nhỏ
    for(int i = 0; i < 5; i++) {
        //Serial2.write(dataPacket[i]);
        delayMicroseconds(500);  // 500us delay giữa các byte
    }
    
    // Đảm bảo dữ liệu được gửi hết
    Serial2.flush();
    delay(10);
    // Debug
    Serial.print("Sent distance: ");
    Serial.print(value);
    Serial.print(" mm [");
    for(int i = 0; i < 5; i++) {
        Serial.print("0x");
        Serial.print(dataPacket[i], HEX);
        Serial.print(" ");
    }
    Serial.println("]");
    
    lastSent = millis();
}
// --------------------Các hàm hỗ trợ-------------------------------
void noteActivity() { lastActivity = millis(); }
void resetInactive() {
    expectedMsgId = POLL;
    receiver();
    noteActivity();
}
void handleSent() { sentAck = true; }
void handleReceived() { receivedAck = true; }
void receiver() {
    DW1000Ng::forceTRxOff();
    DW1000Ng::startReceive();
}

// Các hàm transmit
void transmitPollAck() {
    data[0] = POLL_ACK;
    DW1000Ng::setTransmitData(data, LEN_DATA);
    DW1000Ng::startTransmit();
}

void transmitRangeReport(float curRange) {
    data[0] = RANGE_REPORT;
    memcpy(data + 1, &curRange, 4);
    DW1000Ng::setTransmitData(data, LEN_DATA);
    DW1000Ng::startTransmit();
}

void transmitRangeFailed() {
    data[0] = RANGE_FAILED;
    DW1000Ng::setTransmitData(data, LEN_DATA);
    DW1000Ng::startTransmit();
}
void setupLED() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);  // Tắt LED khi khởi động
}
void ledBlink() {
    digitalWrite(LED_PIN, HIGH);
    delay(100); 
    digitalWrite(LED_PIN, LOW);
    delay(100);
}
//--------------------------------------------* VOID LOOP *-------------------------------------------
void loop() {
    int32_t curMillis = millis();
    
    if (!sentAck && !receivedAck) {
        if (curMillis - lastActivity > resetPeriod) {
            resetInactive();
        }
        return;
    }
    
    if (sentAck) {
        sentAck = false;
        byte msgId = data[0];
        if (msgId == POLL_ACK) {
            timePollAckSent = DW1000Ng::getTransmitTimestamp();
            noteActivity();
        }
        DW1000Ng::startReceive();
    }
    
    if (receivedAck) {
        receivedAck = false;
        DW1000Ng::getReceivedData(data, LEN_DATA);
        byte msgId = data[0];
        
        if (msgId != expectedMsgId) {
            protocolFailed = true;
        }
        
        if (msgId == POLL) {
            protocolFailed = false;
            timePollReceived = DW1000Ng::getReceiveTimestamp();
            expectedMsgId = RANGE;
            transmitPollAck();
            noteActivity();
        }
    else if (msgId == RANGE) {
        timeRangeReceived = DW1000Ng::getReceiveTimestamp();
        expectedMsgId = POLL;
        
        if (!protocolFailed) {
            timePollSent = DW1000NgUtils::bytesAsValue(data + 1, LENGTH_TIMESTAMP);
            timePollAckReceived = DW1000NgUtils::bytesAsValue(data + 6, LENGTH_TIMESTAMP);
            timeRangeSent = DW1000NgUtils::bytesAsValue(data + 11, LENGTH_TIMESTAMP);
            
            // Tính khoảng cách
            double distance = DW1000NgRanging::computeRangeAsymmetric(
                timePollSent, timePollReceived, 
                timePollAckSent, timePollAckReceived,
                timeRangeSent, timeRangeReceived
            );
            
            // Hiệu chỉnh khoảng cách
            distance = DW1000NgRanging::correctRange(distance);
            
            // Chuyển đổi sang mm và áp dụng bộ lọc
            uint16_t rawDistance = (uint16_t)((distance * 1000)- 200);
            uint16_t filteredDistance = distanceFilter.filter(rawDistance);
            
            // Chỉ xử lý khi có khoảng cách hợp lệ
            if (filteredDistance > 0) {
                float rxPower = DW1000Ng::getReceivePower();
                float receiveQuality = DW1000Ng::getReceiveQuality();
                
                // Phát hiện vật cản
                ObstacleDetector::ObstacleInfo obstacleInfo;
                if (OBSTACLE_DETECTION_ENABLED) {
                    obstacleInfo = obstacleDetector.detectObstacle(rxPower, receiveQuality);
                }
                
                // Hiệu chỉnh khoảng cách
                uint16_t correctedDistance = applyDistanceCorrection(filteredDistance, rxPower, receiveQuality);
                
                // Hiển thị và gửi dữ liệu
                ledBlink();
                sendDataToSTM32(correctedDistance);
                updateLCD(correctedDistance, rxPower, receiveQuality, obstacleInfo);
                
                // Debug thông tin
                Serial.print("Raw: ");
                Serial.print(rawDistance);
                Serial.print(" mm, Filtered: ");
                Serial.print(filteredDistance);
                Serial.print(" mm, Corrected: ");
                Serial.print(correctedDistance);
                Serial.print(" mm, Power: ");
                Serial.print(rxPower);
                Serial.print(" dBm, Quality: ");
                Serial.println(receiveQuality);
                
                if (OBSTACLE_DETECTION_ENABLED && obstacleInfo.detected) {
                    Serial.print("Obstacle detected! Confidence: ");
                    Serial.print(obstacleInfo.confidence * 100);
                    Serial.print("%, Reason: ");
                    Serial.println(obstacleInfo.reason);
                }
                
                transmitRangeReport(distance * DISTANCE_OF_RADIO_INV);
                successRangingCount++;
                if (curMillis - rangingCountPeriod > 1000) {
                    samplingRate = (1000.0f * successRangingCount) / (curMillis - rangingCountPeriod);
                    rangingCountPeriod = curMillis;
                    successRangingCount = 0;
                }
              }
            } else {
                
                transmitRangeFailed();
                // Hiển thị lỗi trên OLED
                u8g2.clearBuffer();
                u8g2.setFont(u8g2_font_ncenB08_tr);
                u8g2.drawStr(0, 32, "Range Failed!");
                u8g2.sendBuffer();
             }
          noteActivity();
       }
   }
}
