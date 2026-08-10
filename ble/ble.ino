#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// BLE Service UUID
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

// BLE Characteristic UUID
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;

bool deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
    Serial.println("iPhone connected");
  }

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
    Serial.println("iPhone disconnected");

    // 다시 BLE 광고 시작
    pServer->getAdvertising()->start();
    Serial.println("BLE advertising restarted");
  }
};

void setup() {
  Serial.begin(115200);

  // BLE 장치 이름
  BLEDevice::init("ESP32_TEST");

  // BLE Server 생성
  BLEServer *pServer = BLEDevice::createServer();

  // 연결 상태 Callback
  pServer->setCallbacks(new MyServerCallbacks());

  // Service 생성
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Characteristic 생성
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  // iPhone에서 Notification을 받을 수 있도록 설정
  pCharacteristic->addDescriptor(new BLE2902());

  // 초기 메시지
  pCharacteristic->setValue("ESP32 Ready");

  // Service 시작
  pService->start();

  // BLE Advertising 시작
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);

  BLEDevice::startAdvertising();

  Serial.println("============================");
  Serial.println("BLE Server Started");
  Serial.println("Device Name : ESP32_TEST");
  Serial.println("============================");
}

void loop() {

  if (deviceConnected) {

    static unsigned long previousMillis = 0;

    unsigned long currentMillis = millis();

    // 30초마다 실행
    if (currentMillis - previousMillis >= 30000) {

      previousMillis = currentMillis;

      String message = "Hello iPhone!";

      pCharacteristic->setValue(message.c_str());
      pCharacteristic->notify();

      Serial.print("Message sent: ");
      Serial.println(message);
    }
  }

  delay(10);
}