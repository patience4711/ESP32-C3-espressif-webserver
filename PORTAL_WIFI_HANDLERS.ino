// all requests that have te format PORTAL?argument are handled here

void handle_wifiportal(httpd_req_t *req)
{
  // this function handles all request for webpages
  // first read the request in a char, then find the argument
  // to decide what should be served
  consoleOut("we are in handle_wifiportal");
  char*  buf;
  size_t buf_len;
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
     buf = (char*)malloc(buf_len);
     if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
     consoleOut("the querystring is copied into buf = " + String(buf));  
     char param[QUERYKEY_MAX_LEN], dec_param[QUERYKEY_MAX_LEN] = {0}; 
     // now we can discriminate what was requested

    if(strstr(buf, "portalstyle"))
       {
       consoleOut("STYLESHEET requested");
       httpd_resp_set_type(req, "text/css");
       httpd_resp_send(req, PORTAL_STYLESHEET, HTTPD_RESP_USE_STRLEN);
       return;
       } else
     
     // if not stylesheet requested we set requestUrl
     strcpy(requestUrl, "PORTAL?");
     strcat(requestUrl, buf);
     consoleOut("requestUrl is " + String(requestUrl));
    
    if(strstr(buf, "wifiForm")) 
    {
         consoleOut("wifiform requested");
         handleForm();
         httpd_resp_send(req, toSend.c_str(), HTTPD_RESP_USE_STRLEN );
         toSend="";
         return;
    } //else

    if(strstr(buf, "ssid")) 
    {
       Serial.println("received the wifi form");
       laatsteMeting = millis(); // to refresh the time-out of the loop
       String page ="";
       char ssid[33] = "";
       char pass[40] = "";
       char param[QUERYKEY_MAX_LEN] = {0};
//      process the data and try to connect
       if (httpd_query_key_value(buf, "ssid", param, sizeof(param)) == ESP_OK) strcpy(ssid, decode_uri(String(param)).c_str());
       if (httpd_query_key_value(buf, "pass", param, sizeof(param)) == ESP_OK) strcpy(pass, decode_uri(String(param)).c_str());
       if (httpd_query_key_value(buf, "pswd", param, sizeof(param)) == ESP_OK) strcpy(pswd, decode_uri(String(param)).c_str());

        Serial.println("decoded ssid = " + String(ssid) );
        Serial.println("decoded pass = " + String(pass) );
        Serial.println("decoded pswd = " + String(pswd) );
        wifiConfigsave(); // save the admin passwd

      // trying to connect now
      // therefor we first go in wifi APSTA
        WiFi.mode(WIFI_AP_STA);
        Serial.println("Connecting to " + String(ssid));
        WiFi.begin(ssid, pass);  
        Serial.println("send confirm page  ");
      //
        if (connectWifi() == WL_CONNECTED) {
          Serial.println("youpy, connected");
          delay(10);
          //yield();
          //esp_task_wdt_reset();
            digitalWrite(led_onb, LED_UIT);
          event=101;
           
        } else {
          Serial.println("could not connect, try again");
          //esp_task_wdt_reset();
          delay(10);
          ledblink(10, 200);
          digitalWrite(led_onb, LED_AAN); // 
          event=100;
        }
         
        Serial.println("send the confirmpage");
        page = FPSTR(PORTAL_CONFIRM); 
       
        if(event==100) {
        page.replace("{text}", "connection has failed");
        } else {
        String ipa = WiFi.localIP().toString();
        page.replace("{text}", "connection success");
        page.replace("{ip}", ipa);
        }    
        httpd_resp_send(req, page.c_str(), HTTPD_RESP_USE_STRLEN);
        return;
        } else

        if ( strstr(buf, "portalClose") )
        {
        handlePortalClose(req);
        httpd_resp_send(req, toSend.c_str(), HTTPD_RESP_USE_STRLEN); 
        return;
        }

    }
  } // end buf len
}  
