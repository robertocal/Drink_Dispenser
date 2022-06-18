#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "";
char pass[] = "";

#define button1_vpin  V0
#define button2_vpin  V1
int IRSensor = 27;
int glass = 0;
int filling = 0;
int statusSensor;
int relay_drink1 = 26;
int relay_drink2 = 25;

const int ROW_NUM    = 4;
const int COLUMN_NUM = 4;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROW_NUM] = {17, 16, 4, 15};
byte pin_column[COLUMN_NUM] = {14, 12, 13, 5};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

int cursorColumn = 0;

BLYNK_CONNECTED() {
  Blynk.syncVirtual(button1_vpin);
  Blynk.syncVirtual(button2_vpin);
}

BLYNK_WRITE(button1_vpin) {
    if(statusSensor == 0 && glass == 1 && filling == 0){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Drink 1 selected");
      filling = 1;
      digitalWrite(relay_drink1, LOW);
      delay(2000);
      digitalWrite(relay_drink1, HIGH);
      filling = 0;
    }
}

BLYNK_WRITE(button2_vpin) {
    if(statusSensor == 0 && glass == 1 && filling == 0){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Drink 2 selected");
      filling = 1;
      digitalWrite(relay_drink2, LOW);
      delay(2000);
      digitalWrite(relay_drink2, HIGH);
      filling = 0;
    }
}

void setup()
{   
  pinMode(IRSensor, INPUT); // sensor pin INPUT
  pinMode(relay_drink1, OUTPUT);
  pinMode(relay_drink2, OUTPUT);
  digitalWrite(relay_drink1, HIGH);
  digitalWrite(relay_drink2, HIGH);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Put the glass in position");
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  Blynk.virtualWrite(button1_vpin, statusSensor);
  Blynk.virtualWrite(button2_vpin, statusSensor);
  delay(1000);
}

void loop(){
  Blynk.run();
  char key = keypad.getKey();
  statusSensor = digitalRead(IRSensor);

  if (statusSensor == 0 && glass == 0 && filling == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Select a drink (press 1 or 2)");
    glass = 1;
  }else if(statusSensor == 1 && glass == 1 && filling == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Put the glass in position");
    glass = 0;
  }

  if (key && statusSensor == 0 && glass == 1 && filling == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(key);

    if(key == '1'){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Drink 1 selected, please wait...");
      filling = 1;
      digitalWrite(relay_drink1, LOW);
      delay(2000);
      digitalWrite(relay_drink1, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Put the glass in position");
      filling = 0;
    }else if(key == '2'){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Drink 2 selected, please wait...");
      filling = 1;
      digitalWrite(relay_drink2, LOW);
      delay(2000);
      digitalWrite(relay_drink2, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Put the glass in position");
      filling = 0;
    }else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Invalid selection! Please select 1 or 2");
    }
  }
}
