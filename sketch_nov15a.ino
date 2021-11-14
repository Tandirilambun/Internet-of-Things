#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "TORAYAKAN!!!!!!";
const char* password ="Torayakan123";

String endpoint;
String response;
int levelAC;

int pinled1 = 27;
int pinled2 = 26;
int pinled3 = 25;

DynamicJsonDocument doc(12288);

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 0.5*60*1000;

void setup(void) {
  // put your setup code here, to run once:

  pinMode(pinled1, OUTPUT);
  pinMode(pinled2, OUTPUT);
  pinMode(pinled3, OUTPUT);
  
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network:");
  Serial.println(WiFi.localIP());

}

void loop() {
  HTTPClient http;

  currentTime= millis();

  if(currentTime - previousTime > timeoutTime){
    Serial.println("send request");
    previousTime = currentTime;

    endpoint = "https://api.tomorrow.io/v4/timelines?location=-7.795580,110.369492&fields=temperature&timesteps=1h&units=metric&timezone=Asia/Jakarta&apikey=9hoi9GM07RYVtLciieYBe9HR2zMiNmNM";

    http.begin(endpoint);

    http.GET();

    response = http.getString();
    Serial.println(response);

    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    JsonObject data_timelines_0 = doc["data"]["timelines"][0];
    const char* data_timelines_0_intervals_0_startTime = data_timelines_0["intervals"][0]["startTime"];
    float data_timelines_0_intervals_0_values_temperature = data_timelines_0["intervals"][0]["values"]["temperature"];

    Serial.print("Waktu: ");
    Serial.println(data_timelines_0_intervals_0_startTime);
    Serial.print("Temperature: ");
    Serial.println(data_timelines_0_intervals_0_values_temperature+(String)" Celcius");

    if(data_timelines_0_intervals_0_values_temperature < 17.0){
      Serial.println("Air Conditoner LEVEL: 1");
      levelAC = 1;
    }else if (data_timelines_0_intervals_0_values_temperature > 23.0){
      Serial.println("Air Conditoner LEVEL: 2");
      levelAC = 2;
    }else if (data_timelines_0_intervals_0_values_temperature > 28.0){
      Serial.println("Air Conditoner LEVEL: 3");
      levelAC = 3;
    }

    if(levelAC == 1){
      Serial.println("Set Air Conditoner to LEVEL 1");
      digitalWrite(pinled1, HIGH);
      digitalWrite(pinled2, LOW);
      digitalWrite(pinled3, LOW);
    }else if (levelAC == 2){
      Serial.println("Set Air Conditoner to LEVEL 2");
      digitalWrite(pinled1, HIGH);
      delay(1000);
      digitalWrite(pinled2, HIGH);
      digitalWrite(pinled3, LOW);
    }else if (levelAC == 3){
      Serial.println("Set Air Conditoner to LEVEL 3");
      digitalWrite(pinled1, HIGH);
      digitalWrite(pinled2, HIGH);
      digitalWrite(pinled3, HIGH);
    }
    http.end(); 
    
  }
  

}
