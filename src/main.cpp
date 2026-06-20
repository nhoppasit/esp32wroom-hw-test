// hello

// STUDY OF DESCRIPTOR-2902

// 2021-jun-12, Nhoppasit

#define SW_VERSION "25OCT2022a"



#include <Arduino.h>

#include <BLE2902.h>

#include <BLEDevice.h>

#include <BLEServer.h>

#include <BLEUtils.h>

#include <EEPROM.h>

#include <HardwareSerial.h>



#include "ble.h"

#include "dat.h"

#include "flash_memory.h"

#include "general_info.h"

#include "output_test.h"

#include "rtc.h"

#include "serial_event.h"

#include "uart_commands.h"

#include "utilities.h"



#define BAT_LOW 10.5

float vbat = 12;



void serialProcesses() {

  // ---------------------------------------------------------------------------

  // UART COMMANDER

  // ---------------------------------------------------------------------------

  if (stringComplete) {

    String uInputString;

    String fieldType;

    String data;

    uInputString = inputString;

    uInputString.toUpperCase();

    fieldType = uInputString.substring(0, 2);

    data = inputString.substring(2);

#if _DEBUG_SERIAL_

    Serial.print("Field type: ");

    Serial.println(fieldType);

    Serial.print("Data: ");

    Serial.println(data);

#endif

    Serial.println();

    if (uInputString.equals("V?")) {

      Serial.println(SW_VERSION);

    }

    uart_query_currentDt(uInputString.equals(CMD_QUERY_CURRENT_TIME));

    uart_query_general_infomation(uInputString.equals(CMD_INFO));

    uart_query_token(uInputString.equals(CMD_QUREY_TOKEN));

    uart_clear_token(uInputString.equals(CMD_CLEAR_TOKEN));

    uart_set_token(fieldType == CMD_SET_TOKEN, data);

    uart_set_info_ble_name(fieldType == CMD_SET_BLE_NAME, data);

    uart_set_info_sku(fieldType == CMD_SET_SKU, data);

    uart_set_info_sn(fieldType == CMD_SET_SN, data);

    uart_set_info_lot(fieldType == CMD_SET_LOT, data);

  }

}



void setup() {

  Serial.begin(19200);

  while (!Serial)

    ;

  Serial.println("\r\n-------------------------------------");

  Serial.print("Starting ");

  Serial.println(BLE_DEVICE_ID);



  // ---------------------------------------------------------

  // TIMER0 FOR 10MS TICK

  // ---------------------------------------------------------

  initialization_timer0_10ms();



  // ---------------------------------------------------------

  // EEPROM starting.

  // ---------------------------------------------------------

  if (!EEPROM.begin(EEPROM_SIZE)) {

    Serial.println("Failed to initialise EEPROM");

    Serial.println("Restarting...");

    delay(1000);

    ESP.restart(); // !!! DANGEROUS !!!

  }



  // ---------------------------------------------------------

  // BLE construction

  // ---------------------------------------------------------

  ble_begin(BLE_DEVICE_ID);



  // ---------------------------------------------------------

  // Assign any BLE objects.

  // ---------------------------------------------------------

  ble_service_initialization_token();

  ble_service_initialization_info();

  ble_service_initialization_rtc();

  ble_service_initialization_flash();

  ble_service_initialization_test();



  // ---------------------------------------------------------

  // BLE start advertising

  // ---------------------------------------------------------

  ble_advertising_start();



  // ---------------------------------------------------------

  // PIC16F15386 UART

  // ---------------------------------------------------------

  SerialPIC.begin(BAUDRATE_SERIALPIC, SERIAL_8N1, SerialPIC_RX_PIN,

                  SerialPIC_TX_PIN);

  while (!SerialPIC)

    ;

  Serial.println("PIC16F15386 UART [OK].\r\n");



  // ---------------------------------------------------------

  // ECHO SOFTWARE VERSION

  // ---------------------------------------------------------

  uart_set_info_lot(true, SW_VERSION);



  // ---------------------------------------------------------

  // Next

  // ---------------------------------------------------------

  Serial.println("Setup [OK].\r\n");

  Serial.print(">> ");

}



void loop() {

  // ---------------------------------------------------------

  // UPDATE INPUT OF SYSTEM

  // ---------------------------------------------------------

  serialEvent();

  serialProcesses();



  // ---------------------------------------------------------

  // BLE characteristics and notify changed value

  // ---------------------------------------------------------

  if (deviceConnected) {

    // put your main code here, to run repeatedly:

    // some code...

    // SerialPIC.println(millis());

    // delay(100);

  }



  // ---------------------------------------------------------

  // BLE RE-ADVERTISING

  // ---------------------------------------------------------

  ble_advertising_again(); // For disconnecting process.



  // ---------------------------------------------------------

  // BLE connecting

  // ---------------------------------------------------------

  if (deviceConnected && !oldDeviceConnected) {

    // do stuff here on connecting

    // some code...

    oldDeviceConnected = deviceConnected;

  }



  // ---------------------------------------------------------

  // Finalization

  // ---------------------------------------------------------

  ClearSerialEvent(stringComplete);

}
