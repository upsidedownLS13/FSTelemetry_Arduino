// Example file for Farming Simulator Telemetry project.
// Install ArduinoJson by Benoit Blanchon (can be done via Arduino IDE) min version 7.0
//
// This example reads left indicator, right indicator and motor rpm (current, min, max) for the current vehicle. 
// Right indicator is send to the inbuilt LED to keep this example as simple as possible.
// 
// Use this example to build your own hardware.
//
// Have fun building, driving and farming! :)
// upsidedown, April 2024

#include <ArduinoJson.h>

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
}

JsonDocument doc;

bool indL = false;
bool indR = false;
float RPM = 0.0f;
float RPMMin = 0.0f;
float RPMMax = 0.0f;



void loop() {
  deserializeJson(doc, Serial);
  
  if (doc.overflowed()) {
      sendRequests();
      return;
  }
  
  if(doc.size()>0){

    if(doc.containsKey("IsLightTurnRightOn")){      
      indR = doc["IsLightTurnRightOn"].as<bool>();
    }else{
      sendRequests();
    }
    if(doc.containsKey("IsLightTurnLeftOn")){      
      indL = doc["IsLightTurnLeftOn"].as<bool>();
    }else{
      sendRequests();
    }




  }else{
    sendRequests();
    return;
  }


  digitalWrite(LED_BUILTIN,indR);
}

// this code is used to minimize memory usage due to huge JSON strings being sent to the controller.
// this way controllers like Nanos, Unos etc can be used:
// Adjust this function according to your needs (add/remove req: items)
void sendRequests(){
  Serial.println(F("clr")); //tell the Telemetry server to forget the wishlist for this specific client
  Serial.println(F("req:IsLightTurnLeftOn")); //adds a single item "IsLightTurnLeftOn" to the wishlist for this specific client.
  Serial.println(F("req:IsLightTurnRightOn"));

  Serial.println(F("req:RPM"));  
  Serial.println(F("req:RPMMin"));  
  Serial.println(F("req:RPMMax"));  
}