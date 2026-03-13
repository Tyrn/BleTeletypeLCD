#pragma once

// main.cpp must contain #include "this_one",
// and nothing else.

/*
  TODO

    [] Create a BLE Device
      [] Import BLEDevice Library
      [] Name Device
      [] Initialize Device

    [] Create Server
      [] Import BLEServer Library
      [] Create BLEServer

    [] Create Server Callbacks
      [] OnConnect
        [] Turn on LED
      [] OnDisconnect
        [] Turn off LED

    [] Create Service
      [] Define a Service UUID
      [] Create a Service
      [] Start the Service

    [] Create Characteristic
      [] Define a Characteristic UUID
      [] Create a Characteristic
      [] Add Characteristic to Service

    [] Create a Characteristic Callback
      [] Create sub-class to override BLECharacteristicCallbacks
      [] Send millis value every time request comes in

    [] Create a Descriptor
      [] Add a Descriptor (2901)

    [] Advertise the Service
      [] Get the Advertising object from Server
      [] Set the properties of the Advertisement data
      [] Set the Advertisement Data to the Service
      [] Add Service UUID
      [] Start Advertising
 */

#include <Arduino.h>
#include <BLEDevice.h>

#define DEVICE_NAME "BLE CIA Teletype"
// See https://www.uuidgenerator.net
#define SERVICE_1_UUID "d9b4049b-c663-446c-9211-71754d930811"
#define CHARACTERISTIC_1A_UUID "596d9fe5-baf0-472b-89ee-bbb3ce27165a"

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    digitalWrite(2, HIGH);
    Serial.println("Client Connected");
  };

  void onDisconnect(BLEServer *pServer) {
    digitalWrite(2, LOW);
    Serial.println("Client Disconnected");
    BLEDevice::startAdvertising();
  }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {

  // On client request
  void onRead(BLECharacteristic *pCharacteristic) {
    uint32_t currentMillis = millis() / 1000;
    pCharacteristic->setValue(currentMillis);
  }
};

// NOLINTBEGIN(misc-definitions-in-headers)
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 BLE Server setup beginning...");

  pinMode(2, OUTPUT);

  BLEDevice::init(DEVICE_NAME);

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_1_UUID);
  pService->start();

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_1A_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  // BLE2901 *descriptor_2901 = new BLE2901();
  // descriptor_2901->setDescription("Time");
  // pCharacteristic->addDescriptor(descriptor_2901);
  BLEDescriptor *descriptor_2901 = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
  descriptor_2901->setValue("Time");
  pCharacteristic->addDescriptor(descriptor_2901);

  pService->start();

  // Advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

  BLEAdvertisementData advertisementData; // For passive scan
  advertisementData.setName(DEVICE_NAME);
  advertisementData.setManufacturerData("Chinese Factory");

  BLEAdvertisementData scanResponseData; // For active scan
  scanResponseData.setName("Extra Scan Data");

  pAdvertising->setAdvertisementData(advertisementData);
  pAdvertising->setScanResponseData(scanResponseData);

  pAdvertising->addServiceUUID(SERVICE_1_UUID);
  pAdvertising->setScanResponse(true);

  BLEDevice::startAdvertising();
}

void loop() {}

// NOLINTEND(misc-definitions-in-headers)
