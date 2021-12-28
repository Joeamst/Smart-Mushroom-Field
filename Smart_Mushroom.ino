#include "CTBot.h"
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

#define DHTPIN D3    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

LiquidCrystal_I2C lcd(0x27, 16, 2); // GANTI 0x3F Ke 0x27 kalau LCD ga muncul
DHT dht(DHTPIN, DHTTYPE);
CTBot Bar_Jamur;
TBMessage msg;

int relay = 14;
int relay2 = 12;
String ssid  = "   "    ; // REPLACE mySSID WITH YOUR WIFI SSID
String pass  = "   "; // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
String token = "   "   ; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

void setup() {
  // initialize the Serial
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, HIGH);
  lcd.begin();
  lcd.backlight();
  dht.begin();
  Serial.println("Starting TelegramBot...");
  Bar_Jamur.wifiConnect(ssid, pass);
  Bar_Jamur.setTelegramToken(token);
  if (Bar_Jamur.testConnection()) {

    Serial.println("\ntestConnection OK");
  }
  else
  {
    Serial.println("\ntestConnection NOK");
  }
}

void loop() {
  // a variable to store telegram message data

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  Serial.print("kelembaban: ");
  Serial.print(h);
  Serial.print(" ");
  Serial.print("suhu: ");
  Serial.println(t);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Kelembaban: ");
  lcd.setCursor(11, 0);
  lcd.print(h);

  lcd.setCursor(0, 1);
  lcd.print("Suhu: ");
  lcd.setCursor(5, 1);
  lcd.print(t);
  delay(1000);
  String humid = "Kelembaban: " + String(h) + " % \n";
  String temp = "Suhu: " + String(t) + "°C, ";
  if (Bar_Jamur.getNewMessage(msg)) {

    if (msg.text.equalsIgnoreCase("Lembab")) {
      Bar_Jamur.sendMessage(msg.sender.id, humid);
    }
    if (msg.text.equalsIgnoreCase("Suhu")) {
      Bar_Jamur.sendMessage(msg.sender.id, temp);
    }
    if (msg.text.equalsIgnoreCase("Lamp On")) {
      digitalWrite(relay, LOW);
      Bar_Jamur.sendMessage(msg.sender.id, "Lampu Menyala");
    }
    if (msg.text.equalsIgnoreCase("Lamp Off")) {
      digitalWrite(relay, HIGH);
      Bar_Jamur.sendMessage(msg.sender.id, "Lampu Padam");
    }
    if (msg.text.equalsIgnoreCase("Pump On")) {
      digitalWrite(relay2, LOW);
      Bar_Jamur.sendMessage(msg.sender.id, "Pompa Air Menyala");
    }
    if (msg.text.equalsIgnoreCase("Pump Off")) {
      digitalWrite(relay2, HIGH);
      Bar_Jamur.sendMessage(msg.sender.id, "Pompa Air Mati");
    }
    if (msg.text.equalsIgnoreCase("Otomatis")) {
    Bar_Jamur.sendMessage(msg.sender.id, "Memasuki Mode Otomatis");
      if (t > 29.00 || h < 60.00 ) {
        digitalWrite(relay2, LOW);
        Bar_Jamur.sendMessage(msg.sender.id, "Pompa Air Menyala. Suhu dan Kelembaban Kurang");
      }
      if (t < 25.00 || h > 75.00) {
        digitalWrite(relay2, HIGH);
        Bar_Jamur.sendMessage(msg.sender.id, "Pompa Air Mati. Suhu Stabil");
      }
    } //batas
  }
  delay(10);
}
