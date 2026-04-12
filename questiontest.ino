#include <EEPROM.h>
#include <Arduino.h>

String text_in;
String my_text = "testar"

void setup() {

    Serial.begin(9600);
    EEPROM.put(22, my_text);

}

void loop() {
    EEPROM.get(22, text_in);
    Serial.println(text_in);
    delay(500);
}
