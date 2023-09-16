/*
  LiquidCrystal Library
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

*/

#include <LiquidCrystal.h>
#include <Stepper.h>
#include <SR04.h>


// Arduino pin numbers
const int SW_pin = 8; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output


int AutoSwitch = 22;
int AnalogSwitch = 23;
#define AutoLED 29
#define AnalogLED 28

#define TRIG_PIN 7
#define ECHO_PIN 6
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the Stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 50, 51, 52, 53);


void setup() {

  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  pinMode (AutoSwitch,INPUT_PULLUP);
  pinMode (AnalogSwitch,INPUT_PULLUP);
  pinMode(AutoLED,OUTPUT);
  pinMode(AnalogLED,OUTPUT);

  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
   // set the speed at 60 rpm:
  myStepper.setSpeed(100);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2); 
  lcd.print("Distance: ");
   Serial.begin(9600);
}

void loop() {


  a=sr04.Distance();
  if (digitalRead(AutoSwitch) == LOW)
  {
     lcd.setCursor(0, 1);
    lcd.print("Automatic");
    do{
      digitalWrite(AutoLED, HIGH);
      digitalWrite(AnalogLED, LOW);
      if(a!=10){
        do{
          digitalWrite(10,HIGH);
          //step one revolution in the other direction:
          // Serial.println("counterclockwise");
          myStepper.step(-stepsPerRevolution);
          delay(1);
          a=sr04.Distance(); 
          lcd.setCursor(11, 0);
          lcd.print(a);
          if(digitalRead(AnalogSwitch)== LOW){
            break;
          }
        }while(a!=10);
      }
      if(a==10){
        digitalWrite(10,LOW);
        digitalWrite(9,HIGH);
        delay(800);
        digitalWrite(9,LOW);
        digitalWrite(10,HIGH);
        do{
            // step one revolution in one direction:
            myStepper.step(stepsPerRevolution);
            delay(1);
            a=sr04.Distance();
            lcd.setCursor(11, 0);
            lcd.print(a);
            if(digitalRead(AnalogSwitch)== LOW){
              break;
            }
          }while(a!=29);
      }
    }while(digitalRead(AnalogSwitch)!= LOW);
  }

  if (digitalRead(AnalogSwitch)== LOW)
    {
      lcd.setCursor(0, 1);
      lcd.print("Manual   ");
      digitalWrite(AnalogLED, HIGH);
      digitalWrite(AutoLED, LOW);
      
      do{
        int X_POS_STILL = 535;
        
        if(analogRead(X_pin)<X_POS_STILL && a!=10){
          digitalWrite(10,HIGH);
          digitalWrite(9,LOW);
          myStepper.step(-stepsPerRevolution);
          a=sr04.Distance(); 
          lcd.setCursor(11, 0);
          lcd.print(a);
        }
        if(analogRead(X_pin)>X_POS_STILL && a!=29){
          digitalWrite(10,HIGH);
          digitalWrite(9,LOW);
          myStepper.step(stepsPerRevolution);
          a=sr04.Distance(); 
          lcd.setCursor(11, 0);
          lcd.print(a);
        }
        if(a==10 || a ==29){
          digitalWrite(10,LOW);
          digitalWrite(9,HIGH);
        }
        if(digitalRead(AutoSwitch)== LOW){
          break;
        }
        delay(1);
      }while(digitalRead(AutoSwitch)!= LOW);
    }
}

