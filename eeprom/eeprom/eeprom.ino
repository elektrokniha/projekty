#include <Wire.h>

void writeEEPROM(unsigned int address, uint8_t data) {
 Wire.beginTransmission(0x50);
 Wire.write((int)(address >> 8)); // MSB
 Wire.write((int)(address & 0xFF)); // LSB
 Wire.write(data);
 Wire.endTransmission();
 delay(5);
}

uint8_t readEEPROM(unsigned int address) {
 uint8_t data = 0xFF;
 Wire.beginTransmission(0x50);
 Wire.write((int)(address >> 8)); // MSB
 Wire.write((int)(address & 0xFF)); // LSB
 Wire.endTransmission();
 Wire.requestFrom(0x50,1);
 if (Wire.available()) data = Wire.read();
 return data;
}

void setup() {
    Wire.begin();
    Serial.begin(9600);
   
    writeEEPROM(100, 0x55);
    delay(5);
    byte a = readEEPROM(100);
    Serial.print("Hodnota na adrese 100 je: ");
    Serial.println(a);
}


void loop() {
  // put your main code here, to run repeatedly:

}
