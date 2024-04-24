//// ************************************************************************************
//// *                wifi connection process
//// ************************************************************************************
//void connectionLoop() {
//  Serial.println("entering the connection loop");
// // this is an infinitive loop that imes out after 5 minuts 
// // we stay in this loop until:
// //  - it times out. When that happens we reboot. This takes care for automatic 
// //    reconnection after grid or router failures
// //  - handleBT = true, also reboot
//  previousMillis = millis(); // for the time-out
//  while (millis() < previousMillis + 300*1000UL) { // 5 minutes = 300
//
//    if(SerialBT.available()) {
//        if(handleBT()) break; // handleBT is true on user confirm exit
//    }
//  } 
//  // if we are here we are connected or the loop timed out
//  // in both cases we reboot
//  Serial.println("leaving the connectionLoop");
//  SerialBT.println("\nrebooting");
//  delay(200);
//  ESP.restart();
//}
//
//bool handleBT() {
//      char sid[33] = ""; 
//      char pas[64] = "";       
//      int InByteCounter = 0;
//      byte SerialInByte;
//      char InputBuffer_BT[50];
//      InputBuffer_BT[0] = '\0'; // make it like empty       
//      delay(200); // wait untill maybe more data available
//      Serial.println("serialBT data available: " + String(SerialBT.available()) );
//  
//      while(SerialBT.available()) {
//            SerialInByte = SerialBT.read(); 
//            
//            if(isprint(SerialInByte)) {
//              if(InByteCounter < 50) InputBuffer_BT[InByteCounter++] = SerialInByte;
//            }    
//            if(SerialInByte=='\n') {  // new line character                                            
//              InputBuffer_BT[InByteCounter]=0;
//              break; // serieel data is complete
//            }
//       }  
//     Serial.println("InputBuffer_BT =" + String(InputBuffer_BT) );
//     Serial.println("length InputBuffer_BT = " + String(strlen(InputBuffer_BT)) );
//     diagNose = 1; // direct the output to serial
//     // evaluate the incomming data
//
//     if (strncasecmp(InputBuffer_BT,"INFO",4) == 0 && btAuth) {
//         scroll(4);
//         SerialBT.println("*** AVAILABLE COMMANDS ***");
//         SerialBT.println("CONNECT (connect to the wifi)");
//         SerialBT.println("ADMINPW (set admin password)");
//         SerialBT.println("SECURITY (set security level)");
//         SerialBT.println("WIPEWIFI (remove wifi settings)");
//         SerialBT.println("EXIT  (reboot the device)");
//         SerialBT.println("\n*** SOME SYSTEM INFO ***");
//         SerialBT.print("IP address: "); SerialBT.println(WiFi.localIP());
//         SerialBT.println("admin password : " + String(pswd));
//         SerialBT.println("securityLevel : " + String(securityLevel));
//         return false;
//     } else 
//
//     // **************   AUTHENTICATION   *****************************
//     if (strncasecmp(InputBuffer_BT,"AUTH",4) == 0) {
//        char tpwd[60];
//        char chipid[10];
//        getChipId(true).toCharArray(chipid, getChipId(true).length()+1);
//        Serial.println( "\nchipid = " + String(chipid) );
//        SerialBT.println(F("\nplease enter your admin password"));
//        String temp = readSerial();
//        temp.toCharArray(tpwd, temp.length()+1);
//        if(strcmp(pswd, tpwd) == 0 || strcmp(chipid, tpwd) == 0) {
//        SerialBT.println(F("\npasswd oke"));
//        btAuth = true; 
//        } else {
//        SerialBT.println(F("\nwrong passwd")); 
//        }
//        
//        return false;
//     } else
//     
//     
//     // **************   EXIT   *****************************
//     if (strncasecmp(InputBuffer_BT,"EXIT",4) == 0 && btAuth) {
//        SerialBT.println(F("\nare you sure to exit Y/N ?"));
//        if( confirm_bt() ) return true;
//        return false;
//     } else
//
//    // **************  remove wifi settings   *****************************
//    if (strncasecmp(InputBuffer_BT,"WIPEWIFI", 8) == 0 && btAuth) {
//        SerialBT.println(F("\nare you sure to wipe the wifi Y/N ?)"));
//        if( confirm_bt() ) { 
//            flush_wifi();
//            SerialBT.println(F("\nwiped wifi settings)"));
//         }
//        return false;
//    } else         
//
//    // **************  connect to wifi   *****************************          
//    if (strncasecmp(InputBuffer_BT,"CONNECT",7) == 0 && btAuth) {
//      WiFi.mode(WIFI_OFF);
//      SerialBT.println(F("\nplease enter your wifi name (ssid)"));
//      String temp = readSerial();
//      temp.toCharArray(sid, temp.length()+1);
//      SerialBT.println(F("\nplease enter your wifi password"));
//      temp = readSerial();
//      Serial.println("readstring");
//      temp.toCharArray(pas, temp.length()+1);
//
//      SerialBT.println(F("\nconnect with these credentials Y/N ?"));
//      if( confirm_bt() ) {
//           WiFi.mode(WIFI_STA);
//           SerialBT.println("trying to connect ");
//           WiFi.begin(sid, pas);// Start Wifi with new values.
//           if (connect2Wifi() == WL_CONNECTED) {
//               SerialBT.println(F("\nyoupyyyyy, connected!"));
//               digitalWrite(led_onb, LED_UIT);
//               ledblink(3,500);
//               SerialBT.println(F("do you want to exit Y/N ?"));
//               if( confirm_bt() ) return true;
//               return false;
//            } else {
//               SerialBT.println(F("could not connect, try again"));
//               digitalWrite(led_onb, LED_AAN); // 
//               return false;
//            }
//        } 
//        return false;
//    } else
//
//     // **************   security level  *****************************
//     //char pswd[11] = "0000"; globally defined
//    if (strncasecmp(InputBuffer_BT,"SECURITY",8) == 0 && btAuth) {
//       Serial.println("found SECURITY");
//       SerialBT.println("\nplease enter a value 0-9");
//       while(SerialBT.available() == 0) { } // wait for input
//       securityLevel = SerialBT.parseInt();
//       if(securityLevel < 0 || securityLevel > 9) {
//         SerialBT.println("\n invalid value 0-9");
//       } else {
//         if( confirm_bt() ) {
//         SerialBT.println("\nsaving securityLevel");
//         wifiConfigsave();
//         }     
//       }
//       return false;
//    } else
//
//     // **************   admin password   *****************************
//     if (strncasecmp(InputBuffer_BT,"ADMINPW",7) == 0 && btAuth) {
//         Serial.println("found ADMINPW");
//         SerialBT.println(F("\please enter your admin password"));
//         String temp = readSerial();
//         // save the passwd
//         SerialBT.printf("\npasswd = \"%s\", save it Y/N ?" , String(pswd));
//         if( confirm_bt() ) {
//            temp.toCharArray(pswd, temp.length()+1);
//            SerialBT.println("\nsaving the password");
//            wifiConfigsave(); //  save it in SPIFFS
//         } 
//         return false;
//      }
//           
//      // if we are here, there was input but not recognized
//      // or we are not authorized
//      if(!btAuth) {
//        SerialBT.println(F("\n ** you have no permission! **"));
//      } else {
//      SerialBT.println(F(" INVALID COMMAND , TYPE INFO" ));
//      } // the buffercontent is not making sense so we empty the buffer
//      while(SerialBT.available()) { SerialBT.read(); }     
//      return false; // return to the connectionLoop
//}
//
//String readSerial() {
//    while(SerialBT.available() == 0) { } 
//    String rec = SerialBT.readString();
//    SerialBT.println("\nyou entered " + rec);
//    Serial.println("\nentered " + rec + " len=" + String(rec.length()) );
//    return rec;
//}
//
//bool confirm_bt() {
////     if( !btAuth ) {
////     SerialBT.println(F("\n ** you have no permission! **"));
////     return false; 
////     }
//     while(SerialBT.available() == 0) { } // wait for input
//          int tmp = SerialBT.read();
//          Serial.println("serialBT.read = " + String(tmp));
//          if(tmp == 'Y' || tmp == 'y') {
//              return true;
//          } else {
//              SerialBT.println(F("\nabort"));
//              return false;
//          }
//}
//
//int connect2Wifi() {  
//    Serial.println("try connect with the new credentials");
//    
//    int connRes = WiFi.waitForConnectResult();
//    SerialBT.println("Connection result is : " + String(connRes));
//    SerialBT.println(WiFi.localIP());
//    checkFixed(); // set static ip if configured
//  
//    return connRes;
//}
//
//void flush_wifi() {
//     if( diagNose != 0 ) consoleOut("erasing the wifi credentials");
//     WiFi.disconnect();
//     if( diagNose != 0 ) consoleOut(F("wifi disconnected"));
//     //now we try to overwrite the wifi creentials     
//     const char* wfn = "dummy";
//     const char* pw = "dummy";
//     WiFi.begin(wfn, pw);
//     Serial.println(F("\nConnecting to dummy network"));
//     int teller = 0;
//     while(WiFi.status() != WL_CONNECTED){
//        Serial.print(F("wipe wifi credentials\n"));
//        delay(100);         
//        teller ++;
//        if (teller > 2) break;
//    }
//    // if we are here, the wifi should be wiped 
//}
