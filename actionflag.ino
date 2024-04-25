// all actions called by the webinterface should run outside the async webserver environment
// otherwise crashes will occure.
    void test_actionFlag() {
  // ******************  reset the nework data and reboot in AP *************************
    if (actionFlag == 11 ) { // 
     //consoleOutln("erasing the wifi credentials, value = " + String(value) + "  actionFlag = " + String(actionFlag));
     delay(1000); //reserve time to release the button
     //eraseWifiFlash();
     WiFi.disconnect();
//    WiFi.end();
     // we write a flag in EEPROM
     if( diagNose != 0 ) consoleOut(F("wifi disconnected"));
    //we try to overwrite the wifi creentials     
    const char* ssid = "dummy";
    const char* password = "dummy";
    WiFi.begin(ssid, password);
    Serial.println(F("\nConnecting to dummy network"));
    int teller = 0;
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(F("wipe wifi credentials\n"));
        delay(100);         
        teller ++;
        if (teller > 2) break;
    }
     ESP.restart();
  }  

    if (actionFlag == 10) { // the button was pressed a long time, start ap
     delay(2000); // give the server the time to send the confirm
     consoleOut("rebooting");
     write_eeprom();
     ESP.restart();
  }

    if (actionFlag == 12) { // the button was pressed a short time, switch
     actionFlag = 0; 
     if( ledState == 0) {
      consoleOut("switch on");
      ledsOnNow(true,false,"button");
     } else {
      consoleOut("switch off");
      ledsOffNow(true, true, "button");
     }
  }

// interrrupt button test
if (actionFlag == 15) {
  actionFlag = 0;
  buttonPressed();
}


    // mosquitto reconnect **********************************************    
    if (actionFlag == 24) { // set by the mqtt config page
        actionFlag = 0; //reset the actionflag
        MQTT_Client.disconnect();
       //reset the mqttserver
        MQTT_Client.setServer(Mqtt_Broker, atoi(Mqtt_Port) );
        MQTT_Client.setCallback ( MQTT_Receive_Callback ) ;
        if (Mqtt_Format != 0) mqttConnect(); // reconnect mqtt after change of settings
    }    
    if (actionFlag == 25) {
      actionFlag = 0; //reset the actionflag
      getTijd(); // recalculate time and automation after change of settings
    }

    if (actionFlag == 31) { //triggered by the webpage zbtest
        actionFlag = 0; //reset the actionflag
        ledsOnNow(true,false,"web");
    }    
    if (actionFlag == 32) { //triggered by the webpage zbtest
        actionFlag = 0; //reset the actionflag
        ledsOffNow(true,false,"web");
    }
    
    if (actionFlag == 46) { //triggered by the webpage zbtest
        actionFlag = 0; //reset the actionflag
        showDir(); 
    }


    if (actionFlag == 49) { // triggered by link
        actionFlag = 0; //reset the actionflag
        ledblink(1,100);
        // always first drop the existing connection
        MQTT_Client.disconnect();
        //consoleOut("dropped connection");
        delay(100);
        char Mqtt_send[26] = {0};
       
        if(mqttConnect() ) {
        String toMQTT=""; // if we are connected we do this
        
        strcpy( Mqtt_send , Mqtt_outTopic);
        
        
        toMQTT = "{\"test\":\"" + String(Mqtt_send) + "\"}";
        
             
        if( MQTT_Client.publish (Mqtt_outTopic, toMQTT.c_str() ) ) {
            consoleOut("sent mqtt message : " + toMQTT);
        } else {
            consoleOut("sending mqtt message failed : " + toMQTT);    
        }
      } 
     // the not connected message is displayed by mqttConnect()
    }

} // end test actionflag
