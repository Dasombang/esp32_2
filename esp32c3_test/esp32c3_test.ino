#include <WiFi.h>
#include <esp_bt.h>

// 핀 정의 (ESP32-C3 SuperMini 16핀 기준 가용 GPIO 예시)
// 보드 실크스크린의 GPIO 번호를 확인 후 수정하세요.
const int TEST_GPIO_OUT = 8;  // 테스트용 출력 핀 (보드 내장 LED가 보통 GPIO 8에 연결되어 있습니다)
const int TEST_GPIO_IN = 2;   // 테스트용 입력 핀 (풀업 저항 연결 또는 점퍼선 테스트용)
const int TEST_ADC_PIN = 0;   // ADC 아날로그 입력 가능 핀 (예: GPIO 0, A0)

void setup() {
  // 시리얼 통신 초기화 (USB CDC 활성화 필요)
  Serial.begin(115200);
  delay(2000); // 시리얼 모니터 연결 대기 시간

  Serial.println("\n========================================");
  Serial.println("  ESP32-C3 16핀 종합 기능 테스트 시작");
  Serial.println("========================================\n");

  // 1. CPU 및 시스템 정보 테스트
  testSystemInfo();

  // 2. GPIO 및 내장 LED 테스트
  testGPIO();

  // 3. ADC(아날로그 입력) 테스트
  testADC();

  // 4. 무선 통신(Wi-Fi & Bluetooth) 스캔 테스트
  testWireless();

  Serial.println("\n========================================");
  Serial.println("  모든 테스트가 완료되었습니다!");
  Serial.println("========================================\n");
}

void loop() {
  // 반복 테스트가 필요할 경우 여기에 작성 (현재는 1회성 종합 테스트)
  delay(10000); 
  Serial.println("[대기 중] 시스템 정상 작동 중...");
}

// 1. 시스템 정보 확인
void testSystemInfo() {
  Serial.println("[테스트 1] 시스템 및 칩 정보 확인");
  Serial.print(" - 칩 모델: "); Serial.println(ESP.getChipModel());
  Serial.print(" - 칩 코어 수: "); Serial.println(ESP.getChipCores());
  Serial.print(" - CPU 주파수: "); Serial.print(ESP.getCpuFreqMHz()); Serial.println(" MHz");
  Serial.print(" - 총 힙 메모리: "); Serial.print(ESP.getHeapSize()); Serial.println(" bytes");
  Serial.print(" - 가용 힙 메모리: "); Serial.print(ESP.getFreeHeap()); Serial.println(" bytes");
  Serial.println("----------------------------------------\n");
}

// 2. GPIO 및 내장 LED 테스트
void testGPIO() {
  Serial.println("[테스트 2] GPIO 및 내장 LED 제어 테스트");
  
  pinMode(TEST_GPIO_OUT, OUTPUT);
  pinMode(TEST_GPIO_IN, INPUT_PULLUP);

  Serial.println(" - LED를 3번 깜빡입니다 (GPIO 8 확인).");
  for (int i = 0; i < 3; i++) {
    digitalWrite(TEST_GPIO_OUT, HIGH);
    delay(300);
    digitalWrite(TEST_GPIO_OUT, LOW);
    delay(300);
  }

  int pinState = digitalRead(TEST_GPIO_IN);
  Serial.print(" - 테스트 입력 GPIO(핀 ");
  Serial.print(TEST_GPIO_IN);
  Serial.print(") 상태: ");
  Serial.println(pinState == HIGH ? "HIGH (풀업 유지)" : "LOW");
  Serial.println("----------------------------------------\n");
}

// 3. ADC(아날로그) 센서 입력 테스트
void testADC() {
  Serial.println("[테스트 3] ADC 아날로그 입력 테스트");
  
  // 아날로그 핀 읽기 (아무것도 연결되어 있지 않으면 플로팅 전압이 읽힐 수 있습니다)
  int adcValue = analogRead(TEST_ADC_PIN);
  float voltage = adcValue * (3.3 / 4095.0); // ESP32-C3는 12비트 ADC (0~4095)

  Serial.print(" - 핀 GPIO ");
  Serial.print(TEST_ADC_PIN);
  Serial.print(" RAW 값 (0~4095): ");
  Serial.println(adcValue);
  
  Serial.print(" - 측정된 대략적 전압: ");
  Serial.print(voltage);
  Serial.println(" V");
  Serial.println("----------------------------------------\n");
}

// 4. 무선 통신(Wi-Fi 및 블루투스) 하드웨어 테스트
void testWireless() {
  Serial.println("[테스트 4] 무선 통신(Wi-Fi / 블루투스) 하드웨어 테스트");

  // Wi-Fi 스캔 테스트
  Serial.println(" - Wi-Fi 스캔을 시작합니다 (주변 AP 탐색)...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("   > 검색된 Wi-Fi 네트워크가 없습니다.");
  } else {
    Serial.print("   > 탐색된 Wi-Fi 네트워크 수: ");
    Serial.println(n);
    for (int i = 0; i < min(n, 3); ++i) { // 상위 3개만 출력
      Serial.print("     * ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (RSSI: ");
      Serial.print(WiFi.RSSI(i));
      Serial.println(" dBm)");
    }
  }
  WiFi.scanDelete();

  // 블루투스(BLE) 초기화 테스트
  Serial.println(" - 블루투스(BLE) 서브시스템 초기화 테스트...");
  // ESP32 블루투스 스택 메모리 초기화 확인
  if (btStart()) {
    Serial.println("   > 블루투스(BLE) 하드웨어 정상 활성화 성공");
    btStop();
    // 블루투스 메모리 해제
    esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);
  } else {
    Serial.println("   > 블루투스 활성화 실패 (파티션 설정 확인 필요)");
  }
  
  Serial.println("----------------------------------------\n");
}