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
#define STACKSIZE 5
signed long stackX[STACKSIZE];
byte stackop[STACKSIZE];
byte stackptr = 0;
byte SHIFT = 0;

void updateDisplay() {
  if (ERR) {
    module.setDisplayToString("Err"); return;
  }
  module.setDisplayToRDecNumber(Z,SHIFT?65535:(1<<(9-stackptr)));
};

void num(signed long n) {
  if (ERR==1) {
    ERR = 0;Z=0;X=0;stackptr=0;op=0;
  }
  if (eq==1) {eq=0;Z=0;}
  if (Z>999999) return;
  Z = 10 * Z + n;
  updateDisplay();
}

byte priority[] = {
  0, //=
  10, //+
  10, //-
  20, //*
  20, //:
  };

void doMath(byte newOp) {
  #if DEBUG
  Serial.println("OP");
  Serial.print("X=");
  Serial.println(X);
  Serial.print("Z=");
  Serial.println(Z);
  Serial.print("op=");
  Serial.println(op);
  Serial.print("newop=");
  Serial.println(newOp);
  #endif 
  if (priority[newOp]>priority[op]) {
    if(stackptr<STACKSIZE) {
      #if DEBUG
        Serial.println("PUSH");
      #endif  
      stackX[stackptr] = X;
      stackop[stackptr] = op;
      stackptr++;
      X = Z;
      updateDisplay();
      return;
    } else {
      ERR=1;
    }
  }

  
  switch(op) {
    case 0:
      //Z = X;
      break;
    case 1: //add
      Z = X + Z;
      if (Z>9999999) ERR=1;
      break;
    case 2: //sub
      Z = X - Z;
      break;
    case 3: //mul
      Z = X * Z;
      if (Z>9999999) ERR=1;
      break;
    case 4: //div
      if (Z == 0) {
        ERR=1; break;
      }
      Z = X / Z;
      break;
  }

  if(stackptr>0) {
    if (priority[stackop[stackptr-1]]>=priority[newOp]) {
      #if DEBUG
        Serial.print("POP ");
        Serial.println(stackptr);
      #endif  
      stackptr--;
      op = stackop[stackptr];
      X = stackX[stackptr];
      doMath(newOp);
    }
  }
  
  updateDisplay();
  X = Z;
  op = 0;
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
    case 16384: //=
          doMath(0);
          eq=1;
          //cislo=0;
          break;
    case 32768: //+
          doMath(1);
          op=1;
          Z=0;
          break;                       
    case 2048: //-
          doMath(2);
          op=2;
          Z=0;
          break;                       
    case 128: //*
          doMath(3);
          op=3;
          Z=0;
          break;                       
    case 8: // /
          doMath(4);
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
