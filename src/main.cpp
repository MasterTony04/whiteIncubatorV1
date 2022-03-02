#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <SimpleTimer.h>
#include "DHT.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

//Constants
#define DHTPIN A0   // A1 is DHT 2 sensor
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#include "DHT.h"

LiquidCrystal_I2C lcd(0x27,20,4);

// Classes
#include "menus.h"
#include "Incubation_Screen.h"
#include "fanControl.h"

//Timers
SimpleTimer firstTimer(1000);
SimpleTimer ignitionTimer(300);  //300 initially
SimpleTimer motorTimer(5000);

//Variables
//TODO: pin A2 is unused
uint8_t humidifier = 9, igniter = 10, heater = 11, gasValve=12;
uint8_t buzzer =A3;
uint8_t upButton = 2,downButton = 4,selectButton = 3, okButton=0;
uint8_t powerIn =7;

uint8_t menu = 1;
uint8_t subMenu=0;
uint8_t state1,state2,state3;
uint8_t idle=1;
uint8_t startStatus=1;
bool processInitiated=false;
uint8_t _batteryValue=9;
float minTempValue=37.4,maxTempValue= 38.4,minHumidityIncubation=45.0, maxHumidityIncubation=55, minHatchHum=70, maxHatchHum=75;

////Variables for landing_battery

float hum;  //Stores humidity value
float temp; //Stores temperature value

// transmission variables
SoftwareSerial WiFiSerial(5, 6); //Change pins here  tx=5, rx=6 on pcb

unsigned long millisTime,secondsHolder=59;

int seconds,minutes=59,hours=1,daysCounter=0,day=1,incubationDays,hatchingDays;
int hoursCounter = 0, minutesCounter = 0;
bool startMotor=false;

//Buzzer
bool buzzerToneActive = false;

//PowerIn
bool powerInStatus=0;

bool startFanOut=false;

bool powerPriority;

void exit();
void action3();

void sendMasterData() {
    DynamicJsonDocument doc(512);
    JsonArray array = doc.to<JsonArray>();
    array.add(startFanOut);
    array.add(startMotor);
    array.add(temp);
    array.add(hum);
 //   array.add(incubationActive);
    array.add(powerPriority);
   // array.add(birdTypeNo);
    array.add(incubationDays);
    array.add(hatchingDays);
    array.add(day);
    serializeJson(doc, WiFiSerial);
    String sendStatus = "";
    sendStatus = WiFiSerial.readStringUntil('0');

} //Call this function where you want to send data //Call this function where you want to send data


void setup() {
    //EEPROM Setup Initialization
//   // birdTypeNo = EEPROM.read(1);
//   // incubationActive = EEPROM.read(2);
//    //navigationValue = EEPROM.read(3);
//    //conditionsSet = EEPROM.read(4);
//EEPROM.write(5,0);
    daysCounter = EEPROM.read(5);
    // EEPROM.write(6,19);
    day = EEPROM.read(6);
    incubationDays = EEPROM.read(7);
    hatchingDays = EEPROM.read(8);

    lcd.begin();
    lcd.clear();
    lcd.backlight();
    dht.begin();

    Serial.begin(9600);
    WiFiSerial.begin(9600);

    //variables in classes
    FN.fanSetup(8);  //pin 8 fanIn
    IS.IncubationMenuSetup();

    pinMode(igniter,OUTPUT);
    pinMode(humidifier,OUTPUT);
    pinMode(upButton,INPUT);
    pinMode(selectButton,INPUT);
    pinMode(downButton,INPUT);
    pinMode(heater,OUTPUT);
    pinMode(gasValve,OUTPUT);
    pinMode(powerIn,INPUT);
    pinMode(buzzer,OUTPUT);
    lcd.clear();
}

void loop() {
    millisTime = (millis() / 1000);
    seconds = secondsHolder - millisTime;
    if (seconds < 0) {
        Serial.println("*********************************");
        seconds = 59;
        minutes--;
        secondsHolder = millisTime + 59;

    }

    if (minutes < 0) {
        hours--;
        minutes = 59;
    }

    if (hours < 0) {
        hours = 1;
        daysCounter++;
        EEPROM.write(5, daysCounter);

        //motorPin
        if (day < incubationDays) {
            startMotor = true;
            motorTimer.reset();
        }

        if (daysCounter > 11) {           //days counter (0 - 11)
            daysCounter = 0;
            EEPROM.write(5, daysCounter);
            day++;
            if (day > (incubationDays + hatchingDays + 2)) {
                //resetIncubation();
                exit();
            }

            //TODO: check if this is not performed on the resetIncubation() method
            EEPROM.write(6, day);
        }
    }

    //Egg turning motor initiator
    if (motorTimer.isReady() && startMotor==true) {
        startMotor = false;
        motorTimer.reset();
    }

    hum = dht.readHumidity();
    temp= dht.readTemperature();
    state1=digitalRead(upButton);
    state2=digitalRead(downButton);
    state3=digitalRead(selectButton);

    //power
    powerInStatus = digitalRead(powerIn);

    // send data to slave microController;
    sendMasterData();

    if (powerInStatus) {
        powerPriority = 0;        //electricity
    } else {
        powerPriority = 1;          // gas
    }


    if(idle==1){

        if(processInitiated==false){
            lcd.setCursor(0,1);
            lcd.println("AFRITECH INCUBATORS ");
           //TODO: MK03 lcd.setCursor()
            Serial.print("hmm");
            FN.stopFan();
            digitalWrite(heater,LOW);
            digitalWrite(gasValve,LOW);
            digitalWrite(igniter,LOW);
        }


        if(processInitiated==true){

            IS.IncubationScreen(temp,90,hum,10,60.5,hours,minutes,seconds);
            Serial.print("Time: ");
            Serial.println(seconds);
            FN.startFan();
            //digitalWrite(humidifier,HIGH);
            //    FN.startFan();
            //    digitalWrite(igniter,HIGH);
            //    delay(30000);
            //    FN.stopFan();
            //    digitalWrite(igniter,LOW);
            //    digitalWrite(humidifier,LOW);
            //    delay(10000);

            //temp Check
            if (temp <= minTempValue) {

//TODO:Buzzer Alarm disabled for sabasaba event:

                if (buzzerToneActive == true) {

                    digitalWrite(buzzer, HIGH);          // changed here
                } else {
                    digitalWrite(buzzer, LOW);

                }


                if (powerPriority == 0) {
//  // switch on the heaters because there is power
                    digitalWrite(heater, HIGH);
                   // ignitionStarted = false;
                    digitalWrite(gasValve, LOW);

                }


                if (powerPriority == 1) {       //TODO: switch on gas
//                    if (ignitionStarted == false) {
//                        ignitionStarted = true;
//                        sparkCounter = 0;
//                        // =====================
//                        digitalWrite(heater, LOW);
//                        ignitionTimer.reset();
//                    }
                }
            } else {
                noTone(buzzer);

                //===========================
            }

            if (temp == 37.5) {
                if (powerPriority == 0) {
                    //=========================
                    digitalWrite(heater, HIGH);     // electricity on
                    digitalWrite(igniter, LOW);
                    digitalWrite(gasValve, LOW);

                }
            }

            if (temp >= 38.2 && powerPriority == 0) {
                //TODO:=======================
                digitalWrite(heater, LOW);
            }
            if (temp >= maxTempValue) {
                //TODO: find what ignitionStarted meant
              //  ignitionStarted = false;
                digitalWrite(gasValve, LOW);
                digitalWrite(igniter, LOW);
                digitalWrite(heater, LOW);

            }

//======================================

//Buzzer Timer counter
            if (firstTimer.isReady()) {

                if (temp < 37.2) {
                    buzzerToneActive = !buzzerToneActive;
                } else {
                    buzzerToneActive = false;
                }

                firstTimer.reset();
            }

            //TODO: igniter timer goes here


            if (temp > 38.8) {

                startFanOut = true;
            } else {

                startFanOut = false;
            }
            //humidity check
            if (day <= incubationDays) {

                if (hum < minHumidityIncubation) {

                    digitalWrite(humidifier, HIGH);
                }

                if (hum >= maxHumidityIncubation) {
                    digitalWrite(humidifier, LOW);
                }

            } else {

                if (hum < minHatchHum) {
                    digitalWrite(humidifier, HIGH);
                }

                if (hum >= maxHatchHum) {
                    digitalWrite(humidifier, LOW);
                }
            }

        }

    }

    ////this is up upButton
    if(state1==1){
        Serial.print("up button working fine");

        if(okButton>0 && subMenu==0){
            idle++;
            menu--;
            if(menu<1)
                menu=4;
            MF.MainMenu(menu);
            Serial.println(menu);
            Serial.println("@@@@@@@");
        }

        if(subMenu>0){
            subMenu--;
            if(subMenu<1 && menu==1){

                subMenu=3;}

            if(subMenu<1 && menu==2){

                subMenu=2;}

            if(menu==1)
                MF.SubMainMenuIncubation(subMenu);

            if(menu==2)
                MF.SubMainMenuSettings(subMenu);

            if(okButton==3){
                startStatus--;

                if(startStatus<1 &&menu==1)
                    startStatus=2;

                if(startStatus<1 && menu==2)
                    startStatus=3;

                if(menu==1)
                    MF.Start_Back(processInitiated,startStatus);

                if(menu==2)
                    MF.languageSettings(startStatus);
                Serial.print("Jeshi");
            }
        }

        delay(200);

    }


////==============================================================================================
//ok button Pressed
    if(state3==1){
        Serial.print("ok button working fine");
        okButton++;
        delay(200);
        if(okButton==1){
            idle++;
            MF.MainMenu(menu);
            Serial.println("first place");
            Serial.println(okButton);

        }

        if(okButton==2){
            subMenu++;
            Serial.println("second place");
            Serial.println(okButton);
            Serial.println(subMenu);
            Serial.println(menu);
            switch (menu) {
                case 1:
                    MF.SubMainMenuIncubation(subMenu);
                    break;
                case 2:
                    MF.SubMainMenuSettings(subMenu);
                    break;
                case 3:
                    action3();
                    break;
                case 4:
                    exit();
                    break;
            }
        }



        if(okButton==3){

//returning back to main menu
            if(subMenu==3){
                okButton=1;
                Serial.print("this is running now subMenu ==3");
                subMenu=0;
                menu=1;
                MF.MainMenu(menu);
            }

            if(menu==2 && subMenu==2){
                okButton=1;
                subMenu=0;
                menu=1;
                MF.MainMenu(menu);
            }


//going to start or stop the incubation process
            if(menu==1 && (subMenu==1 || subMenu==2)){
                Serial.println("Sub.");
                Serial.println(subMenu);
                MF.Start_Back(processInitiated,startStatus);
            }

//change the language
            if(menu==2 && subMenu==1){

                MF.languageSettings(startStatus);
            }

        }


//initiating the incubation process ... starting up the fans .. heater ..egg turner etc
        if(okButton==4 && startStatus==1 && menu==1){
            lcd.clear();

            if(subMenu==1){
                lcd.setCursor(0,2);
                lcd.print("Chicken...");
                processInitiated=!processInitiated;
                EEPROM.write(2, processInitiated);

                firstTimer.reset();
                secondsHolder = millisTime + 59;
                seconds = secondsHolder - millisTime;
                minutes = 59;
                hours = 1;

                daysCounter = 0;
                EEPROM.write(5, daysCounter);

                day = 1;
                EEPROM.write(6, day);
                delay(1000);

            }

            if(subMenu==2){
                lcd.setCursor(0,2);
                lcd.print("GuineaFowl..");
                processInitiated=!processInitiated;
                delay(1000);
            }

            lcd.clear();
            idle=1;
            okButton=0;
            menu=1;
            subMenu=0;
        }

//back from the start or stop of incubation process
        if(okButton==4 && startStatus==2 &&menu==1){
            Serial.println("imefika hapa");
            okButton=2;
            lcd.clear();
            subMenu=1;
            startStatus=1;
            MF.SubMainMenuIncubation(subMenu);
        }

//back from languageSettings
        if(okButton==4 && menu==2 && startStatus==3){
            Serial.println("jjjj");
            Serial.println(startStatus);
            okButton=2;
            lcd.clear();
            subMenu=1;
            menu=2;
            startStatus=1;
            MF.SubMainMenuSettings(subMenu);
        }


        if(okButton==4 && menu==2 &&(startStatus==2 || startStatus==1)){

            if(startStatus==1){
                lcd.clear();
                lcd.setCursor(2,1);
                lcd.println("Please Wait...");
                delay(1000);
                exit();
            }

            if(startStatus==2){
                lcd.clear();
                lcd.setCursor(2,1);
                lcd.println("Tafadhali Subiri...");
                delay(1000);
                exit();
            }
        }

    }



////=============================================================================================

//this is the downButton Arrow
    if(state2==1){
        Serial.println("State1 down");

        if(subMenu==0 && okButton>0){
            idle++;
            menu++;
            if(menu>4)
                menu=1;
            lcd.clear();
            MF.MainMenu(menu);
            Serial.println(menu);}


        if(subMenu>0){
            subMenu++;
            if(subMenu>3 && menu==1)
                subMenu=1;
            if(subMenu>2 && menu==2)
                subMenu=1;
            Serial.print("mastertony is great");
            if(menu==1)
                MF.SubMainMenuIncubation(subMenu);

            if(menu==2)
                MF.SubMainMenuSettings(subMenu);

            if(okButton==3){
                startStatus++;
                if(startStatus>2 && menu==1)
                    startStatus=1;
                if(startStatus>3 && menu==2)
                    startStatus=1;

                if(menu==1)
                    MF.Start_Back(processInitiated,startStatus);

                if(menu==2)
                    MF.languageSettings(startStatus);
            }
        }

        delay(200);
    }

////==================================================================================================
}

void exit() {
    lcd.clear();
    idle=1;
    okButton=0;
    subMenu=0;
    menu=1;

    daysCounter = 0;
    EEPROM.write(5, daysCounter);

    day = 1;
    EEPROM.write(6, day);
}

void action3() {

    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("Still under Dev...");
    delay(1000);
    okButton=1;
    menu=1;
    subMenu=0;
    MF.MainMenu(menu);

}
