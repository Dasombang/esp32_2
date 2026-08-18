#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// =====================================================
// TFT
// =====================================================

TFT_eSPI tft = TFT_eSPI();


// =====================================================
// XPT2046
// =====================================================

#define XPT2046_IRQ  36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK  25
#define XPT2046_CS   33

SPIClass touchscreenSPI = SPIClass(VSPI);

XPT2046_Touchscreen touchscreen(
    XPT2046_CS,
    XPT2046_IRQ
);


// =====================================================
// Touch calibration
// =====================================================

#define TOUCH_MIN_X  280
#define TOUCH_MAX_X  3670

#define TOUCH_MIN_Y  440
#define TOUCH_MAX_Y  3700


// =====================================================
// Setup
// =====================================================

void setup()
{
    Serial.begin(115200);

    delay(1000);

    // TFT 초기화
    tft.init();
    tft.setRotation(1);

    tft.fillScreen(TFT_BLACK);

    // 화면 테두리
    tft.drawRect(
        0,
        0,
        320,
        240,
        TFT_GREEN
    );

    // 제목
    tft.setTextDatum(MC_DATUM);

    tft.setTextFont(2);
    tft.setTextColor(
        TFT_CYAN,
        TFT_BLACK
    );

    tft.drawString(
        "TOUCH TEST",
        160,
        30
    );

    tft.setTextColor(
        TFT_WHITE,
        TFT_BLACK
    );

    tft.drawString(
        "Touch the screen",
        160,
        65
    );


    // Touch SPI 초기화
    touchscreenSPI.begin(
        XPT2046_CLK,
        XPT2046_MISO,
        XPT2046_MOSI,
        XPT2046_CS
    );

    touchscreen.begin(
        touchscreenSPI
    );

    touchscreen.setRotation(1);

    Serial.println("Touch test started.");
}


// =====================================================
// Loop
// =====================================================

void loop()
{
    if (touchscreen.tirqTouched() &&
        touchscreen.touched())
    {
        TS_Point p = touchscreen.getPoint();


        // Raw → LCD 좌표
        int screenX = map(
            p.x,
            TOUCH_MIN_X,
            TOUCH_MAX_X,
            0,
            319
        );

        int screenY = map(
            p.y,
            TOUCH_MIN_Y,
            TOUCH_MAX_Y,
            0,
            239
        );


        // 범위 제한
        screenX = constrain(
            screenX,
            0,
            319
        );

        screenY = constrain(
            screenY,
            0,
            239
        );


        // Serial 출력
        Serial.print("RAW X = ");
        Serial.print(p.x);

        Serial.print("  RAW Y = ");
        Serial.print(p.y);

        Serial.print("  ->  X = ");
        Serial.print(screenX);

        Serial.print("  Y = ");
        Serial.println(screenY);


        // 기존 좌표 표시 영역 삭제
        tft.fillRect(
            60,
            90,
            200,
            70,
            TFT_BLACK
        );


        // 좌표 출력
        tft.setTextDatum(MC_DATUM);
        tft.setTextFont(2);
        tft.setTextColor(
            TFT_YELLOW,
            TFT_BLACK
        );

        char buffer[32];

        sprintf(
            buffer,
            "X = %d",
            screenX
        );

        tft.drawString(
            buffer,
            160,
            110
        );

        sprintf(
            buffer,
            "Y = %d",
            screenY
        );

        tft.drawString(
            buffer,
            160,
            140
        );


        // 터치 위치 표시
        tft.fillCircle(
            screenX,
            screenY,
            4,
            TFT_RED
        );


        delay(100);
    }
}