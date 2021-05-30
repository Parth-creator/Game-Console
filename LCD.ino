#include <Wire.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <stdarg.h>
//#include <DateTimeStrings.h>
#include <LedControl.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN 13
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int DIN = 10;
int CS =  9;
int CLK = 8;

LedControl lc=LedControl(DIN,CLK,CS,1);

int prevRightVal = 1;
int newRightVal = 1;

int prevLeftVal = 1;
int newLeftVal = 1;

int prevOkVal = 1;
int newOkVal = 1;

int prevHomeVal = 1;
int newHomeVal = 1;

int prevUpVal = 1;
int newUpVal = 1;

int prevDownVal = 1;
int newDownVal = 1;

int rightBtnPin = 2;
int rightBtnVal;

int leftBtnPin = 3;
int leftBtnVal;

int okBtnPin = 4;
int okBtnVal;

int homeBtnPin = 12;
int homeBtnVal;

int upBtnPin = 6;
int upBtnVal;

int downBtnPin = 7;
int downBtnVal;

int bluetoothPin = 5;
int prevBluetoothVal = 0;
int newBluetoothVal = 0;

int humidity;
int temperature;
int recieved;
String recievedString = "";

int currentOption = 0;
int gameOption = 0;

int flashPin = 11; 
int splashScreenDelay = 3000;
int bluetooth;


int spaceshipRow = 7;
int spaceshipColumn = 3;

int bulletRow = -1;
int bulletColumn = -1;
unsigned long previousBulletMillis = 0;
int bulletSpeed = 100;

bool isFlashOn = false;
bool isBluetoothConnected = false;
bool isBulletActive = false;

int enemyRow[] = {
  0,
  0,
};

int enemyColumn[] = {
  0,
  1,
};
unsigned long previousEnemyMillis = 0;
int enemySpeed = 500;
int enemyVelocity = 1;

String menuOptions[] = {
  "Temperature", "Games", "Flashlight"  
};

String gameOptions[] = {
  "Snake Game", "Space Shooters"
};

String screenState = "home";

byte rightArrow[] = {
  B10000,
  B11000,
  B11100,
  B11110,
  B11110,
  B11100,
  B11000,
  B10000
};

byte bluetoothOn[] = {
  B00100,
  B00110,
  B00101,
  B00110,
  B00110,
  B00101,
  B00110,
  B00100
};

byte bluetoothOff[] = {
  B00100,
  B00110,
  B10101,
  B01110,
  B01110,
  B10101,
  B00110,
  B00100
};

byte dot[] = {
  B00001000,
  B00010000,
  B01111110,
  B10000001,
  B10111101,
  B10100101,
  B11000011,
  B00000000
};

byte dotOff[] ={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

void updateDisplay(String state = " ") {
  lcd.clear();
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  if(state == "right" && screenState == "home"){
    lcd.clear();
        if(currentOption == sizeof(menuOptions)/sizeof(String)-1){
          currentOption = 0;
        }else{
          currentOption++; 
        }
        lcd.setCursor(10, 0);
        if(bluetooth != 0){
          lcd.write(1);
         }else{
          lcd.write(2);
         }
        lcd.setCursor(0, 1);
        lcd.write(0);
        Serial.println(isFlashOn);
        if (currentOption == 2){
          if (isFlashOn == true){
            lcd.print(menuOptions[currentOption] + " ON");
          }
          if (isFlashOn == false){
            lcd.print(menuOptions[currentOption] + " OFF");
          }          
        }else{
          lcd.print(menuOptions[currentOption]);
        } 
  }
  if(state == "right" && screenState == "game"){
    lcd.clear();
    if(gameOption == sizeof(gameOptions)/sizeof(String)-1){
       gameOption = 0;
    }else{
       gameOption++; 
    }
    lcd.setCursor(0, 0);
    lcd.print("     Games     ");
    lcd.setCursor(0, 1);
    lcd.write(0);
    lcd.print(gameOptions[gameOption]);
  }
  
  if(state == "right" && screenState == "ss"){
    lcd.clear();
    spaceshipColumn = spaceshipColumn + 1;
    if(spaceshipColumn >= 8){
    spaceshipColumn = 7;
//    spaceshipRow = 3;
    lc.setLed(0, spaceshipRow, spaceshipColumn, HIGH);
    }else{      
      lc.setLed(0, spaceshipRow, spaceshipColumn, HIGH);
    }
  }
  
  if(state == "left" && screenState == "home"){
    lcd.clear();
        if(currentOption == 0){
          currentOption = sizeof(menuOptions)/sizeof(String)-1;
        }else{
          currentOption = currentOption - 1; 
        }
        lcd.setCursor(10, 0);
         if(bluetooth != 0){
          lcd.write(1);
         }else{
          lcd.write(2);
         }
        lcd.setCursor(0, 1);
        lcd.write(0);
        if (currentOption == 2){
          if (isFlashOn == true){
            lcd.print(menuOptions[currentOption] + " ON");
          }
          if (isFlashOn == false){
            lcd.print(menuOptions[currentOption] + " OFF");
          }       
        }else{
          lcd.print(menuOptions[currentOption]);
        }
  }
  if(state == "left" && screenState == "game"){
    lcd.clear();
    if(gameOption == 0){
       gameOption = sizeof(gameOptions)/sizeof(String)-1;
    }else{
       gameOption = gameOption - 1; 
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("     Games     ");
    lcd.setCursor(0, 1);
    lcd.write(0);
    lcd.print(gameOptions[gameOption]);
  }
  
  if(state == "left" && screenState == "ss"){
    lcd.clear();
    spaceshipColumn = spaceshipColumn - 1;
    if(spaceshipColumn <= 0){
//      for(int r = 0; r < 8; r++){
//        for(int c = 0; c < 8; c++){
//            lc.setLed(0, r, c, HIGH);
//            delay(50);
//        }
//        lcd.setCursor(0, 0);
//        lcd.print("GAME OVER");
//      }
    spaceshipColumn = 0;
//    spaceshipRow = 3;
    lc.setLed(0, spaceshipRow, spaceshipColumn, HIGH);
    }else{
      lc.setLed(0, spaceshipRow, spaceshipColumn, HIGH);
    }
  }

  if(state == "up" && screenState == "ss"){
    lcd.clear();
    spaceshipRow = spaceshipRow - 1;
    if(spaceshipRow <= -1){
    spaceshipRow = 0;
//    spaceshipRow = 3;
    lc.setLed(0, spaceshipRow, spaceshipColumn, HIGH);
    }else{      
      lc.setLed(0, spaceshipRow, spaceshipColumn, HIGH);
    }
  }

  if(state == "up" && screenState == "home"){
    lcd.clear();
    printByte(dotOff);
    lcd.setCursor(10, 0);
    if(bluetooth != 0){
      lcd.write(1);
    }else{
      lcd.write(2);
    }
    lcd.setCursor(0, 1);
    lcd.write(0);
    lcd.print(menuOptions[currentOption]);
    gameOption = 0;
  }
  
  if(state == "down" && screenState == "ss"){
    lcd.clear();
    spaceshipRow = spaceshipRow + 1;
    if(spaceshipRow >= 8){
    spaceshipRow = 7;
//    spaceshipRow = 3;
    lc.setLed(0, spaceshipRow, spaceshipColumn, HIGH);
    }else{      
      lc.setLed(0, spaceshipRow, spaceshipColumn, HIGH);
    }
  }

  if(state == "down" && screenState == "home"){
    lcd.clear();
    printByte(dotOff);
    lcd.setCursor(10, 0);
    if(bluetooth != 0){
      lcd.write(1);
    }else{
      lcd.write(2);
    }
    lcd.setCursor(0, 1);
    lcd.write(0);
    lcd.print(menuOptions[currentOption]);
    gameOption = 0;
  }
  
  if(state == "none" && screenState == "home"){
    lcd.clear();
//    currentOption = 0;
    printByte(dotOff);
    lcd.setCursor(10, 0);
    if(bluetooth != 0){
      lcd.write(1);
    }else{
      lcd.write(2);
    }
    lcd.setCursor(0, 1);
    lcd.write(0);
    lcd.print(menuOptions[currentOption]);
    gameOption = 0;
//    Serial.println((sizeof(menuOptions)/sizeof(String))-1);
  }
  
  if(state == "ok"){
    if(currentOption == 0){
      screenState = "temp";
      updateDisplay();
    }
    if(currentOption == 2){
      if(isFlashOn == true){
        screenState = "flashOff";
      }
      if(isFlashOn == false){
        screenState = "flashOn";
      }
      updateDisplay();
    }
    if(currentOption == 1){
      screenState = "game";
    }
    if(gameOption == 1){
      screenState = "ss";
    }
    if(screenState == "ss"){
      if(isBulletActive == false){
        bulletRow = spaceshipRow - 1;
        bulletColumn = spaceshipColumn;
        isBulletActive = true;
      }
    }
  }
  if (screenState == "temp"){
    lcd.setCursor(0, 0);
    lcd.print("Humidity: ");
    lcd.print(humidity);
//   Serial.println(humidity);
    lcd.setCursor(0, 1);
    lcd.print("Temperature: ");
    lcd.print(temperature);
    lcd.print("C");
//      Serial.println(temperature);
//      lcd.print("Temperature");
  }
  if(screenState == "game"){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("     Games     ");
    lcd.setCursor(0, 1);
    lcd.write(0);
    lcd.print(gameOptions[gameOption]);

    printByte(dot);
  }
  
  if(screenState == "ss"){
     lcd.clear();
     lc.clearDisplay(0);
     lcd.setCursor(0, 0);
     lcd.print("Playing");
     lcd.setCursor(0, 1);
     lcd.print("SPACE SHOOTERS");
     lc.setLed(0, spaceshipRow, spaceshipColumn, HIGH);
  }
  
  if (screenState == "flashOn"){
      digitalWrite(flashPin, HIGH);
      screenState = "home";
      isFlashOn = true;
      currentOption = 0;
      delay(250);
      updateDisplay("none");   
  }
  if (screenState == "flashOff"){
    digitalWrite(flashPin, LOW);
    screenState = "home";
    isFlashOn = false;
    currentOption = 0;
    delay(100);
    updateDisplay("none");  
  }
}

void setup(){
  Serial.begin(9600);
  dht.begin();
  lc.shutdown(0,false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);  
  lcd.begin();
  lcd.print(" MOBILE Console");
  delay(3000);
  lcd.home();
  lcd.noCursor();
  lcd.createChar(0, rightArrow);
  lcd.createChar(1, bluetoothOff);
  lcd.createChar(2, bluetoothOn);
  pinMode(rightBtnPin, INPUT_PULLUP);
  pinMode(leftBtnPin, INPUT_PULLUP);
  pinMode(okBtnPin, INPUT_PULLUP);
  pinMode(homeBtnPin, INPUT_PULLUP);
  pinMode(upBtnPin, INPUT_PULLUP);
  pinMode(downBtnPin, INPUT_PULLUP);
  pinMode(flashPin, OUTPUT);
  pinMode(bluetoothPin, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  updateDisplay("none");
}

void loop(){
  unsigned long currentMillis = millis();
  bluetooth = digitalRead(bluetoothPin);
  prevBluetoothVal = newBluetoothVal;
  newBluetoothVal = bluetooth;
  if(bluetooth != 0){
    isBluetoothConnected = true;
  }else{
    isBluetoothConnected = false;
  }
  if(prevBluetoothVal != newBluetoothVal){
    updateDisplay("none");
  }
  if (Serial.available() > 0){
        recieved = Serial.read();
        recievedString = Serial.readStringUntil('\n');
  }
  rightBtnVal = digitalRead(rightBtnPin);
  leftBtnVal = digitalRead(leftBtnPin);
  homeBtnVal = digitalRead(homeBtnPin);
  okBtnVal = digitalRead(okBtnPin);
  upBtnVal = digitalRead(upBtnPin);
  downBtnVal = digitalRead(downBtnPin);
  
  prevRightVal = newRightVal;
  newRightVal = rightBtnVal;

  prevLeftVal = newLeftVal;
  newLeftVal = leftBtnVal;
   
  prevOkVal = newOkVal;
  newOkVal = okBtnVal;

  prevUpVal = newUpVal;
  newUpVal = upBtnVal;

  prevDownVal = newDownVal;
  newDownVal = downBtnVal;
  
  prevHomeVal = newHomeVal;
  newHomeVal = homeBtnVal;
  
  if(newRightVal != prevRightVal){
      if(rightBtnVal == 0){
        updateDisplay("right");
      }   
  }
  
  if(newLeftVal != prevLeftVal){
      if(leftBtnVal == 0){
        updateDisplay("left");
      }   
  }
  if(newOkVal != prevOkVal){
      if(okBtnVal == 0){
        updateDisplay("ok");
      }
  }
  
  if(newUpVal != prevUpVal){
      if(upBtnVal == 0){
        updateDisplay("up");
      }   
  }
  
  if(newDownVal != prevDownVal){
      if(downBtnVal == 0){
        updateDisplay("down");
      }   
  }
  if(newHomeVal != prevHomeVal){
      if(homeBtnVal == 0){
        screenState = "home";
        currentOption = 0;
        gameOption = 0;
        updateDisplay("none");
       }
  }

  if(bulletRow != -1 && bulletColumn != -1){
    lc.setLed(0, bulletRow, bulletColumn, HIGH);
  }

  if(bulletRow < 0){
    bulletRow = -1;
    isBulletActive = false;
  }

 if (currentMillis - previousBulletMillis >= bulletSpeed) {
   if(bulletRow != -1 && bulletColumn != -1){
     lc.setLed(0, bulletRow, bulletColumn, LOW);
     bulletRow = bulletRow - 1;
     for(int index = 0; index < (sizeof(enemyRow)/sizeof(int));index++){
       if(enemyRow[index] == bulletRow && enemyColumn[index] == bulletColumn){
        lc.setLed(0, bulletRow, bulletColumn, LOW);
        enemyRow[index] = -200;
        enemyColumn[index] = -200;
        isBulletActive = false;
        bulletRow = -1;
        bulletColumn = -1;
        lc.setLed(0, enemyRow[index], enemyColumn[index], LOW);
       }else{
        lc.setLed(0, bulletRow, bulletColumn, HIGH);
       }
     }
   previousBulletMillis = currentMillis;
 }
  }

  

 if(screenState == "ss"){
  for(int index = 0; index < (sizeof(enemyRow)/sizeof(int));index++){
    lc.setLed(0, enemyRow[index], enemyColumn[index], HIGH);
  }
  if (currentMillis - previousEnemyMillis >= enemySpeed){
    for(int index = 0; index < (sizeof(enemyRow)/sizeof(int));index++){
      if(enemyRow[index] != -1 && enemyColumn[index] != -1){
        lc.setLed(0, enemyRow[index], enemyColumn[index], LOW);
        enemyColumn[index] += enemyVelocity;
        lc.setLed(0, enemyRow[index], enemyColumn[index], HIGH);
      }
    }

    if(enemyColumn[(sizeof(enemyColumn)/sizeof(int)) - 1] >= 8){
      enemyVelocity = -1;
      for(int index = 0; index < (sizeof(enemyRow)/sizeof(int));index++){
        enemyRow[index] += 1;
        lc.setLed(0, enemyRow[index], enemyColumn[index], LOW);
        enemyColumn[index] += enemyVelocity;
        lc.setLed(0, enemyRow[index], enemyColumn[index], HIGH);
      }
    }

    if(enemyColumn[0] <= -1){
      enemyVelocity = 1;
      for(int index = 0; index < (sizeof(enemyRow)/sizeof(int));index++){
        enemyRow[index] += 1;
        lc.setLed(0, enemyRow[index], enemyColumn[index], LOW);
        enemyColumn[index] += enemyVelocity;
        lc.setLed(0, enemyRow[index], enemyColumn[index], HIGH);
      }
    }

    previousEnemyMillis = currentMillis;
  }
 }

// BLUETOOTH PORTION
  if(recieved == 'f'){
    recieved = 0;
    currentOption = 2;
    updateDisplay("ok");
  }
  if(recieved == 't'){
    currentOption = 0;
    recieved = 0;
    updateDisplay("ok");
  }
  if(recieved == 'l'){
    recieved = 0;
    updateDisplay("left");
  }
  if(recieved == 'r'){
    recieved = 0;
    updateDisplay("right");
  }
  if(recieved == 'o'){
    recieved = 0;
    updateDisplay("ok");
  }
  if(recieved == 'h'){
    recieved = 0;
    screenState = "home";
    currentOption = 0;
    updateDisplay("none");
  }
  if(recievedString != ""){
//    delay(250);
//    Serial.println(recievedString);
//    recievedString = "";
    Serial.println(recievedString.indexOf(" HOUR"));
  }
  
}

void printByte(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}
