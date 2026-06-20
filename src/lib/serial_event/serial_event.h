#ifndef SERIAL_EVENT_H_
#define SERIAL_EVENT_H_

#define _DEBUG_SERIAL_ 0

#define STX ':'
#define ETX1 '\n'
#define ETX2 '\r'
#define ACK 0x06 //'*'
#define NAK 0x15 //'!'
#define ESC 0x1B // FOR TIMEOUT! OF PIC COMMUNICATIONS

#define BAUDRATE_SERIAL0 19200

#define TIMEOUT_SERIALPIC_RETURN 2000
#define BAUDRATE_SERIALPIC 19200
#define SerialPIC_RX_PIN 14
#define SerialPIC_TX_PIN 13
#define SerialPIC Serial1

#include <Arduino.h>

extern String inputString;  // a String to hold incoming data
extern bool stringComplete; // whether the string is complete
extern bool STX_COME;

void ClearSerialEvent(bool);
void serialEvent();

#endif
