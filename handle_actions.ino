void handle_actions(httpd_req_t *req)
{
  // this function handles all request for actions like reboot or buttonpress
  // first read the request in a char, then find the argument
  // to decide what should be served
  consoleOut("we are in handle_actions");
  char*  buf;
  size_t buf_len;
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
     buf = (char*)malloc(buf_len);
     if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
     consoleOut("buf = " + String(buf));  
     char param[QUERYKEY_MAX_LEN], dec_param[QUERYKEY_MAX_LEN] = {0}; 
     // now we can discriminate what was requested
//links are RUNACTION?sw=off RUNACTION?sw=off RUNACTION?reboot RUNACTION?startap

     if(strstr(buf, "sw=on"))
       {
       consoleOut("sw=on requested");
       ledsOnNow(true,false,"web");
       httpd_resp_send(req, NULL, 0);
       return;
       } else
       
       if(strstr(buf, "sw=off"))
       {
       consoleOut("sw=off requested");
       ledsOffNow(true,false,"web");
       httpd_resp_send(req, NULL, 0);
       return;
       } else
       
       if(strstr(buf, "sw=reboot"))
       {
       consoleOut("reboot requested");
       //loginBoth(request, "admin");
       actionFlag = 10;
       confirm();
       httpd_resp_send(req, toSend.c_str(), HTTPD_RESP_USE_STRLEN); 
       //request->send(200, "text/html", toSend);
       return;
     } else
     if(strstr(buf, "sw=startap"))
     {      
       consoleOut("reset");
       //loginBoth(request, "admin");
       String toSend = F("<!DOCTYPE html><html><head><script type='text/javascript'>setTimeout(function(){ window.location.href='/SHOWHTML?back'; }, 5000 ); </script>");
       toSend += F("</head><body><center><h2>wifi is erased and reboot.</h2>Wait unil the led goes on.<br><br>Then go to smartconfig pc/phone/tablet and connect to");
       toSend += getChipId(false);
       actionFlag = 11;
       httpd_resp_send(req, toSend.c_str(), HTTPD_RESP_USE_STRLEN); 
       return;
       } //else
// *****************************************************************
// *                        ota update                             *
// *****************************************************************
//     if(strstr(buf, "sw=fwupdate"))
//     {
//        //if(checkRemote( request->client()->remoteIP().toString()) ) request->redirect( "/DENIED" );
//        strcpy(requestUrl, "/");
//       // if (!request->authenticate("admin", pswd) ) return request->requestAuthentication();
//      String arg = "/onknown";
//      //char* url = (char*)arg;
//      char* url = (char*){"/tralala"};
//      printf( "Starting OTA - %s", url);
//              
//      esp_http_client_config_t config = {
//        .url = url,
//        .cert_pem = (char *)server_cert_pem_start,
//        .event_handler = _http_event_handler,
//      };
//              
//      esp_err_t ret = esp_https_ota(&config);
//      if (ret == ESP_OK) {
//         indicate_ota_inprogress(false);
//         ESP_LOGE(TAG, "Firmware upgrade SUCCESS");
//         esp_restart();
//      } else {
//         indicate_ota_inprogress(false);
//         ESP_LOGE(TAG, "Firmware upgrade failed");
//      }
//     }
//     
     
     
     }
  }
  free(buf);
}       
