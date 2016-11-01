#include <PhotodiodeTrigger.h>
#include <LaserRadar.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,2);

int laserPin = 12;
int laserPin2 = 11;

int startOutput = 2;
int endOutput = 3;

int laserBrokenValue = 1020;
float distance = 10.0;

float best = 0.0;

PhotodiodeTrigger startPhotodiode(5, laserBrokenValue);
PhotodiodeTrigger endPhotodiode(4, laserBrokenValue);
LaserRadar radar(startPhotodiode, endPhotodiode, distance);

// the setup routine runs once when you press reset:
void setup() { 
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, HIGH);
  pinMode(laserPin2, OUTPUT);
  digitalWrite(laserPin2, HIGH);

  pinMode(startOutput, OUTPUT); 
  pinMode(endOutput, OUTPUT); 
  digitalWrite(startOutput, HIGH);
  digitalWrite(endOutput, HIGH);
  
  lcd.init();
  lcd.backlight();

  best = 0.0;
  
  // Print a message to the LCD.
  lcd.print("Best: ");
  lcd.print(best, 2);
  lcd.print(" KM/H");
  
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  float speed = radar.check();

  if (0.00 != speed) {
    Serial.print(speed, 6);
    Serial.println(" km/h");
    
    if (speed >= best) {
      best = speed;
    }
      
    reDrawDisplay(best, speed);
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
