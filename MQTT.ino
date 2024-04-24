bool mqttConnect() {   // 
/* this function checks if we are connected to the broker, if not connect anyway */  
    if( MQTT_Client.connected() ) {
    Serial.println("mqtt was connected");
    return true;
    }
    // we are here because w'r not connected. Signal with the LED
    //ledblink(2,70);

    if (Mqtt_Port[0] == '\0' ) strcpy(Mqtt_Port, "1883");   // just in case ....
    uint8_t retry = 3;
    
    //char Mqtt_inTopic[11]={"ESP-ECU/in"};

    while (!MQTT_Client.connected()) {

      if ( MQTT_Client.connect( Mqtt_Clientid, Mqtt_Username, Mqtt_Password) )
      {
         //connected, so subscribe to inTopic (not for thingspeak)
        if( Mqtt_Format != 0 ) {
        if(  MQTT_Client.subscribe ( "ESP32-SWITCH/in" ) ) {
               Serial.println("subscribed to ESP32-SWITCH/in");
           }
        }
         Serial.println(F("mqtt connected"));
         //Update_Log(3, "connected");
      
       return true;

    } else {
        //String term = "connection failed state: " + String(MQTT_Client.state());
        //Update_Log(3, "failed");
        if (!--retry) break; // stop when tried 3 times
        delay(500);
    }
  }
  // if we are here , no connection was made.

  if(diagNose) consoleOut(F("mqtt connection failed"));
  return false;
}

// *************************************************************************
//                   process received mqtt
// *************************************************************************

void MQTT_Receive_Callback(char *topic, byte *payload, unsigned int length)
{
   
    StaticJsonDocument<1024> doc;       // We use json library to parse the payload                         
   //  The function deserializeJson() parses a JSON input and puts the result in a JsonDocument.
   // DeserializationError error = deserializeJson(doc, Payload); // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, payload); // Deserialize the JSON document
    if (error)            // Test if parsing succeeds.
    {
       Serial.println("mqtt no valid json ");
        return;
    } 
    // We check the kind of command format received with MQTT
    //now we have a payload like {"switch",on}    

    if( doc.containsKey("switch") )
    {
        String onoff = doc["switch"]; 
        Serial.println( "got message: {\"switch\":\"" + onoff + "\"}" );
        if(onoff == "on") ledsOnNow(true,false,"mqtt"); 
        if(onoff == "off") ledsOffNow(true,true,"mqtt");
    }
}

bool mqttGeldig() {
    if (Mqtt_Broker[0] == '\0' || Mqtt_Format == 0) {
    return false;
    }
    if (Mqtt_switchIDX == 0) { // geen geldige idx
    return false;   
    } else {
    return true; 
    }
}

void sendMqttswitch() 
{
  if ( !mqttGeldig() ){ 
    consoleOut("no valid mqtt address or not configured, skipping..");
    return;
  }    
// update switch state
  StaticJsonDocument<256> doc;
  //doc["command"] = "switchlight";
  doc["idx"] = Mqtt_switchIDX;
  if (ledState == 0) { doc["nvalue"] = 0; } else { doc["nvalue"] = 1;}
  char out[64];
  int b =serializeJson(doc, out);
  MQTT_Client.publish ( Mqtt_outTopic, out );
}
