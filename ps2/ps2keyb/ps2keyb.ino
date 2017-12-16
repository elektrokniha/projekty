#include <PS2KeyboardPlus.h>
#define byte uint8_t

PS2KeyboardPlus keyboard;

void onRelease(unsigned int i){
  Serial.print("RELEASE:");
  Serial.println(i, HEX);
}

void onPress(unsigned int i){
  Serial.print("PRESS:");
  Serial.println(i, HEX);
}


void setup() {
  keyboard.begin(DATA_PIN);
  Serial.begin(9600);
  keyboard.setReleaseHandler(onRelease);
  keyboard.setPressHandler(onPress);
  Serial.println("Keyboard Plus ready");
  keyboard.reset();
  delay(1000);
  //keyboard.setLED(7);
}

void loop() {
  if(keyboard.available()) {
    unsigned int dat = keyboard.read();
    Serial.println(dat, HEX);
  }
}
