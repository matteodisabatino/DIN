
#include <avr/pgmspace.h> //dmled
#include <avr/io.h>   //dmled
#include <SPI.h> // DAC
#include <digitalWriteFast.h>  // DAC
#include <DACRT760.h> // DAC
DACRT760 dac;  //DAC
#include "Apio.h"
#include "property.h"
#include <math.h>
#include <Wire.h>   //// IODIG  & ADC
#include "RTI2CDIGIO.h"  ////IODIG
//#include <Wire.h>  // ADC
#include "ADCRT3424.h" // ADC

byte gain = 0; // 0-3 = {x1,x2,x4,x8}  // 0 = 0-10v //  1 = 0-5v
byte resolution = 0; // 0-3 = {12bits, 14bits, 16bits, 18bits}

ADCRT3424 ADC10(0x69, gain, resolution);
ADCRT3424 ADC20(0x6A, gain, resolution);// create MCP3424 instance.

byte din1Value =0;
byte din2Value =0;
byte din3Value =0;
byte din4Value =0;
byte din5Value =0;
byte din6Value =0;
byte din7Value =0;
byte din8Value =0;

#define ADC1 9 // Led To Custom Function
#define ADC2 11 // Led To Custom Function
#define ADC3 12 // Led To Custom Function
#define ADC4 20 // Led To Custom Function
#define ADC5 21 // Led To Custom Function
#define ADC6 22 // Led To Custom Function

#define DIG1 23 // Led To Custom Function
#define DIG2 24 // Led To Custom Function
#define DIG3 25 // Led To Custom Function
#define DIG4 26 // Led To Custom Function
#define DIG5 27 // Led To Custom Function
#define DIG6 19 // Led To Custom Function 
#define DIG7 14 // Led To Custom Function
#define DIG8 17 // Led To Custom Function

#define RL1 5 // Led To Custom Function
#define RL2 4 // Led To Custom Function
#define RL3 6 // Led To Custom Function
#define RL4 16 // Led To Custom Function
#define RL5 10 // Led To Custom Function   ??
#define RL6 7 // Led To Custom Function

#define VIN 18 // Led To Custom Function

#define UCTX 3
byte time = 50;

RTI2CDIGIO mcp;
RTI2CDIGIO mcp2;

double PrevCH1 = 0; 
double PrevCH2 = 0;
double PrevCH3 = 0;
double PrevCH4 = 0;
double PrevCH5 = 0;
double PrevCH6 = 0;

int flagS = 1;
int i = 0;
int out1 = 0;
int out2 = 0;
unsigned long previousMillis = 0;


  double CH6 ;
  double CH5 ;
  double CH4 ;
  double CH3 ;
  double CH2 ;
  double CH1 ;
  
  
long heartBeatArray[] = {50, 100, 15, 1200};
int hbeatIndex = 1;  
long prevMillis;


void setup() {
  
  Apio.setup("DIN", "1,0", 3030, 0x01);
   
  mcp.begin(1);

  mcp2.begin(2);

  mcp.pinMode(0, OUTPUT); // N8
  mcp.pinMode(1, OUTPUT); // N7
  mcp.pinMode(2, OUTPUT); // N6
  mcp.pinMode(3, OUTPUT); // N5
  mcp.pinMode(4, OUTPUT); // N4
  mcp.pinMode(5, OUTPUT); // N3
  mcp.pinMode(6, OUTPUT); // N2
  mcp.pinMode(7, OUTPUT); // N1

  mcp.pinMode(8, INPUT); // N1
  mcp.pinMode(9, INPUT); // N2
  mcp.pinMode(10, INPUT); // N3
  mcp.pinMode(11, INPUT); // N4
  mcp.pinMode(12, INPUT); // N5
  mcp.pinMode(13, INPUT); // N6
  mcp.pinMode(14, INPUT); // N7

  mcp.pullUp(8, HIGH);
  mcp.pullUp(9, HIGH);
  mcp.pullUp(10, HIGH);
  mcp.pullUp(11, HIGH);
  mcp.pullUp(12, HIGH);
  mcp.pullUp(13, HIGH);
  mcp.pullUp(14, HIGH);
  mcp.pullUp(15, HIGH);

  mcp2.pinMode(0, OUTPUT); // RELE' 1
  mcp2.pinMode(1, OUTPUT); // RELE' 2
  mcp2.pinMode(2, OUTPUT); // RELE' 3
  mcp2.pinMode(3, OUTPUT); // RELE' 4
  mcp2.pinMode(4, OUTPUT); // RELE' 5
  mcp2.pinMode(5, OUTPUT); // RELE' 6

pinMode(ADC1, OUTPUT);
pinMode(ADC2, OUTPUT);
pinMode(ADC3, OUTPUT);
pinMode(ADC4, OUTPUT);
pinMode(ADC5, OUTPUT);
pinMode(ADC6, OUTPUT);

pinMode(DIG1, OUTPUT);
pinMode(DIG2, OUTPUT);
pinMode(DIG3, OUTPUT);
pinMode(DIG4, OUTPUT);
pinMode(DIG5, OUTPUT);
pinMode(DIG6, OUTPUT);
pinMode(DIG7, OUTPUT);
pinMode(DIG8, OUTPUT);

pinMode(RL1, OUTPUT);
pinMode(RL2, OUTPUT);
pinMode(RL3, OUTPUT);
pinMode(RL4, OUTPUT);
pinMode(RL5, OUTPUT);
pinMode(RL6, OUTPUT);

pinMode(VIN, OUTPUT);
pinMode(UCTX, OUTPUT);

//Wire.begin(); 
dac.begin(1,1);  // 0 - (0-5v) | 1 - (0-10v) | 2 - (+/-5v) | 3 - (+/-10v) | 4 - (not allowed) | 5 - (4-20mA) | 6 - (0-20mA) | 7 - (0-24mA) |

//digitalWrite(RX0,LOW);
//digitalWrite(TX0,LOW);

}



void loop(){    ////////////////////////////    LOOP     /////////////////    LOOP     /////////////////

Apio.loop();
//SYS_TaskHandler();
//CH1 = ADC20.getChannelV(1);

AdcRead();
LedAdcUpdate();
LedDigUpdtate();

    if(mcp.digitalRead(8) == HIGH && din1Value==0) { 
    din1Value = 1;
    Apio.send("3030:update:din1:"+String(din1Value)+"-");
    }
    else if(mcp.digitalRead(8) == LOW && din1Value==1){ 
      din1Value = 0;  
      Apio.send("3030:update:din1:"+String(din1Value)+"-");
    }
    
    if(mcp.digitalRead(9) == HIGH && din2Value==0) { 
    din2Value = 1;
    Apio.send("3030:update:din2:"+String(din2Value)+"-");
    }
    else if(mcp.digitalRead(9) == LOW && din2Value==1){ 
      din2Value = 0;  
      Apio.send("3030:update:din2:"+String(din2Value)+"-");
    }
    
    if(mcp.digitalRead(10) == HIGH && din3Value==0) { 
    din3Value = 1;
    Apio.send("3030:update:din3:"+String(din3Value)+"-");
    }
    else if(mcp.digitalRead(10) == LOW && din3Value==1){ 
      din3Value = 0;  
      Apio.send("3030:update:din3:"+String(din3Value)+"-");
    }
    
    if(mcp.digitalRead(11) == HIGH && din4Value==0) { 
    din4Value = 1;
    Apio.send("3030:update:din4:"+String(din4Value)+"-");
    }
    else if(mcp.digitalRead(11) == LOW && din4Value==1){ 
      din4Value = 0;  
      Apio.send("3030:update:din4:"+String(din4Value)+"-");
    }
    
    if(mcp.digitalRead(12) == HIGH && din5Value==0) { 
    din5Value = 1;
    Apio.send("3030:update:din5:"+String(din5Value)+"-");
    }
    else if(mcp.digitalRead(12) == LOW && din5Value==1){ 
      din5Value = 0;  
      Apio.send("3030:update:din5:"+String(din5Value)+"-");
    }
    
    if(mcp.digitalRead(13) == HIGH && din6Value==0) { 
    din6Value = 1;
    Apio.send("3030:update:din6:"+String(din6Value)+"-");
    }
    else if(mcp.digitalRead(13) == LOW && din6Value==1){ 
      din6Value = 0;  
      Apio.send("3030:update:din6:"+String(din6Value)+"-");
    }
    
    if(mcp.digitalRead(14) == HIGH && din7Value==0) { 
    din7Value = 1;
    Apio.send("3030:update:din7:"+String(din7Value)+"-");
    }
    else if(mcp.digitalRead(14) == LOW && din7Value==1){ 
      din7Value = 0;  
      Apio.send("3030:update:din7:"+String(din7Value)+"-");
    }
    
    if(mcp.digitalRead(15) == HIGH && din8Value==0) { 
    din8Value = 1;
    Apio.send("3030:update:din8:"+String(din8Value)+"-");
    }
    else if(mcp.digitalRead(15) == LOW && din8Value==1){ 
      din8Value = 0;  
      Apio.send("3030:update:din8:"+String(din8Value)+"-");
    }



	
//HeartLed(1.0,VIN);  // pulse hearth

Triggeri2ctwo("rel1",5 ,1, 0, RL1 );
Triggeri2ctwo("rel2",4 ,1, 0, RL2 );
Triggeri2ctwo("rel3",3 ,1, 0, RL3 );
Triggeri2ctwo("rel4",2 ,1, 0, RL4 );
Triggeri2ctwo("rel5",1 ,1, 0, RL5 );
Triggeri2ctwo("rel6",0 ,1, 0, RL6 );

Triggeri2cone("dig1",7 ,1, 0 );
Triggeri2cone("dig2",6 ,1, 0 );
Triggeri2cone("dig3",5 ,1, 0 );
Triggeri2cone("dig4",4 ,1, 0 );
Triggeri2cone("dig5",3 ,1, 0 );
Triggeri2cone("dig6",2 ,1, 0 );
Triggeri2cone("dig7",1 ,1, 0 );
Triggeri2cone("dig8",0 ,1, 0 );




Sliderdac();

unsigned long currentMillis = millis();
       
        if(currentMillis - previousMillis >= 1000){
               previousMillis = currentMillis;
        }  ///  fine millis()
 
	UpdateDataToSystem();
} 
// fine loop




void AdcRead(){
  
CH1 = ADC20.getChannelV(1);
CH2 = ADC20.getChannelV(0);
CH3 = ADC10.getChannelV(3);
CH4 = ADC10.getChannelV(2);
CH5 = ADC10.getChannelV(1);
CH6 = ADC10.getChannelV(0);

}


void LedAdcUpdate(){
  
if(CH1 > 3){digitalWrite(ADC1,HIGH);}
else {digitalWrite(ADC1,LOW);}
if(CH2 > 3){digitalWrite(ADC2,HIGH);}
else {digitalWrite(ADC2,LOW);} 
if(CH3 > 3){digitalWrite(ADC3,HIGH);}
else {digitalWrite(ADC3,LOW);}
if(CH4 > 3){digitalWrite(ADC4,HIGH);}
else {digitalWrite(ADC4,LOW);}
if(CH5 > 3){digitalWrite(ADC5,HIGH);}
else {digitalWrite(ADC5,LOW);}
if(CH6 > 3){digitalWrite(ADC6,HIGH);}
else {digitalWrite(ADC6,LOW);}
}

void LedDigUpdtate(){
  
digitalWrite(DIG1,mcp.digitalRead(8 ));
digitalWrite(DIG2,mcp.digitalRead(9 ));
digitalWrite(DIG3,mcp.digitalRead(10));
digitalWrite(DIG4,mcp.digitalRead(11));
digitalWrite(DIG5,mcp.digitalRead(12));
digitalWrite(DIG6,mcp.digitalRead(13));
digitalWrite(DIG7,mcp.digitalRead(14));
digitalWrite(DIG8,mcp.digitalRead(15));


}


void UpdateDataToSystem(){

 if(abs(CH1-PrevCH1)>0.3){
 //CH1 = ADC20.getChannelV(1);
 Apio.send("3030:update:adc1:"+String(CH1)+"-");}
 
 if(abs(CH2-PrevCH2)>0.3){
 //CH2 = ADC20.getChannelV(0);
 Apio.send("3030:update:adc2:"+String(CH2)+"-");}
 
 if(abs(CH3-PrevCH3)>0.3){
 //CH3 = ADC10.getChannelV(3);
 Apio.send("3030:update:adc3:"+String(CH3)+"-");}
 
 if(abs(CH4-PrevCH4)>0.3){
 //CH4 = ADC10.getChannelV(2);
 Apio.send("3030:update:adc4:"+String(CH4)+"-");}
 
 if(abs(CH5-PrevCH5)>0.3){
 //CH5 = ADC10.getChannelV(1);
 Apio.send("3030:update:adc5:"+String(CH5)+"-");}
 
 if(abs(CH6-PrevCH6)>0.3){
 //CH6 = ADC10.getChannelV(0);
 Apio.send("3030:update:adc6:"+String(CH6)+"-");}

 
 PrevCH1 = CH1;
 PrevCH2 = CH2;
 PrevCH3 = CH3;
 PrevCH4 = CH4;
 PrevCH5 = CH5;
 PrevCH6 = CH6;
}


void Triggeri2cone(char *propertyName, int pin, int onValue, int offValue){   /////// ADD By Ulisse Ruiz for i2c 
 
  if(Apio.property == propertyName){
     //Serial.println(Apio.property+" "+propertyName);
    if(onValue == Apio.value.toInt()){
      mcp.digitalWrite(pin, onValue);
    } else if (offValue == Apio.value.toInt()){
      mcp.digitalWrite(pin, offValue);
    }
  }
}

void Triggeri2ctwo(char *propertyName, int pin, int onValue, int offValue, int ledNumber){   /////// ADD By Ulisse Ruiz for i2c 
  
  if(Apio.property == propertyName){
    //Serial.println(Apio.property+" "+propertyName);
    if(onValue == Apio.value.toInt()){
      mcp2.digitalWrite(pin, onValue);
      digitalWrite(ledNumber,HIGH);
      
    } else if (offValue == Apio.value.toInt()){
      mcp2.digitalWrite(pin, offValue);
      digitalWrite(ledNumber,LOW);
    }
  }
}

void Sliderdac(){
  
     if(Apio.property == "dac1"){
   out1 = Apio.value.toInt();}
   else if (Apio.property == "dac2"){
   out2 = Apio.value.toInt();}
   dac.write(out1,out2);  
      
}


void HeartLed(float tempo,int pin){
    if ((millis() - prevMillis) > (long)(heartBeatArray[hbeatIndex] * tempo)){
        hbeatIndex++;
        if (hbeatIndex > 3) hbeatIndex = 0;

        if ((hbeatIndex % 2) == 0){ 
            digitalWrite(pin, HIGH); 
            delay((int)heartBeatArray[hbeatIndex]) ;   
            digitalWrite(pin, LOW); 
        }
        hbeatIndex++;
        // Serial.println(hbeatIndex);
        prevMillis = millis();

    }
}


////// DA CONTROLLARE SU HARDWARE
// RISCALDAMENTO LDO DEL RELè
//


