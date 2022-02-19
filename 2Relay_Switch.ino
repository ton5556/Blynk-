#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""
#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
#define USE_NODE_MCU_BOARD

#include "BlynkEdgent.h"
#include <AceButton.h>
using namespace ace_button;

#define RelayPin1 5  //D1
#define RelayPin2 4  //D2

#define SwitchPin1 10  //SD3
#define SwitchPin2 D3   //D3 

#define wifiLed   16   //D0


#define VPIN_BUTTON_1    V1
#define VPIN_BUTTON_2    V2

int toggleState_1 = 0; //Define integer to remember the toggle state for relay 1
int toggleState_2 = 0; //Define integer to remember the toggle state for relay 2

ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);

void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);
void relayOnOff(int relay){
    switch(relay){
      case 1: 
             if(toggleState_1 == 0){
              digitalWrite(RelayPin1, LOW); // turn on relay 1
              toggleState_1 = 1;
              Serial.println("Device1 ON");
              }
             else{
              digitalWrite(RelayPin1, HIGH); // turn off relay 1
              toggleState_1 = 0;
              Serial.println("Device1 OFF");
              }
             delay(100);
      break;
      case 2: 
             if(toggleState_2 == 0){
              digitalWrite(RelayPin2, LOW); // turn on relay 2
              toggleState_2 = 1;
              Serial.println("Device2 ON");
              }
             else{
              digitalWrite(RelayPin2, HIGH); // turn off relay 2
              toggleState_2 = 0;
              Serial.println("Device2 OFF");
              }
             delay(100);
      break;
      default : break;      
      }  
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
}

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  if(toggleState_1 == 1){
    digitalWrite(RelayPin1, LOW);
  }
  else { 
    digitalWrite(RelayPin1, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  if(toggleState_2 == 1){
    digitalWrite(RelayPin2, LOW);
  }
  else { 
    digitalWrite(RelayPin2, HIGH);
  }
}
void setup()
{
  Serial.begin(115200);
  delay(100);
  
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  
  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);

  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);

  digitalWrite(wifiLed, HIGH);

  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);

  button1.init(SwitchPin1);
  button2.init(SwitchPin2);

  BlynkEdgent.begin();

  Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
  Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
}

void loop() {
    BlynkEdgent.run();
    button1.check();
    button2.check();
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  relayOnOff(1);
  Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Update Button Widget
}
void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT2");
  relayOnOff(2);
  Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);   // Update Button Widget
}
