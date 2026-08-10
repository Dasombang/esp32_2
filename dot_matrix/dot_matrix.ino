#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include "ford_mustang_128.h" // 분리한 헤더 파일 포함

// 핀 정의
#define LCD_CLK  18
#define LCD_DATA 23
#define LCD_CS   5
#define LCD_DC   21
#define LCD_RST  22

// 소프트웨어 SPI 생성자
U8G2_ST7567_JLX12864_F_4W_SW_SPI u8g2(U8G2_R0, LCD_CLK, LCD_DATA, LCD_CS, LCD_DC, LCD_RST);

void setup() {
  u8g2.begin();
  u8g2.setFlipMode(0);
  u8g2.setContrast(160); // 디스플레이 액정 대비 조절 (화면이 안 보일 때 핵심)
}

void loop() {
  u8g2.firstPage();
  do {
    u8g2.clearBuffer(); // 버퍼 초기화
    
    u8g2.drawBitmap(0, 3, 16, 50, ford_mustang_128_map);

  } while (u8g2.nextPage());

  delay(1000);
}