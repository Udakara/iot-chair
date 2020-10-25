#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>


#define FIREBASE_HOST "iot-chair.firebaseio.com"                     
#define FIREBASE_AUTH "jOmR0bmr2qJfsSNO1dbYT0YOydnrbpmCQG6zDpYg"

const char* ssid = "Xperia XA Dual";
const char* password = "firefire";

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int button = D0;
const int light = LED_BUILTIN;
const int buzzerPin = D5;
int count = 0;
int max_time = 5;

void setup() {

Serial.begin(9600);
Wire.begin(D2, D1);
lcd.begin();
pinMode(button,INPUT);
pinMode(light,OUTPUT);
pinMode(buzzerPin, OUTPUT);
lcd.home();

WiFi.begin(ssid, password);                                     
Serial.print("Connecting to ");
lcd.print("Connecting to ");
lcd.setCursor(1,1);
lcd.print(ssid);
Serial.print(ssid);

while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
}

lcd.clear();
Serial.println();
Serial.print("Connected to ");
lcd.print("Connected to");
lcd.setCursor(1,1);
lcd.println(WiFi.localIP());
Serial.println(ssid);
Serial.print("IP Address is : ");
Serial.println(WiFi.localIP());
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
delay(1000);
lcd.clear();
}

void loop() {

if (Firebase.failed())
  { delay(500);
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Serial.println(Firebase.error());
    Serial.println("Connection to firebase failed. Reconnecting...");
    delay(500);
  }
   
byte val = digitalRead(button);
  if(val == HIGH){
    Firebase.setString("/status","Sitting");
    Firebase.setString("/timer","Started");
    
    if(count>max_time){
      Firebase.setInt("/expired",1);
      lcd.print("Take a break");
      Serial.println("takr a break");
      tone (buzzerPin,200);
      delay(30); 
      noTone(buzzerPin);
      delay(30);
    }
    count+=1;
    Firebase.setInt("/count",count);
    digitalWrite(LED_BUILTIN,LOW);
  }else{
      Firebase.setString("/status","Not Detected");
      Firebase.setString("/timer","stopped");
      Firebase.setInt("/expired",0);
      digitalWrite(LED_BUILTIN,HIGH);
      count=0;
  }
  lcd.clear();  
}
