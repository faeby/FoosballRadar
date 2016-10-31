#include <LiquidCrystal.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
//LiquidCrystal lcd(2,3,4,5,6,7);
LiquidCrystal_I2C lcd(0x27,20,2);

int laserPin = 12;
int laserPin2 = 11;

int startOutput = 2;
int endOutput = 3;

int startLaserPin = 5;
int endLaserPin = 4;
int laserBrokenValue = 1020;

int startTrigger = -1;
int endTrigger = -1;

bool startLaserBrokenCurrent;
bool startLaserBrokenLast;
bool endLaserBrokenCurrent;
bool endLaserBrokenLast;

float best = 0.0;

// Distance between the 2 lasers in cm
float laserDistance = 10;

// the setup routine runs once when you press reset:
void setup() { 
  startLaserBrokenCurrent = false;
  startLaserBrokenLast = false;
  endLaserBrokenCurrent = false;
  endLaserBrokenLast = false;

  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, HIGH);
  pinMode(laserPin2, OUTPUT);
  digitalWrite(laserPin2, HIGH);

  pinMode(startOutput, OUTPUT); 
  pinMode(endOutput, OUTPUT); 
  digitalWrite(startOutput, HIGH);
  digitalWrite(endOutput, HIGH);
  
  // set up the LCD's number of columns and rows: 
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  //lcd.begin(16, 2);

  best = 0.0;
  
  // Print a message to the LCD.
  lcd.print("Best: ");
  lcd.print(best, 2);
  lcd.print(" KM/H");
  
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {

  //int myValue = analogRead(startLaserPin);
  //Serial.print("test: ");
  //Serial.println(myValue);
  
  // Read PhotoDiod Start
  startLaserBrokenCurrent = analogRead(startLaserPin) <= laserBrokenValue;
  
  if (startLaserBrokenCurrent && !startLaserBrokenLast) {
    startTrigger = millis();
    Serial.print("Start Laser: broken, timer started at ");
    Serial.println(startTrigger);
    startLaserBrokenLast = true;
  } else if (!startLaserBrokenCurrent && startLaserBrokenLast) {
    Serial.println("Start Laser: No more broken");
    startLaserBrokenLast = false;
  }
  
  endLaserBrokenCurrent = analogRead(endLaserPin) <= laserBrokenValue;
  
  if (endLaserBrokenCurrent && !endLaserBrokenLast) {
    Serial.println("End Laser: broken");
    if (startTrigger != -1) {
      endTrigger = millis();
      int time = endTrigger - startTrigger;
      Serial.print("Time between 2 lasers: ");
      Serial.print(time);
      Serial.println(" ms");
      
      float centimeterPerMillis = laserDistance / time;
      
      float lastSpeed = centimeterPerMillisToKmph(centimeterPerMillis);
      Serial.print(lastSpeed, 6);
      Serial.println(" km/h");
      
      if (lastSpeed >= best) {
        best = lastSpeed;
      }
      
      reDrawDisplay(best, lastSpeed);
      
      
      startTrigger = -1;
    }
    
    endLaserBrokenLast = true;
  } else if (!endLaserBrokenCurrent && endLaserBrokenLast) {
    Serial.println("End Laser: No more broken");
    endLaserBrokenLast = false;
  }
  
}

void reDrawDisplay(float best, float current) {
  lcd.clear();
        
  lcd.setCursor(0,0);
  lcd.print("Best: ");
  lcd.print(best, 2);
  lcd.print(" KM/H");
      
  lcd.setCursor(0,1);
  lcd.print("Curr: ");
  lcd.print(current, 2);
  lcd.print(" KM/H"); 
}

float centimeterPerMillisToKmph(float centimeterPerMillis) {  
  /*double cmPerS = centimeterPerMillis * 1000;
  double cmPerH = cmPerS * 3600;
  double mPerH  = cmPerH / 100;
  double kmPerH = mPerH / 1000;*/
  
  return centimeterPerMillis * 36;
}
