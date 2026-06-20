#include <serial_event.h>

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool STX_COME = false;

void ClearSerialEvent(bool flag) {
    if (flag) {
        STX_COME = false;
        stringComplete = false;
        inputString = "";
        Serial.print("\r\n>> ");
    }
}

void serialEvent() {
    if (Serial.available()) {
        // get the new byte:
        char inChar = (char)Serial.read();
#if _DEBUG_SERIAL_
        if (!(inChar == STX || inChar == ETX1 || inChar == ETX2))
            Serial.print(inChar);
#endif

        // add it to the inputString:
        if (STX_COME) {
            if (inChar == ETX1 || inChar == ETX2) {
                stringComplete = true;
#if _DEBUG_SERIAL_
                Serial.println("\r\nETX come. The message end.");
#endif
                return;
            }
            if (inChar != STX && inChar != ETX1 && inChar != ETX2) {
                inputString += inChar;
            }
            return;
        }

        if (inChar == STX) {
            STX_COME = true;
            stringComplete = false;
            inputString = "";
#if _DEBUG_SERIAL_
            Serial.println("\r\nSTX come. Type your new message. \"Field type\" + \"data\"");
            Serial.print(">> ");
#endif
            return;
        }
    }
}
