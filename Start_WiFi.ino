// ************************************************************************************
// *                        START wifi
// ************************************************************************************
void start_wifi() {
 WiFi.softAPdisconnect(true);
 WiFi.setHostname(getChipId(false).c_str()); //troubles with the hostname
 WiFi.mode(WIFI_STA);
 Serial.println("starting wifi ");
 delay(1000);
//Serial.println("start wifi 3");
// WiFi.setHostname(getChipId(false).c_str()); 
 WiFi.hostname(getChipId(false).c_str());
//Serial.println("start wifi 4");

// WiFi.mode(WIFI_STA); // geen ap op dit moment 

// we gaan 10 pogingen doen om te verbinden
// met de laatst gebruikte credentials
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print("*");
     WiFi.begin();
     event+=1;
     if (event==10) {break;}
  }
// als het verbinden is mislukt gaan we naar het configportal  
  if (event>9) {
     event=0;
     Serial.println("\nWARNING connection failed");
    digitalWrite(led_onb, LED_AAN); // the onboard led on
     start_portal();
     //connect_manager();
      } else {
     Serial.print("\nconnection success, ip = ");
     Serial.println(WiFi.localIP());
     }
   Serial.print("# connection attempts = ");  //Serial.println(event);
   event=0; // we kunnen door naar de rest
   checkFixed();
   Serial.println(WiFi.localIP());
   //start_server();
   start_webserver(); // start the server
}
//// *************************************************************************
////                      connect manager smart config doesn't work
//// *************************************************************************
//void connect_manager() {
//
//  WiFi.mode(WIFI_AP_STA); // ap + station mode
//  
//  WiFi.beginSmartConfig();
//
//  //Wait for SmartConfig packet from mobile
//  Serial.println("Waiting for SmartConfig.");
//  // this is an ever lasting loop, we time it out to reboot after 5 minutes
//  Serial.println("entering the infinitive loop with heartbeat");
//  laatsteMeting = millis(); //voor de time-out
//  unsigned long heartbeat = 0;
//  while (!WiFi.smartConfigDone()) {
//    delay(500);
//    if ( millis() > heartbeat + 10*1000UL ) {
//    heartbeat = millis(); // elke 10 sec een heartbeat
//    Serial.print("a ");
//    }
//    //Serial.print(".");
//    if (millis() > laatsteMeting + 300*1000UL) break;  // 5 minuten== 300 30 == 30sec
//    // SERIAL: *************** see if there is data available **********************
//    if(Serial.available()) {
//       handle_Serial();
//    }
//  }
//  // when we are here we broke out of the while loop
//  // either is WiFi.smartConfigDone() or the loop timed out
//  
//  if(!WiFi.smartConfigDone()) {
//   // so we timed out
//   consoleOut("portal timed out, resetting...");
//   ESP.restart();
//  }
//
//  Serial.println("\nSmartConfig received.");
//  //Wait for WiFi to connect to AP
//  // 10 attemps to connect with the new credentials 
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print("*");
//     WiFi.begin();
//     event+=1;
//     if (event==10) {break;}
//  }
//if(event < 10) {
//  Serial.println("WiFi Connected.");
//  Serial.print("IP Address: ");
//  Serial.println(WiFi.localIP());
//} else {
//  Serial.println("no wifi connection");
//  delay(500);
//  ESP.restart();
//}

//{
    //Initialize NVS
//    esp_err_t ret = nvs_flash_init();
//    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//        ESP_ERROR_CHECK(nvs_flash_erase());
//        ret = nvs_flash_init();
//    }
//    ESP_ERROR_CHECK(ret);
//
//    dpp_enrollee_init();
//}

//}
 // ********************************************************************
//             check if there must come a static ip
// ********************************************************************
void checkFixed() {
  // we come here only when wifi connected
    char GATE_WAY[16]="";
    IPAddress gat=WiFi.gatewayIP();
    sprintf(GATE_WAY, "%d.%d.%d.%d", gat[0], gat[1], gat[2], gat[3]);
    //DebugPrint("GATE_WAY in checkFixed = nu: "); //DebugPrintln(String(GATE_WAY));
    //DebugPrint("static_ip in checkFixed = nu: "); //DebugPrintln(String(static_ip));

    if (static_ip[0] != '\0' && static_ip[0] != '0') {
      //DebugPrintln("we need s static ip  Custom STA IP/GW/Subnet");
      IPAddress _ip,_gw,_sn(255,255,255,0); // declare 
      _ip.fromString(static_ip);
      _gw.fromString(GATE_WAY);//  if (ssid != "") {
      WiFi.config(_ip, _gw, _sn);
      //DebugPrintln(WiFi.localIP());
  } else {
      //DebugPrintln("trying to get rid of wificonfig");
     // WiFi.config(0u, 0u, 0u);     
  }
}

//void loginBoth(AsyncWebServerRequest *request, String who) {
//  String authFailResponse = "<h2>login failed <a href='/'>click here</a></h2>";
//  if (who == "admin" ){
//  const char* www_realm = "login as administrator."; 
//    if (!request->authenticate("admin", pswd)) return request->requestAuthentication();
//  }
//  if (who == "both" ){
//  const char* www_realm = "login as administrator or user."; 
//    if (!request->authenticate("admin", pswd) && !request->authenticate("user", userPwd)) return request->requestAuthentication();
//  }
//}
