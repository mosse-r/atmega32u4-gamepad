/*
 * tinkerBOY's usb_gamepad v1.0
 * + Power monitoring
 */

#include <Joystick.h>

#define PINS 14
#define ENABLE_ANALOG1 true
#define ENABLE_BATTERY_MONITORING true

int X1 = A0;
int Y1 = A1;
int BATTERY_PIN = A2; // Should be connected with a 10K ohm resistor

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, PINS, 0, 
  true, true, false, false, false, false, false, false, false, false, false);

class CButton {
  public:
  int pin = NULL;
  int lastState = 0;
  
  CButton(int p) {
    pin = p;
  }
};

CButton Buttons[PINS] ={0,1,2,3,5,6,7,8,9,10,16,14,15,21};

void setup() {
  for(int i=0 ; i<PINS ;i++) {
    pinMode(Buttons[i].pin, INPUT_PULLUP);
  }

  Joystick.begin();
  if (ENABLE_ANALOG1) {
    Joystick.setXAxisRange(-512, 512);
    Joystick.setYAxisRange(-512, 512);
  }

  Serial.begin(115200);
}

void JButtonStates() {
  if (ENABLE_ANALOG1) {
    Joystick.setXAxis(analogRead(X1) - 512);
    Joystick.setYAxis(analogRead(Y1) - 512);
  }
  
  for (int i = 0; i < PINS; i++) {
    int currentState = !digitalRead(Buttons[i].pin);
    
    if (currentState != Buttons[i].lastState) {
      Joystick.setButton(i, currentState);
      Buttons[i].lastState = currentState;
    }
  }  
}

void batteryMonitor() {
  if (Serial.available()) {
    int input = Serial.read();
    if( input == '1') {
      int sensorValue = analogRead(BATTERY_PIN);
      Serial.println(sensorValue);
    }
  }
}

void loop() {
  JButtonStates();
  
  if(ENABLE_BATTERY_MONITORING) {
    batteryMonitor();
  }
  
  delay(25);
}
