// Example file for Farming Simulator Telemetry project.
// Install ArduinoJson by Benoit Blanchon (can be done via Arduino IDE) min version 7.0
// You need to install the respective boards in the Arduino IDE as well.
//
// In this example communication between the telemetry server application and a microcontroller via Wifi is demonstrated.
// Obviously you need a Wifi capable controller for this. ESP32 and ESP8266 settings are provided below, but adaptions to other Wifi capable controllers should not be too complicated.
//
// This example reads left indicator for the current vehicle. Have a look at the Serial example how to read multiple variables.
// Left indicator is send to the inbuilt LED to keep this example as simple as possible.
// 
// Use this example to build your own hardware.
//
// Have fun building, driving and farming! :)
// upsidedown, April 2024

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
#elif defined(ESP32)
   #include <WiFi.h>
   #include <HTTPClient.h>
#endif
#include <WiFiClient.h>
#include <ArduinoJson.h>

// CHANGE these to match YOUR Wifi network:
const char* ssid = "SSID_of_YOUR_Wifi";
const char* password = "12345678901234567890";


// CHANGE this to the (fixed!) IP of the computer running farming simulator:
// keep the port :25556 part!
//
// yes, this means you need to have fixed IPs in your home network. Alternatively setup a separate Wifi or use the Serial option
const char* serverName = "http://192.168.188.21:25556/";




JsonDocument doc;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(LED_BUILTIN,OUTPUT);

}


void loop() {

  WiFiClient client;
  HTTPClient http;

  http.useHTTP10(true);
  http.begin(client, serverName);
  http.GET();  
  deserializeJson(doc, http.getStream()); // Parse response  
  http.end(); // Disconnect

  if (doc.overflowed()) {
      sendRequests();
  }
  
  if(doc.size()>0){
    if(doc.containsKey("IsLightTurnLeftOn")){
      digitalWrite(LED_BUILTIN,!doc["IsLightTurnLeftOn"].as<bool>());
    }else{
      sendRequests();
    }
  }else{
    sendRequests();
  }
}

//this function is mostly here to make Wifi and Serial versions of Arduino code as similar as possible, but it is still useful to detect if you have issues:
void sendRequests(){
  Serial.println(F("out of memory!"));
}