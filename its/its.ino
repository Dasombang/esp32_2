#include <Arduino.h>
#include <U8g2lib.h>


// 핀 정의
#define LCD_CLK  18
#define LCD_DATA 23
#define LCD_CS   5
#define LCD_DC   21
#define LCD_RST  22

// 소프트웨어 SPI 생성자
U8G2_ST7567_JLX12864_F_4W_SW_SPI u8g2(U8G2_R0, LCD_CLK, LCD_DATA, LCD_CS, LCD_DC, LCD_RST);


#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32C3
// ESP32-S 및 C 시리즈의 경우 내부 온도 센서 활성화 필요
#include "driver/temp_sensor.h"
#endif

void setup() {
  Serial.begin(115200);

  // 1. LCD 초기화
  u8g2.begin();
  u8g2.setContrast(160); // ★ 대비값 강제 설정
  // u8g2.setFont(u8g2_font_ncenR08_tr); 
  u8g2.setFont(u8g2_font_5x8_tf); 

  delay(1000);


  Serial.println("\nESP32 Internal Temperature Sensor Test");

  #if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32C3
    // S 시리즈 및 C 시리즈 초기화 설정
    temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor.dac = TSENS_DAC_L2; // 측정 범위 설정 (L2: -10°C ~ 80°C)
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
  #elif defined(ARDUINO_ARCH_ESP32) && !defined(CONFIG_IDF_TARGET_ESP32)
    // 기타 아키텍처 대응
  #else
    // 오리지널 ESP32(ESP32-D0WD 등)의 경우 센서가 기본 내장되어 있으나
    // 하드웨어 특성상 오차가 매우 크거나 실리콘 칩 온도만 측정되므로 아래 리얼타임 함수 사용
  #endif
}

void loop() {
  float temperature = 0;

  #if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32C3
    // S 및 C 계열 온도 읽기 (섭씨)
    temp_sensor_read_celsius(&temperature);
  #else
    // 오리지널 ESP32 내부 온도 센서 읽기 함수 (아두이노 코어 기본 제공)
    // 주의: ESP32 전통 모델의 내부 센서는 값이 정확하지 않고 칩 자체의 발열을 반영하므로 참고용으로만 사용됩니다.
    temperature = temperatureRead();
  #endif

  Serial.print("Internal Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  char tempBuffer[32];
  sprintf(tempBuffer, "%.1f\xb0" "C", temperature);
  u8g2.clearBuffer();

  // 제목
  const char* title = "Internal Temperature";
  int titleWidth = u8g2.getStrWidth(title);
  u8g2.drawStr((128 - titleWidth) / 2, 25, title);

  // 온도
  int tempWidth = u8g2.getStrWidth(tempBuffer);
  u8g2.drawStr((128 - tempWidth) / 2, 45, tempBuffer);
  u8g2.sendBuffer();


  delay(2000); // 2초 간격으로 출력
}