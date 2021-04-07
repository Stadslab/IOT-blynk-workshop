#include <Blynk.h>
#include <Arduino.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "BLYNK AUTH CODE";
char ssid[] = "WIFI SSID";
char pass[] = "WIFI PASSWORD";
 
 
//define DHT
#include <DHT.h>
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
 
 
 
//define Push notificatie debug led en button
#define buttonPin D3
#define dbLed A0
 
 
 
//define led
BLYNK_WRITE(V7) 
{
 int pin = param.asInt();
 if (pin == 1){
 pin = 1024;
 }
 analogWrite(D7, pin);
}
 
 
 
//ultrasoon lcd
const int ping = D2;
const int echo = D1;
long tijd;
int afstand;
long teller = 0;
int telnummer = 25;
#define BLYNK_PRINT Serial
WidgetLCD lcd(V1);
WidgetLCD vlcd(V2);
 
 
 
//algemeen
void setup()
{
 Serial.begin(9600);
 Blynk.begin(auth, ssid, pass);
 pinMode(ping , OUTPUT);
 pinMode(echo, INPUT);
 pinMode(buttonPin, INPUT_PULLUP);
 pinMode(dbLed, OUTPUT);
 vlcd.clear();
 dht.begin();
}
 
 
//void push notificatie 
void pushy(){
 if(digitalRead(buttonPin) == LOW){
 analogWrite(dbLed, 255);
 Serial.write("Knop is ingedrukt"); //Debug in serial monitor
 Blynk.notify("Hey! This message travelled through 14 different network layers to reach your phone. :)"); //Dit komt in de push notificatie
 }
 else{
 analogWrite(dbLed, 0);
 }
}
 
//void ultrasoon lcd print
void printtje(){
 digitalWrite(ping, LOW);
 delayMicroseconds(2);
 digitalWrite(ping, HIGH);
 delayMicroseconds(2);
 digitalWrite(ping, LOW);
 tijd = pulseIn(echo, HIGH);
 afstand = tijd*0.034/2;

 vlcd.print(0,1,"     ");
 teller=0;
 if (afstand > 50){
 vlcd.print(0,1,"50+");
 }
 else{
 vlcd.print(0,0,"Afstand in cm");
 vlcd.print(0,1,afstand);
 }
}
 
 
 
//void DHT sensor
void sendSensor()
{
 float h = dht.readHumidity();
 float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
 Blynk.virtualWrite(V5, t); 
 Blynk.virtualWrite(V6, h); 
}
 
 
 
//algemeen loop
void loop(){
 Blynk.run();
 teller = teller + 1;
 if (teller>telnummer) {
 printtje();
 sendSensor();
 }
 pushy();
 }
