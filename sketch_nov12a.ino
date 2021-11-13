#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <Servo_ESP32.h>

#include "DHT.h"
#define DHTTYPE DHT11
#define DHT_PIN 0
DHT dht(DHT_PIN, DHTTYPE);

//data
#define WIFI_SSID "TORAYAKAN!!!!!!"
#define WIFI_PASSWORD "Torayakan123"
#define DATABASE_URL "https://mpiot-5e0e2-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define DATABASE_SECRET "qf2vr1u6HwBUUYv8YVzj5taSPkEpEBFShC1kfMiN"


//servo
static const int servoPin = 14;
Servo_ESP32 servo1;

int angle =0;
int angleStep = 1;

int angleMin =0;
int angleMax = 180;

void servo(){
   for(int angle = 0; angle <= angleMax; angle +=angleStep) {
        servo1.write(angle);
        delay(20);
    }

    for(int angle = 180; angle >= angleMin; angle -=angleStep) {
        servo1.write(angle);
        delay(20);
    }
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(DATABASE_URL,DATABASE_SECRET);

  servo1.attach(servoPin);

}


void loop() {
  // put your main code here, to run repeatedly:
  int value = Firebase.getInt("Path/dht");
  Serial.println(value);
  if(value == 0){
    Firebase.setFloat("Path/Kelembaban",0);
    Firebase.setString("Path/Status","Berhenti");
    Serial.println("Turning OFF");
    return;
  }else if(value == 1){
    float lembab = dht.readHumidity();
    if (isnan(lembab)){
      Serial.print("Failed to read from DHT sensor!");
      Serial.println(Firebase.error());
      return;
    }
    Serial.print("Cek kelembaban!");
    Firebase.setFloat("Path/Kelembaban",lembab);
    Firebase.setString("Path/Status","Cek kelembaban!");
    Serial.print("Kelembaban: "+(String)lembab);
    if(lembab >= 70){
      Serial.println(" mejalankan servo !!");
      servo();
    }
  }
  
}
