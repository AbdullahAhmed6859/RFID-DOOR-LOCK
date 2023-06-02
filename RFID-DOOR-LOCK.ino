#include <SPI.h>
#include <RFID.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define SS_PIN 10
#define RST_PIN 9


RFID rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

String rfidCard;

const int magnet = 4;
const int buzzer = 5;
const int red_led = 6;
const int green_led = 7;

bool found = false;

const int len = 8;
const String ark_members[len] = {"    Armaghan    ", "      SAAD      ", "      MAAZ      ", "     Askari     ", "     Adeem      ", "    Abdullah    ", "  Abdullah Asif "};
const String ark_members_id[len] = {"12571133117", "39673921", "85321382", "1701218626", "23814717520", "18519710964", "136487149"};

void setup() {
  Serial.begin(9600);
  Serial.println("Starting the RFID Reader...");
  SPI.begin();
  rfid.init();

  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(magnet, OUTPUT);

  digitalWrite(red_led, LOW);
  digitalWrite(green_led, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(magnet, LOW);
  lcd.begin(16, 2);
  lcd.backlight();

}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("Please Scan Card");

  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      rfidCard = String(rfid.serNum[0]) + String(rfid.serNum[1]) + String(rfid.serNum[2]) + String(rfid.serNum[3]);
      Serial.println(rfidCard);
      for (int n = 0; n < len; n++) {
        if (rfidCard == ark_members_id[n]) {
          lcd.clear();
          Serial.println("Welcome" + ark_members[n]);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Welcome To ARK  ");
          lcd.setCursor(0, 1);
          lcd.print(ark_members[n]);

          digitalWrite(magnet, HIGH);
          digitalWrite(red_led, HIGH);
          digitalWrite(green_led, LOW);
          digitalWrite(buzzer, HIGH);
          delay(2000);

          found = true;
          lcd.clear();
        }
      }

      if (found == false) {
        lcd.clear();
        Serial.println("Card not found");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Denied");

        digitalWrite(red_led, LOW);
        digitalWrite(green_led, HIGH);

        for (int i = 0; i < 10; i++) {
          digitalWrite(buzzer, HIGH);
          delay(200);
          digitalWrite(buzzer, LOW);
          delay(200);
        }
        lcd.clear();
      }
    }
    rfid.halt();
  }

  digitalWrite(red_led, LOW);
  digitalWrite(green_led, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(magnet, LOW);
  found = false;
}
