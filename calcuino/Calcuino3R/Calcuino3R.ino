#include <TM1638.h>
#include <TM1638QYF.h>
TM1638QYF module(11,13,12);
void setup() {
  // put your setup code here, to run once:
  module.setupDisplay(true, 0);
  Serial.begin(9600);
  updateDisplay();
}

signed long Z = 0;
signed long X;
byte op=0;
byte eq=0;
byte ERR = 0;
word lastbutt = 0;
word buttons, buttons2;

//#define DEBUG 1
#define DEBUG 0
#define STACKSIZE 8
signed long stack[STACKSIZE];
byte stackptr = 0;
byte SHIFT = 0;

void push(signed long n) {
  if (stackptr<STACKSIZE) {
    stack[stackptr++]=n;
  } else {ERR=1;}
}
signed long pop() {
  if(stackptr>0) {
    return stack[--stackptr];
  } else {
    ERR=1;
  }
}

void updateDisplay() {
  if (ERR) {
    module.setDisplayToString("Err"); return;
  }
  module.setDisplayToRDecNumber(Z,SHIFT?65535:(1<<(8-stackptr)));
};

void num(signed long n) {
  if (ERR==1) {
    ERR = 0;Z=0;X=0;stackptr=0;op=0;
  }
  if (op==1) {op=0;Z=0;}
  if (Z>999999) return;
  Z = 10 * Z + n;
  updateDisplay();
}



void keydown(word key) {
  if (SHIFT) {
    SHIFT = 0;
    switch(key) {
      case 8192: //Shift 0 - Clear
            Z = 0;
            ERR = 0;
            break;
      case 2048: //Shift - Negate
            Z = -Z;
            break;
      case 16384: //Shift Enter - safe POP
            Z = pop();
            if (ERR==1) {ERR=0; Z=0;}
            break;

      case 32768: //Shift + - DUP
            Z = pop();
            push(Z);push(Z);
            module.setDisplayToString("duP");
            return;
            break;
      case 128: //Shift * - over
            X = pop();Z=pop();
            push(Z);push(X);push(Z);
            break;
                  
                  
    }
    updateDisplay();

    return;
  }
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
    case 4096: //Shift
            SHIFT = 1;
            updateDisplay();
            break;
    case 16384: //ENTER
          push(Z);
          updateDisplay();
          Z=0;
          op=1;
          break;
    case 32768: //+
          Z = pop() + pop();
          push(Z);
          updateDisplay();
          op=1;
          break;                       
    case 2048: //-
          Z = pop();
          Z = pop() - Z;
          push(Z);
          updateDisplay();
          op=1;
          break;                       
    case 128: //*
          Z = pop() * pop();
          push(Z);
          updateDisplay();
          op=1;
          break;                       
    case 8: // /
          Z = pop();
          Z = pop() / Z;
          push(Z);
          updateDisplay();
          op=1;
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
        keydown(buttons-lastbutt);
      }  else {
        //Serial.println(lastbutt-buttons);
      }
      lastbutt=buttons;
    }
  }
  
  
  // put your main code here, to run repeatedly:
  //
  //delay(1000);
  
}
