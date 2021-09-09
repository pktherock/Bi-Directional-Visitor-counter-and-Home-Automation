// Bi-Directional Visitor Counter with Home Automation!
// --> It will count how many students are in the classroom.
// --> It displays the no of students in the classroom.
// --> And on the basis of the counter value, it works automatically as well as in-person too!


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define trigPin2 2
#define out 3
#define allowed 4
#define trigPin1 5
#define in 6
//#define buzzer 7
#define L293D_exit1 7
#define fan2 8
#define fan1 9
#define led2 10
#define led1 11
//#define wrong 12
#define L293D_exit2 12
#define full 13
#define entryLaser A0
#define exitLaser A1
#define L293D_entry1 A2
#define L293D_entry2 A3


char val;
int count = 0;
int watch = 1 ;
int jarvis = 0;
int ironMan = 0;
int entryCounterState = 0;
int exitCounterState = 0;
int gate1 = 0;
int gate2 = 0;

//Initial SetUp
void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Visitor Counter");
  lcd.setCursor(0, 1);
  lcd.print("   PCE Purnea");
  delay(2000);
  lcd.clear();
  lcd.print("Person In Room:");
  lcd.setCursor(0, 1);
  lcd.print(count);
  pinMode(trigPin1, OUTPUT);
  pinMode(in, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(out, INPUT);
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(fan1, OUTPUT);
  pinMode(fan2, OUTPUT);
//  pinMode(wrong, OUTPUT);
  pinMode(full, OUTPUT); // status of students in the classroom
  pinMode(allowed, OUTPUT); // if empty or less then 10 students then allowed will be enable
  pinMode(entryLaser, INPUT);
  pinMode(exitLaser, INPUT);
  pinMode(L293D_entry1, OUTPUT);
  pinMode(L293D_entry2, OUTPUT);
  pinMode(L293D_exit1, OUTPUT);
  pinMode(L293D_exit2, OUTPUT);
//  pinMode(buzzer, OUTPUT);
  digitalWrite(entryLaser, LOW);
  digitalWrite(exitLaser, LOW);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(fan1, HIGH);
  digitalWrite(fan2, HIGH);
  digitalWrite(full, LOW);
  digitalWrite(allowed, HIGH);
//  digitalWrite(wrong, LOW);
//  digitalWrite(buzzer, LOW);
}
//Initial SetUp ends


// Function of Entry gate! start
void IN()
{
  if (count < 15) {
    digitalWrite(allowed, HIGH);
    digitalWrite(full, LOW);
    long duration, distance;
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    duration = pulseIn(in, HIGH);
    distance = (duration / 2) / 29.1;
    Serial.write(distance);
    if (distance < 5) {
      jarvis = 1;
      gate1 = 1;
      digitalWrite(L293D_entry1, HIGH);
      digitalWrite(L293D_entry2, LOW);
      delay(1000);
      digitalWrite(L293D_entry1, LOW);
      digitalWrite(L293D_entry2, LOW);
    } else {
      if (jarvis == 1 && !(distance < 5) && entryCounterState == 0) {
        digitalWrite(L293D_entry1, LOW);
        digitalWrite(L293D_entry2, HIGH);
        delay(1000);
//        tone(buzzer, 1000);
//        delay(1000);
//        noTone(buzzer);
        jarvis = 0;
      }
    }
    if (digitalRead(entryLaser) != HIGH) {
      entryCounterState = 1;
    } else {
      if (entryCounterState == 1 && digitalRead(entryLaser) == HIGH && gate1 == 1) {
        count++;
        delay(1000);
        lcd.clear();
        lcd.print("Person In Room:");
        lcd.setCursor(0, 1);
        lcd.print(count);
        delay(1000);
        entryCounterState = 0;
        gate1 = 0;
      }
    }
  }
  digitalWrite(L293D_entry1, LOW);
  digitalWrite(L293D_entry2, LOW);
  digitalWrite(allowed, LOW);
  digitalWrite(full, HIGH);
}
// Function of Entry gate! ends


// Function of Exit gate!
void OUT() {
  if (count > 0) {
    long duration1, distance1;
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    duration1 = pulseIn(out, HIGH);
    distance1 = (duration1 / 2) / 29.1;
    if (distance1 < 5) {
      ironMan = 1;
      gate2 = 1;
      digitalWrite(L293D_exit1, HIGH);
      digitalWrite(L293D_exit2, LOW);
      delay(1000);
      digitalWrite(L293D_exit1, LOW);
      digitalWrite(L293D_exit2, LOW);
    } else {
      if (ironMan == 1 && !(distance1 < 5) && exitCounterState == 0) {
        digitalWrite(L293D_exit1, LOW);
        digitalWrite(L293D_exit2, HIGH);
        delay(1000);
//        tone(buzzer, 1000);
//        delay(1000);
//        noTone(buzzer);
      }
    }
    if (digitalRead(exitLaser) != HIGH) {
      exitCounterState = 1;
    } else {
      if (exitCounterState == 1 && digitalRead(exitLaser) == HIGH && gate2 == 1) {
        count--;
        delay(1000);
        if (count < 0)
          count = 0;
        if (count > 0) {
          lcd.clear();
          lcd.print("Person In Room:");
          lcd.setCursor(0, 1);
          lcd.print(count);
          delay(1000);
        }
        exitCounterState = 0;
        gate2 = 0;
      }
    }
  }
  digitalWrite(L293D_exit1, LOW);
  digitalWrite(L293D_exit2, LOW);
}
// Function of Exit gate! ends


// Loop which will run again and again!
void loop()
{
  if (count <= 0) {
    count = 0;
    if ( watch == 1 ) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nobody in room");
      lcd.setCursor(0, 1);
      lcd.print("Light Is Off");
      digitalWrite(led1, HIGH );
      digitalWrite(led2, HIGH );
      digitalWrite(fan1, HIGH );
      digitalWrite(fan2, HIGH );
      watch = 0;
    }
  } else if (count > 0 && count < 2) {
    watch = 1;
    if (digitalRead(led1) == HIGH) {
      digitalWrite(led1, LOW );
    }
    digitalWrite(led2, HIGH);
    digitalWrite(fan1, HIGH);
    digitalWrite(fan2, HIGH);
  } else {
    if (Serial.available()) {
      val = Serial.read();
      if (val == 'A') //in case of 'A' turn the LED1 on
      {
        digitalWrite(led1, LOW);

      }
      else if (val == 'a')  //in case of 'a' turn the LED1 off
      {
        digitalWrite(led1, HIGH);
      }
      else if (val == 'B') //in case of 'B' turn the LED2 on
      {
        digitalWrite(led2, LOW);
      }
      else if (val == 'b')  //incase of 'b' turn the LED2 off
      {
        digitalWrite(led2, HIGH);
      }
      else if (val == 'C') //in case of 'C' turn the FAN1 on
      {
        digitalWrite(fan1, LOW);
      }
      else if (val == 'c')  //incase of 'c' turn the FAN1 off
      {
        digitalWrite(fan1, HIGH);
      }
      else if (val == 'D') //in case of 'D' turn the FAN2 on
      {
        digitalWrite(fan2, LOW);
      }
      else if (val == 'd')  //incase of 'd' turn the FAN2 off
      {
        digitalWrite(fan2, HIGH);
      }
      else
      {
//        digitalWrite(wrong, HIGH);
//        delay(100);
//        digitalWrite(wrong, LOW);

      }
      val = 0x00;
      delay(1000);
    }
  }
  IN();
  OUT();
}
