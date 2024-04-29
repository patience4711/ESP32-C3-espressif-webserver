// ******************   spiffs lezen  *************************

// als er geen spiffs bestand is dan moet hij eigenlijk altijd een ap openenen
void SPIFFS_read() {
  //DebugPrintln("mounting FS...");
 if (SPIFFS.begin(true)) {
    Serial.println("mounted file system");

       if( file_open_for_read("/wificonfig.json") ) {
                Serial.println("read wificonfig\n");
          } else {
             Serial.println("wificonfig.json not opened\n");
          }
       if( file_open_for_read("/timerconfig.json") ) {
                Serial.println("read timerconfig\n");
          } else {
             Serial.println("timerconfig.json not opened\n");
          }
       
       if( file_open_for_read("/basisconfig.json") ) {     
             Serial.println("read basisconfig\n");
          } else {
          Serial.println("basisconfig.json not opened\n");
          } 
       if( file_open_for_read("/mqttconfig.json") ) {     
             Serial.println("mqttconfig read");
          } else {
             Serial.println("mqttconfig.json not opened");
          }         
  } else {
        Serial.println("failed to mount FS");
   }
 // einde spiffs lezen 5 bestanden
  if(timeRetrieved) tijd_convert; // to set asoTime
}  




// **************************************************************************** 
// *                    save the data in SPIFFS                               *  
// ****************************************************************************
void wifiConfigsave() {
   Serial.println("saving config");

    DynamicJsonDocument doc(1024);
    JsonObject json = doc.to<JsonObject>();   
    json["ip"] = static_ip;
    json["pswd"] = pswd;
    json["longi"] = longi;
    json["lati"] = lati;
    
    json["gmtOffset"] = gmtOffset;
    json["zomerTijd"] = zomerTijd;
    json["securityLevel"] = securityLevel;
    File configFile = SPIFFS.open("/wificonfig.json", "w");
    if (!configFile) {
      Serial.println("open file for writing failed!");
    }
    //DebugPrintln("wificonfig.json written");
    #ifdef DEBUG 
//    json.printTo(Serial);
    serializeJson(json, Serial);
    Serial.println(F("")); 
    #endif
    serializeJson(json, configFile);
    configFile.close();
}

void timerConfigsave() {   
consoleOut("saving config");
    DynamicJsonDocument doc(1024);
    JsonObject json = doc.to<JsonObject>();
    json["weekDag"] = weekDag;
    json["relToSunOn"] = relToSunOn;
    json["relToSunOff"] = relToSunOff;   
    json["switchOn"] = switchOn;   
    json["switchOff"] = switchOff;  
    json["timerActive"] = timerActive; 
 
    File configFile = SPIFFS.open("/timerconfig.json", "w");
    if (!configFile) {
      consoleOut("failed to open timerconfig file for writing");
    }
    consoleOut("timerconfig.json written");
    #ifdef DEBUG 
    serializeJson(json, Serial);
    Serial.println("");     
    #endif
//    json.printTo(configFile);
    serializeJson(json, configFile);
    configFile.close();
}

void basisConfigsave() {
    Serial.println("saving basis config");
    DynamicJsonDocument doc(1024);
    JsonObject json = doc.to<JsonObject>();
    json["userPwd"] = userPwd;
    json["dvName"] = dvName;
    json["duty"] = duty;
    defaultDuty = duty;
    json["aso"] = aso;
    
    File configFile = SPIFFS.open("/basisconfig.json", "w");
    if (!configFile) {
      //DebugPrintln("open file for writing failed");
    }
    Serial.println("inverterconfig.json written");
    #ifdef DEBUG 
    serializeJson(json, Serial);
    Serial.println(F(""));     
    #endif
    serializeJson(json, configFile);
    configFile.close();
}

void mqttConfigsave() {
   //DebugPrintln("saving mqtt config");
    DynamicJsonDocument doc(1024);
    JsonObject json = doc.to<JsonObject>();
// 
//    json["Mqtt_Enabled"] = Mqtt_Enabled;
    json["Mqtt_Broker"] = Mqtt_Broker;
    json["Mqtt_Port"] = Mqtt_Port;    
    json["Mqtt_outTopic"] = Mqtt_outTopic;
    json["Mqtt_Username"] = Mqtt_Username;
    json["Mqtt_Password"] = Mqtt_Password;
    json["Mqtt_switchIDX"] = Mqtt_switchIDX;
    json["Mqtt_Format"] = Mqtt_Format;    
    File configFile = SPIFFS.open("/mqttconfig.json", "w");
    if (!configFile) {
      //DebugPrintln("open file for writing failed");
    }
    Serial.println("mqttconfig.json written");
    #ifdef DEBUG
    serializeJson(json, Serial);
    Serial.println(F("")); 
    #endif
    serializeJson(json, configFile);
    configFile.close();
}


bool file_open_for_read(String bestand) {
      //DebugPrint("we are in file_open_for_read, bestand = "); //DebugPrintln(bestand); 
      if (!SPIFFS.exists(bestand)) return false;

      //file exists, reading and loading
      //DebugPrintln("bestand bestaat");
        File configFile = SPIFFS.open(bestand, "r");
        if (!configFile) return false;

           //DebugPrint("opened config file"); //DebugPrintln(bestand);
           size_t size = configFile.size();
          // Allocate a buffer to store contents of the file.
           std::unique_ptr<char[]> buf(new char[size]);
           configFile.readBytes(buf.get(), size);
           DynamicJsonDocument doc(1024);
           auto error = deserializeJson(doc, buf.get());
           //#ifdef DEBUG 
           serializeJson(doc, Serial); Serial.println(F(""));
           //#endif
             if (error) return false;
              //DebugPrintln("parsed json");
              String jsonStr = ""; // we printen het json object naar een string
            // nu kunnen we eerst controleren of een bepaalde entry bestaat
            // zoniet slaan we die over anders crasht de ESP
                serializeJson(doc, jsonStr);
            if (bestand == "/wificonfig.json") {
                      if(jsonStr.indexOf("ip") > 0){ strcpy(static_ip, doc["ip"]);}
                      if(jsonStr.indexOf("pswd") > 0){ strcpy(pswd, doc["pswd"]);}
                      if(jsonStr.indexOf("longi") > 0){longi = doc["longi"].as<float>();}
                      if(jsonStr.indexOf("lati") > 0){lati = doc["lati"].as<float>();}                      
                      if(jsonStr.indexOf("gmtOffset") > 0){ strcpy(gmtOffset, doc["gmtOffset"]);}
                      if(jsonStr.indexOf("zomerTijd") > 0){zomerTijd = doc["zomerTijd"].as<bool>();}
                      if(jsonStr.indexOf("securityLevel") > 0){securityLevel = doc["securityLevel"].as<int>();}
            }
            if ( bestand == "/timerconfig.json" ) {
                      if(jsonStr.indexOf("weekDag") > 0){ strcpy(weekDag, doc["weekDag"]);}
                      if(jsonStr.indexOf("relToSunOn") > 0){ strcpy(relToSunOn, doc["relToSunOn"]);}
                      if(jsonStr.indexOf("relToSunOff") > 0){ strcpy(relToSunOff, doc["relToSunOff"]);}
                      if(jsonStr.indexOf("switchOn") > 0){ strcpy(switchOn, doc["switchOn"]);}        
                      if(jsonStr.indexOf("switchOff") > 0){ strcpy(switchOff, doc["switchOff"]);}
                      if(jsonStr.indexOf("timerActive") > 0){ strcpy(timerActive, doc["timerActive"]);}
            }

            if (bestand == "/basisconfig.json") {
                    if(jsonStr.indexOf("dvName") > 0)  {strcpy(dvName, doc["dvName"]);}
                    if(jsonStr.indexOf("userPwd") > 0) { strcpy (userPwd, doc["userPwd"] );}
                    if(jsonStr.indexOf("duty") > 0)    {duty = doc["duty"].as<int>();}
                    if(jsonStr.indexOf("aso") > 0){ strcpy(aso, doc["aso"]);}
//Serial.println("spiffs dvName read : " + String(dvName));
              }            

            if (bestand == "/mqttconfig.json"){
                     if(jsonStr.indexOf("Mqtt_Broker")   >  0) { strcpy(Mqtt_Broker,   doc["Mqtt_Broker"]);}
                     if(jsonStr.indexOf("Mqtt_Port")     >  0) { strcpy(Mqtt_Port,     doc["Mqtt_Port"]);}  
                     if(jsonStr.indexOf("Mqtt_outTopic") >  0) { strcpy(Mqtt_outTopic, doc["Mqtt_outTopic"]);}         
                     if(jsonStr.indexOf("Mqtt_Username") >  0) { strcpy(Mqtt_Username, doc["Mqtt_Username"]);}
                     if(jsonStr.indexOf("Mqtt_Password") >  0) { strcpy(Mqtt_Password, doc["Mqtt_Password"]);}
                     if(jsonStr.indexOf("Mqtt_Format")   >  0) { Mqtt_Format =         doc["Mqtt_Format"].as<int>();}
                     if(jsonStr.indexOf("Mqtt_switchIDX")   >  0) { Mqtt_switchIDX =   doc["Mqtt_switchIDX"].as<int>();}
            }
             return true;
} 
