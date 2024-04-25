void handle_params(httpd_req_t *req)
{
  // this function handles all parameters comming from forms
  // first read the request into a char
  consoleOut("we are in handle_params");
  char*  buf;
  size_t buf_len;
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
     buf = (char*)malloc(buf_len);
     if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
     consoleOut("handle_params querystring is copied into buf = " + String(buf));  
     //char param[QUERYKEY_MAX_LEN] = {0}; 
     }
     // now we can discriminate what was requested
     if(strstr(buf, "longi")) // if contains longi then we have the geo config form
     {
       // we got the geoconfig form and going to process it
       consoleOut("received the geo form");
       char param[QUERYKEY_MAX_LEN] = {0};
       // the parameters are lati longi tz and checkbox ts
       if (httpd_query_key_value(buf, "longi", param, sizeof(param)) == ESP_OK) longi = atof(param);
       if (httpd_query_key_value(buf, "lati", param, sizeof(param)) == ESP_OK) lati = atof(param);
       if (httpd_query_key_value(buf, "tz", param, sizeof(param)) == ESP_OK) strcpy(gmtOffset, decode_uri(String(param)).c_str());
       if (httpd_query_key_value(buf, "ts", param, sizeof(param)) == ESP_OK) {
          //this is a checkbox so if the param is there it is true
          //consoleOut("found ts so zomerTijd = true");
          zomerTijd = true; 
          } else { 
          //consoleOut("no ts so zomerTijd = fals");
          zomerTijd = false;
       }
      wifiConfigsave();// save the params
      actionFlag=25; // recalculate the time with these settings
      return;
     } // end strstr(longi
  
     if(strstr(buf, "swname"))
     {
       consoleOut("received the basis config form");
       char param[QUERYKEY_MAX_LEN], dec_param[QUERYKEY_MAX_LEN]= {0}; 
       if (httpd_query_key_value(buf, "pw1", param, sizeof(param)) == ESP_OK) strcpy(userPwd, param);
       if (httpd_query_key_value(buf, "swname", param, sizeof(param)) == ESP_OK) strcpy(dvName, param);
       if (httpd_query_key_value(buf, "bev", param, sizeof(param)) == ESP_OK) strcpy(aso, decode_uri(String(param)).c_str());
       basisConfigsave();  // alles opslaan
       actionFlag=25; // recalculates the time for aso new values
       return; 
     } // end if swname
     
     if(strstr(buf, "mqtAdres"))
     {
       consoleOut("received the mqtt config form");
       char param[QUERYKEY_MAX_LEN] = {0};
       if (httpd_query_key_value(buf, "mqtAdres", param, sizeof(param)) == ESP_OK) strcpy(Mqtt_Broker, param);     
       if (httpd_query_key_value(buf, "mqtPort", param, sizeof(param)) == ESP_OK) strcpy(Mqtt_Port, param);    
       if (httpd_query_key_value(buf, "mqtoutTopic", param, sizeof(param)) == ESP_OK) strcpy(Mqtt_outTopic, decode_uri(String(param)).c_str());
       if (httpd_query_key_value(buf, "mqtUser", param, sizeof(param)) == ESP_OK) strcpy(Mqtt_Username, param);
       if (httpd_query_key_value(buf, "mqtPas", param, sizeof(param)) == ESP_OK) strcpy(Mqtt_Password, param);
       if (httpd_query_key_value(buf, "mqtCi", param, sizeof(param)) == ESP_OK) strcpy(Mqtt_Clientid, param);
       if (httpd_query_key_value(buf, "mqidx", param, sizeof(param)) == ESP_OK) Mqtt_switchIDX = atoi(param); // int
       if (httpd_query_key_value(buf, "fm", param, sizeof(param)) == ESP_OK) Mqtt_Format = atoi(param); // int
       mqttConfigsave();  // 
       actionFlag=24; // causes a reconnect with these settings
       return;
     }   

     if(strstr(buf, "zonattaan"))
     {
      consoleOut("received the timer config form, tKeuze = " + String(tKeuze));
      char param[QUERYKEY_MAX_LEN] = {0};
      // so we have the arguments for the timerform in buf, lets extract them
      if (httpd_query_key_value(buf, "ta", param, sizeof(param)) == ESP_OK) timerActive[tKeuze] = '1'; else timerActive[tKeuze] = '0';
    
      // first extract what is selected in zonattaan and zonattuit
      if (httpd_query_key_value(buf, "zonattaan", param, sizeof(param)) == ESP_OK) {
      //String temp = "";
      char temp[1]="";
      strncpy(temp, param,1 );
      //temp = request->arg("zonattaan").c_str(); // arguments are 0 to 6
      //temp.toCharArray(tempChar, 2);
      relToSunOn[tKeuze]=temp[0];  // update variabele relToSunOn 
      consoleOut("relToSunOn = " + String(relToSunOn));
      }
      if (httpd_query_key_value(buf, "zonattuit", param, sizeof(param)) == ESP_OK) {
        char temp[1]="";
        strncpy(temp, param,1 );
        relToSunOff[tKeuze]=temp[0];  // de variabele relToSunOn bijwerken
        consoleOut("relToSunOff = " + String(relToSunOff));      
//        temp = request->arg("zonattuit").c_str(); // arumenten zijn 0 tm 4
//        temp.toCharArray(tempChar, 2);
//        relToSunOff[tKeuze]=tempChar[0];         
//        consoleOut("relToSunOff = "); consoleOutln(String(relToSunOff));                    
      }  
              //now the switch-on time
      if (httpd_query_key_value(buf, "inw", param, sizeof(param)) == ESP_OK) {
        char temp[6]="";
        strncpy(temp, decode_uri(String(param)).c_str(), 6); //param should be decoded
        //strncpy(temp, param,6 );
        //temp = request->arg("inw").c_str();
        //temp.toCharArray(tempChar, 6);
        for (int i=0; i<5; i++){
           switchOn[tKeuze*5+i] = temp[i];
        }
      }
      if (httpd_query_key_value(buf, "uitw", param, sizeof(param)) == ESP_OK) {
        char temp[6]="";
        strncpy(temp, decode_uri(String(param)).c_str(), 6); //param should be decoded
        //strncpy(temp, param,6 );
        //temp = request->arg("inw").c_str();
        //temp.toCharArray(tempChar, 6);
        for (int i=0; i<5; i++){
           switchOff[tKeuze*5+i] = temp[i];
        }
      }
  
      //put the values of the days in an array
      char *grap[] = {"su", "mo", "tu", "we", "th", "fr", "sa"};
     // String wd = ""; 
      // String dag = "";
      //Char ddag[7] = "  
      //for (int i = 0; i<aantal; i++) {    
      char dag[3] = "";
      //query string" ta=on&inw=01%3A00&zonattaan=0&uitw=02%3A00&zonattuit=4&su=on&tu=on&th=on&sa=on
      consoleOut("buf now = " + String(buf));
      if(httpd_query_key_value(buf, "su", param, sizeof(param)) == ESP_OK ) {
      consoleOut("there is an argument su saved in param " + String(dag));
      }
      if(httpd_query_key_value(buf, grap[0], param, sizeof(param)) == ESP_OK ) {
      consoleOut("there is an argument su saved in dag " + String(dag));
}
 
      
      for (int x = 0; x < 7; x++) {
        //grap[x] should be something like "su" mo "mo" etc
        consoleOut("grap[x] = " + String(grap[x])); //can we use that as an argument in:
        if(httpd_query_key_value(buf, grap[x], param, sizeof(param)) == ESP_OK ) {
            consoleOut("at grap[x] param = " + String(param));
            // if the argument exists the selectbox is selected  
            //dag = request->arg(grap[x]) ; //dus zo+1
            //bijv dag=server.arg(zo1)
            // if (String(dag) == "on") {
            // als tKeuze=5 dan begint het bij 35 en eindigt bij 42
            // als tKeuze=0 dan begint het bij 0 en eindigt bij 6  
            weekDag[tKeuze*7+x] = 'y'; 
            } else {
            weekDag[tKeuze*7+x] = 'n';
            }  
          }
       // these funtions work
       // nu zenden we de confirm 
       //consoleOutln("we gaan een nieuwe webpage inlezen in toSend");
       // toSend = FPSTR(CONFIRM);
              
       timerConfigsave(); // save all in SPIFFS
       consoleOut("timer config saved");
       actionFlag = 25; // recalculate the timers
      return;
    }    

     if(strstr(buf, "pwmsave")) // 
      {  
          consoleOut("Found pwmsave parameter ");
          char param[QUERYKEY_MAX_LEN] = {0};
          if (httpd_query_key_value(buf, "ts", param, sizeof(param)) == ESP_OK) duty = atoi(param);
           // String(param).toInt() ?? 
          //consoleOut("saved duty = " + String(duty));
          basisConfigsave();
          return;
          //}
     } // end save pwm

     if(strstr(buf, "ipchoice")) // 
     { 
       consoleOut("received a ip config form");
       char param[QUERYKEY_MAX_LEN] = {0};
       char static_ip2[16] = "";
       IPAddress ipc = WiFi.gatewayIP();
       char option[5] = {0};
       //String optie = request->getParam("keuze")->value();
       if (httpd_query_key_value(buf, "ipchoice", param, sizeof(param)) == ESP_OK) strcpy(option, param); //DHCP or STAT    
       if (httpd_query_key_value(buf, "ipadd", param, sizeof(param)) == ESP_OK) strcpy(static_ip2, decode_uri(String(param)).c_str());    
       //if (httpd_query_key_value(buf, "gwadd", param, sizeof(param)) == ESP_OK) strcpy(Mqtt_outTopic, decode_uri(String(param)).c_str());
       //if (httpd_query_key_value(buf, "snadd", param, sizeof(param)) == ESP_OK) strcpy(Mqtt_Username, decode_uri(String(param)).c_str());
 
       String ipcheck = String(static_ip2[0]) + "." + String(static_ip2[1]) + "." + String(static_ip2[2]);         
      // see if dhcp is selected. if yes, empty static_ip2
      if ( strstr(option, "DHCP" )) {
            consoleOut("dhcp set, dropped static_ip, optie = " + String(option));
            static_ip2[0] = '\0';
           }
          //we must check if ip has changed, this influences the confirm page
          //when dhcp static_ip is zero-ed, so this is always true.
          //how do we do this?
          //We have the variable static_ip (from spiffs), we compare that with the supplied value
          //If not equal, it has changed 
           String test1 = String(static_ip);  
           String test2 = String(static_ip2);
           consoleOut("the teststrings are: " + test1 + " and " + test2);
           consoleOut("read the confirm page in toSend");
           toSend = FPSTR(CONFIRM_IP);
           String adres="";
           String zin="";
           //bool reBoot = false;
           //bool leegmaken = false;
           
           if (String(static_ip) != String(static_ip2) )
           {
              consoleOut("the IP has changed");
              //static_ip=static_ip2;
              strcpy(static_ip, static_ip2);
    
              // if an ip was entered we put the ip data in the confirmpage
              if (static_ip[0] != '\0' && static_ip[0] != '0') 
              {
                 actionFlag = 10; // make it reboot in the loop
                 adres = String(static_ip);
                 if( diagNose != 0 ) consoleOut("the specified ip = " + adres);
                 zin = F("The entered IP is <strong><a href='http://{adres1}'>http://{adres2}</a></strong>");
                 zin += F("<br>Use the new IP adres in your browser or click the link.<br>");
                 zin += F("<br>This page will close after a few seconds...");
                 zin.replace("{adres1}" , adres);
                 zin.replace("{adres2}" , adres);
                 toSend.replace("#zin#" , zin);
                 toSend.replace("{adres0}" , adres); // the address in the settimeout          
              } 
               else 
              {
                // if no ip is specified then we try to connect immediately 
                 zin="IP not specified, this is unknown now !!<br><br>NOTE: the configuration modus (AP) is started<br>The led is lighted up. Connect to the AP<br>so you can find out the DHCP ip address.<br><br>You can close this page.<br><br></div>";
                 adres ="/";   
                 actionFlag = 11; //makes it reboot in AP mode
                 toSend.replace("#zin#" , zin);
                 toSend.replace("{adres0}" , adres);
              }      
           }
         httpd_resp_send(req, toSend.c_str(), HTTPD_RESP_USE_STRLEN);
         consoleOut("IPconfig saved");
         wifiConfigsave();     
         
      }// end if ipchoice
    
    consoleOut("if we are here something went wrong");
  } // end if buf_len >1
} // end handle params

//String decode_uri(String toDec) {
//  if(toDec.indexOf("%3A") > -1) toDec.replace("%3A" , ":"); else
//  if(toDec.indexOf("%2B") > -1) toDec.replace("%2B" , "+"); else  
//  if(toDec.indexOf("%2F") > -1) toDec.replace("%2F" , "/");
//  // the minus is correct
//  return toDec;
//}
