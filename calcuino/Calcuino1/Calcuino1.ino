/*********************************
Calcuino 1

Simple integer calculator for 
Arduino + QYF-TM1638

Dependencies: tm1638 library with 
modification. Available at 
http://retrocip.cz/files/tm1638-library-v2.2.0-modified.zip

**********************************/

#include <TM1638.h>
#include <TM1638QYF.h>

TM1638QYF module(11,13,12);

void setup() {
  module.setupDisplay(true, 4);
  updateDisplay();
}

signed long Z = 0; // display register
signed long X; // work register
byte op=0; //Last operation
byte eq=0; //The "=" button was pressed
word lastbutt = 0; // debounce helper
word buttons, buttons2; // debounce helper

//Just update display with the contents
//of display register

void updateDisplay() {
  module.setDisplayToRDecNumber(Z,0);
};

//Pressed number key
void num(signed long n) {
  //Number after "=" means to clear display first
  if (eq==1) {eq=0;Z=0;}
  //Check the boundary
  if (Z>999999) return;
  //Add a number
  Z = 10 * Z + n;
  updateDisplay();
}

//Do math operation
void doMath() {
  if (op==0) return;
  switch(op) {
    case 1: //add
      Z = X + Z;
      break;
    case 2: //sub
      Z = X - Z;
      break;
    case 3: //mul
      Z = X * Z;
      break;
    case 4: //div
      Z = X / Z;
      break;
  }
  updateDisplay();
  //the result is the new work register
  X = Z;
  op = 0;
}

//What to do with a pressed key?!
void keydown(word key) {
  switch(key) {
    case 8192: num(0); break;
    case 256: num(1); break;
    case 512: num(2); break;
    case 1024: num(3); break;
    case 16: num(4); break;
    case 32: num(5); break;
    case 64: num(6); break;
    case 1: num(7); break;
    case 2: num(8); break;
    case 4: num(9); break;
    case 4096: //clear
            Z = 0;
            updateDisplay();
            break;
    case 16384: //=
          doMath();
          eq=1;
          //cislo=0;
          break;
    case 32768: //+
          doMath();
          op=1;
          Z=0;
          break;                       
    case 2048: //-
          doMath();
          op=2;
          Z=0;
          break;                       
    case 128: //*
          doMath();
          op=3;
          Z=0;
          break;                       
    case 8: // /
          doMath();
          op=4;
          Z=0;
          break;                       
          
  }
}

void loop() {
  buttons = module.getButtons();
  if (buttons!=lastbutt) {
    delayMicroseconds(10);
    buttons2=buttons;
    buttons2 = module.getButtons();
    if (buttons == buttons2) {
      //go for it!
      if (buttons>lastbutt) {
        //Serial.print("Keydown ");
        //Serial.println(buttons-lastbutt);
        keydown(buttons-lastbutt);
      }  else {
        //Serial.print("Keyup   ");
        //Serial.println(lastbutt-buttons);
      }
      lastbutt=buttons;
    }
  }
  
  
}
