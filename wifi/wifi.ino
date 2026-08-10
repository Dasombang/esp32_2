#include <WiFi.h>
#include <WiFiManager.h>
#include <U8g2lib.h>


// 핀 정의
#define LCD_CLK  18
#define LCD_DATA 23
#define LCD_CS   5
#define LCD_DC   21
#define LCD_RST  22

// 소프트웨어 SPI 생성자
U8G2_ST7567_JLX12864_F_4W_SW_SPI u8g2(U8G2_R0, LCD_CLK, LCD_DATA, LCD_CS, LCD_DC, LCD_RST);


void setup() {
  Serial.begin(115200);

  // 1. LCD 초기화
  u8g2.begin();
  u8g2.setContrast(160); // ★ 대비값 강제 설정
  u8g2.setFont(u8g2_font_ncenR08_tr); 

  // 2. LCD 화면 표시: 연결 대기 중 안내
  u8g2.clearBuffer();
  u8g2.drawStr(0, 15, "WiFi Status:");
  u8g2.drawStr(0, 35, "AP Mode Active");
  u8g2.drawStr(0, 50, "Connect to AP");
  u8g2.sendBuffer();

  // 3. WiFiManager 설정
  WiFiManager wifiManager;

  // AP 모드 진입 및 연결 대기
  if (!wifiManager.autoConnect("ESP32_Config_AP")) {
    Serial.println("연결 실패 또는 시간 초과. 재부팅합니다.");
    
    u8g2.clearBuffer();
    u8g2.drawStr(0, 30, "Connection Failed!");
    u8g2.drawStr(0, 50, "Rebooting...");
    u8g2.sendBuffer();
    
    delay(3000);
    ESP.restart();
  }

  // 4. Wi-Fi 연결 성공 후 실행
  Serial.println("Wi-Fi 연결 성공!");
  String ipStr = WiFi.localIP().toString();
  Serial.print("IP 주소: ");
  Serial.println(ipStr);

  // 5. LCD 화면 표시: 연결 완료 및 IP 주소 출력
  u8g2.clearBuffer();
  u8g2.drawStr(0, 15, "WiFi Connected!");
  u8g2.drawStr(0, 35, "IP Address:");
  u8g2.drawStr(0, 55, ipStr.c_str());
  u8g2.sendBuffer();
}

void loop() {
  // 메인 루프 작업 수행
}