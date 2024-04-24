
void start_portal() {
// setup of configportal and next an infinitive loop
 
  WiFi.mode(WIFI_OFF); // otherwise the scanning fails
  delay(5000);

  Serial.println("scan start");
  scanWifi();
  Serial.println("result scan networksFound = " + String(networksFound));

  /* Soft AP network parameters */
  IPAddress apIP(192, 168, 4, 1);
  IPAddress netMsk(255, 255, 255, 0);

  Serial.println("\ncould not connect,  starting access point...");
  WiFi.softAPConfig(apIP, apIP, netMsk);
  const char* apNaam = getChipId(false).c_str();
  const char* apPasswd = "123456789";
  WiFi.softAP(apNaam, apPasswd);

  delay(500); // without delay the IP address may be blanco
  Serial.println("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  start_portalwebserver(); // server for the portal 
  
  Serial.println("started HTTPD server for the portal");
  digitalWrite(led_onb, LED_AAN);  // led aan

  // *******************************************************************************
  //                 connection loop
  // *******************************************************************************
  // now we enter a loop that we leave only after 5 minutes, 
  // or when via server.handleClient something happens   
  // bool Timed_Out = true;
  Serial.println("entering the infinitive loop with heartbeat");
  laatsteMeting = millis(); //for the time-out
  // this is the infinitive  loop 
  static unsigned long heartbeat = 0;
  while (millis() < laatsteMeting + 300*1000UL) { // 5 minuten== 300 30 == 30sec
  if ( millis() > heartbeat + 10*1000UL ) {
    //heartbeat = millis(); // each 10 sec a heartbeat
    //Serial.print("a ");
  }
  if (tryConnectFlag) { // there are credentials provided
      if (wifiConnect())  {
        // if true we are connected else not
        }  else {
        } // what to do now
   // we try to connect once so make the  flag false
   // tryConnectFlag = false; is done in wifiConnect   
  }
  
  if(Serial.available()) { // make the serial monitor work
    handle_Serial();
   }
     //DNS
    //dnsServer.processNextRequest();
     //HTTP
    //portalserver.handleClient();
    //yield();
  } 
  // ************************ end while loop ******************************* 

  //we only are here after a timeout. If we click finish we restart      
//  if (Timed_Out == true) {
    Serial.println("portal timed out, resetting...");
    ESP.restart();
//    } 
}





// ********************************************************************
//                 de homepagina van het portal
// ********************************************************************
void handlePortalRoot(httpd_req_t *req) {
    // always as we are here, portalstart is updated, so when there is activity in the
    // webinterface we won't time out. 
    // sendHeaders();
    String to_Send="";
    delay(100);
    laatsteMeting = millis(); // update portalstart
    Serial.println("handlePortalRoot, event = " + String(event));
    //to_Send = FPSTR(PORTAL_HEAD);
    delay(100);
    to_Send = FPSTR(PORTAL_HOME);
    //to_Send.replace("{haha}" , "if (window.location.hostname != \"192.168.4.1\") {window.location.href = 'http://192.168.4.1'};");
    to_Send.replace("{ma}", String(WiFi.macAddress()) );

      // if event = 101 we adapt the page with new data
      if (event == 101) {  // the 2e time that we are here we are connected 101 or not 100

          to_Send.replace("hadden", "hidden"); // verberg de configuratieknop
          to_Send.replace("close' hidden", "close'"); // show the close button
          String page = "";
          page += F("<div class=\"msg\">");
          page += F("connected to network <strong> unset<a>");
          page.replace("unset", WiFi.SSID());
          page += F("</strong><br><br>");
          page += F("The IP address is <strong><a href=\"http://");
          page += WiFi.localIP().toString();
          page += F("\">");
          page += WiFi.localIP().toString();
          page += F("</a></strong><br><br>");
          page += F("<h3 style='color:#FF0000';>Note down the ip-address and click \"close\"!</h3>");
      to_Send.replace("<strong>Currently not connected to a wifi network!</strong>" , page);
      // show the close button
      to_Send.replace("close' hidden", "close'");
      }
      if (event==100) {  // niet verbonden
          String page = "";
          page+=F("Your attempt to connect to the network has failed!<br><br>");
          page+=F("Try again, click on wifi configuration!"); 
          to_Send.replace("Currently not connected to a  wifi network!" , page);
      }
      // hadden hudden hodden hedden

       // if( static_ip[0] != '\0' && static_ip[0] != '0' && event!=101) { //show if static is present
       to_Send.replace("static' hidden", "static'"); 

      // send the response
      httpd_resp_send(req, to_Send.c_str(), HTTPD_RESP_USE_STRLEN);
      }


// *********************************************************************
//                          pagina voor invoeren credentials
// *********************************************************************
void handleForm() {
   toSend=""; 
   laatsteMeting = millis();
// if we come here for the first time we start with processing the scan
// we already did at the start

//toSend = FPSTR(PORTAL_HEAD);
toSend = FPSTR(PORTAL_FORM);
toSend.replace("{pw}",  String(pswd) );

if (networksFound == 0) {
      toSend.replace("aplijst" ,"WiFi scan not found networks. Restart configuration portal to scan again.");
    } else {
      Serial.println("make a list of found networks");
      String lijst = makeList(networksFound);
      Serial.println("lijst = "); Serial.println(lijst);
      toSend.replace("aplijst", lijst);  
    }
}

//**********************************************************************
//      process the provided data and try to connect
// **********************************************************************
bool wifiConnect() {
  // we are here because bool tryConnectFlag was true in the loop
      digitalWrite(led_onb, LED_UIT);
       tryConnectFlag=false;
       laatsteMeting = millis();

      consoleOut("we are in wifiConnect");

      WiFi.mode(WIFI_AP_STA);
      delay(500);
     
      Serial.println("Connecting to " + String(ssid));
      Serial.println("password =  " + String(pass));

      if (connectWifi() == 1) {
         Serial.println("yoepy, connected");
         ledblink(3, 500);
         event=101;
       } else {
         Serial.println("could not connect, try again");
         digitalWrite(led_onb, LED_AAN); // 
         event=100;
        } 
 }

//void handlePortalNotFound(AsyncWebServerRequest *request) {
//  String message="file not found";
//  AsyncWebServerResponse *response = request->beginResponse(404,"text/plain",message);
//  response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
//  response->addHeader("Pragma", "no-cache");
//  response->addHeader("Expires", "-1");
//  request->send (response );  
//}

// ************************************************************************
//          try to connect with new credentials
// ************************************************************************

int connectWifi() {  
  Serial.println("try connect with the new credentials");
  WiFi.begin(ssid, pass);// Start Wifi with new values.
  
  int connRes = WiFi.waitForConnectResult();
  Serial.println ("Connection result is : ");
  Serial.println( WiFi.localIP() );
  //checkFixed(); // set static ip if configured

  return connRes;
}
// *********************************************************************
//                         Closing the  portal
// *********************************************************************
void handlePortalClose(httpd_req_t *req) {
  String to_Send="";
  laatsteMeting = millis();
  to_Send = F("<!DOCTYPE html><html><head>");
  to_Send += F("<script type='text/javascript'>setTimeout(function(){ window.location.href='/'; }, 1000 ); </script>");
  to_Send += F("</head><body><center><h2>The esp is going to restart.!</h2>Do not forget to note the ip address!!<br>");
  to_Send += F("<h2>The ip address is : ");
  to_Send += WiFi.localIP().toString();
  to_Send += F("</h2></body></html>");
  httpd_resp_send(req, to_Send.c_str(), HTTPD_RESP_USE_STRLEN);
  delay(500);
  ESP.restart();
}



//void resetStatic() {
//   DebugPrintln("static ip wissen");
//  String toSend = F("<!DOCTYPE html><html><head>");
//  toSend += F("<script type='text/javascript'>setTimeout(function(){ window.location.href='/redirect'; }, 500 ); </script>");
// toSend += F("</head><body><h2>erasing ip settings, please wait... !</h2></body></html>");

////request->send ( 200, "text/html", toSend ); //zend bevestiging
//  delay(500); //wait half a second after sending the data 
//  delay(1);
//  static_ip[0] = '\0';
//  static_ip[1] = '\0';  
//}

void scanWifi() {
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      //Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  networksFound = n;

  }

// ********************************************************
//     put the found networks in a string
// ********************************************************
String makeList(int aantal) { // aantal is the number of found networks

// FIRST sort
      int indices[aantal];
      for (int i = 0; i < aantal; i++) {
        indices[i] = i;
      }
      // RSSI SORT
      for (int i = 0; i < aantal; i++) {
        for (int j = i + 1; j < aantal; j++) {
          if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
            std::swap(indices[i], indices[j]);
          }
        }
      }   
   
  String lijst="";
  for (int i = 0; i < aantal; i++) {

        String item = FPSTR(PORTAL_LIST);
        String rssiQ;
        //int quality = (WiFi.RSSI(i));
        int quality = WiFi.RSSI(indices[i]);
        Serial.print("quality = "); Serial.println(quality);
        // -50 is groter dan -80
        if (quality < -99){ continue; } // skip als kleiner dan -65 bijv -66
//        item.replace("{v}", WiFi.SSID(i));
        item.replace("{v}", WiFi.SSID(indices[i]));        
        item.replace("{r}", String(quality));
        item.replace("{i}", "");
        lijst += item;
  }
return lijst;  
}
