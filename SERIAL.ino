void handle_Serial () {
Serial.println("we are in handle serial");  
int SerialInByteCounter = 0;
char InputBuffer_Serial[100] = "";
byte SerialInByte;  
// first check if there is enough data, at least 13 bytes
delay(200); //we wait a little for more data as the esp seems slow
if(Serial.available() < 13 ) {
  // les then 13, we can't expect more so we give up 
  while(Serial.available()) { Serial.read(); } // make the buffer empty 
  Serial.println("invalid command, abort " + String(InputBuffer_Serial));
 return;
}  

//if(Serial.available() < 4 ) { 
//  delay(200); //we wait a little for more data as the esp seems slow
//  if(Serial.available() < 13 ) {
//    while(Serial.available()) { Serial.read(); } // make the buffer empty
//    Serial.println("invalid command, abort " + String(InputBuffer_Serial));
//    return;
//  }
//}

// now we know there are at least 13 bytes so we read them
 while(Serial.available()) {
             SerialInByte=Serial.read(); 
             //Serial.print("+");
            
            if(isprint(SerialInByte)) {
              if(SerialInByteCounter<100) InputBuffer_Serial[SerialInByteCounter++]=SerialInByte;
            }    
            if(SerialInByte=='\n') {                                              // new line character
              InputBuffer_Serial[SerialInByteCounter]=0;
              //   Serial.println(F("found new line"));
             break; // serieel data is complete
            }
       }   
   Serial.println("InputBuffer_Serial = " + String(InputBuffer_Serial) );
   // evaluate the incomming data

        // -------------------------------------------------------
        // Handle Device Management Commands
        // -------------------------------------------------------
        
        if (strcasecmp(InputBuffer_Serial, "PRINTOUT-SPIFFS") == 0) {
          File root = SPIFFS.open("/");
          File file = root.openNextFile();
          int total = 0;
          while(file){
             total += file.size();
             Serial.println("File: " + String(file.name()) + " size: " + String(file.size()) ); 
             file = root.openNextFile();
          }
          Serial.println("total SPIFFS = " + String(total));
          return;
        
        } else

        if (strcasecmp(InputBuffer_Serial,"SENDSOCKET-EVENT")==0) {
          Serial.println("\nsending an event ! \n");
           eventSend(0);
           delay(500);
           eventSend(1);
           return;
        } else

        if (strcasecmp(InputBuffer_Serial,"VARIABLES-DUMP")==0) {
          Serial.println("\ndump variables ! \n");
          Serial.println("duty = " + String(duty));
          Serial.println("Mqtt_Format = " + String(Mqtt_Format));
          Serial.println("dvName = " + String(dvName));
          Serial.println("dvName = " + String(dvName));
           delay(500);
           eventSend(1);
           return;
        } else

        if (strcasecmp(InputBuffer_Serial,"WIFI-WIPECREDENTIALS")==0) {
          Serial.println("\nwipe credentials");
          actionFlag=11;
          return;
        } else        

        if (strcasecmp(InputBuffer_Serial,"DEVICE-REBOOT")==0) {
          Serial.println("\ngoing to reboot ! \n");
           delay(1000);
           ESP.restart();
        } else

           if (strncasecmp(InputBuffer_Serial, "DELETE-FILE", 11 ) == 0) {  
               String bestand = InputBuffer_Serial;
               bestand = bestand.substring(12); // clean 'SETVAR;'
               Serial.println("string bestand cleaned = " + bestand);
               // now should have like /bestand.json or so;
               if (SPIFFS.exists(bestand)) 
               {
                  Serial.println("going to delete file " + bestand); 
                  SPIFFS.remove(bestand);
                  Serial.println("file " + bestand + " removed!"); 
               } else { Serial.println("unkown file"); }
           return;   
           } else 
          
          if (strncasecmp(InputBuffer_Serial, "SETVAR",6)==0) {
       
           String setvar = InputBuffer_Serial;
           setvar = setvar.substring(7); // clean 'SETVAR;'
           Serial.println("string setvar cleaned = " + setvar);
           // now shoud have like variable;123;
           int pos1 = setvar.indexOf(';');
           int pos2 = setvar.lastIndexOf(';');
           String var = setvar.substring(0,pos1);
           Serial.println("string var = " + var);
           String val = setvar.substring(pos1+1,pos2);
           //Serial.println("string val = " + val);                     
           int var_len = var.length();
           Serial.println("var length = " + String(var_len) );
           int value=val.toInt();
           Serial.println("value = " + String(value));
           delay(100); // to get serial.print by
           if(var == "diagnose") {
             delay(100);
             diagNose = value;
             Serial.println("diagNose set to " + String(diagNose) );
             write_eeprom();
             return;
           } 
//           if(var == "touch_th") {
//             delay(100);
//             touch_th = value;
//             Serial.println("touch_th set to " + String(touch_th) );
//             return;
//             } else
//           if(var == "ledon_time") {
//             delay(100);
//             ledon_time = value;
//             Serial.println("ledon_time set to " + String(ledon_time) );
//             return;
//           } else
//           if(var == "rotate") {
//             delay(100);
//             if(val == "true") rotate=true; else rotate = false; 
//             Serial.println("rotate set to " + String(rotate) );
//             return;
//           }                       
//           Serial.println("no such variable !");
//           return;
                      
        } else  
        if (strncasecmp(InputBuffer_Serial, "WIFI",4)==0) {
       
           String credent = InputBuffer_Serial;
           credent = credent.substring(5); // WIFI;
           Serial.println("string credentials cleaned = " + credent);
           int pos1 = credent.indexOf(';');
           int pos2 = credent.lastIndexOf(';');
           String sid = credent.substring(0,pos1);
           Serial.println("string sid = " + sid);
           String pw = credent.substring(pos1+1,pos2);
           Serial.println("string pw = " + pw);                     
           int sid_len = sid.length() + 1;
           char APID[sid_len];
           sid.toCharArray(APID, sid_len);
           int pw_len = pw.length() + 1;
           char PASS[pw_len];
           pw.toCharArray(PASS, pw_len);                     
           WiFi.begin(APID, PASS);
          
           while (WiFi.status() != WL_CONNECTED) {
              delay(500);
              Serial.print("* ");
              WiFi.begin();
              event += 1;
              if(event>10) break;
            }
            event=0;  //reset event
            if(WiFi.status() == WL_CONNECTED) {
            Serial.print("WiFi connected, ip = ");
            Serial.println(WiFi.localIP());
            Serial.println("\ntype DEVICE-REBOOT; to reboot");  
            } else {
            Serial.println("connection failed, try again\n");
            }
            return;
        } else

     // **************   admin password   *****************************
     if (strncasecmp(InputBuffer_Serial,"SET-ADMINPW", 11 ) == 0 ) {
         Serial.println("found SET-ADMINPW");
         String passwd = InputBuffer_Serial;
         passwd = passwd.substring(12); // SET-ADMINPW;
         passwd.toCharArray(pswd, passwd.length()+1);
         Serial.printf("\npasswd = \"%s\", is saved! " , String(pswd));
         
         //Serial.println("\nsaving the password");
         wifiConfigsave(); //  save it in SPIFFS
         return;
      } else
        
        if (strcasecmp(InputBuffer_Serial,"LIST-COMMANDS") == 0) {
            Serial.println("* * * AVAILABLE COMMANDS * * *"); 
            Serial.println("WIFI;SSID;PASSWORD; (connect to wifi)");
            Serial.println("WIFI-WIPECREDENTIALS; (reset wifi and start ap)");            
            Serial.println("SET-ADMINPW;PASSWORD (set admin password)");
            Serial.println("SETVAR;VAR;VALUE (set the value of a vailable)");            
            Serial.println("DEVICE-REBOOT; (restart the device)");
            Serial.println("PRINTOUT-SPIFFS; (show fs)");
            Serial.println("DELETE-FILE;/file.abc (delete a file from fs)");
            return;
        } else {
             sprintf(InputBuffer_Serial, "CMD UNKNOWN;"); // 
             Serial.println( InputBuffer_Serial );
        }
}
