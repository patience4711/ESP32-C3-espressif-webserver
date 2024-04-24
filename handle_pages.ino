void handle_pages(httpd_req_t *req)
{
  // this function handles all request for webpages
  // first read the request in a char, then find the argument
  // to decide what should be served
  consoleOut("we are in handle_pages");
  char*  buf;
  size_t buf_len;
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
     buf = (char*)malloc(buf_len);
     if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
     consoleOut("the querystring is copied into buf = " + String(buf));  
     char param[QUERYKEY_MAX_LEN], dec_param[QUERYKEY_MAX_LEN] = {0}; 
     // now we can discriminate what was requested

 // links are timers basisV geoV menuV mqttV about ipconf fwupdate   
    if(strstr(buf, "style"))
       {
       consoleOut("STYLESHEET requested");
       httpd_resp_set_type(req, "text/css");
       httpd_resp_send(req, STYLESHEET, HTTPD_RESP_USE_STRLEN);
       return;
       } else
     // if not stylesheet requested we set requestUrl
     strcpy(requestUrl, "SHOWHTML?");
     strcat(requestUrl, buf);
     consoleOut("requestUrl is " + String(requestUrl));
    
    if(strstr(buf, "geo")) 
       {
         consoleOut("geo page requested");
         zendGEOpage(req);
         return;
       } else

    if(strstr(buf, "menu")) 
       {
         consoleOut("menu page requested");
         httpd_resp_send_chunk(req, HTML_HEAD, HTTPD_RESP_USE_STRLEN );
         httpd_resp_send_chunk(req, MENUPAGE, HTTPD_RESP_USE_STRLEN );
         
         httpd_resp_send_chunk(req, NULL, 0); // end response
          //toSend = FPSTR(HTML_HEAD);
         //toSend += FPSTR(MENUPAGE);
         //httpd_resp_send(req, toSend.c_str(), HTTPD_RESP_USE_STRLEN);
         return;
       } else

    if(strstr(buf, "basis")) 
       {
         consoleOut("basis config page requested");
         zendPageBasis(req);
         return;
       } else

    if(strstr(buf, "mqtt")) 
       {
        zendPageMQTTconfig(req);
        consoleOut("mqtt config page requested");
        return;
       } else

    if(strstr(buf, "timers_gen")) 
       {
         consoleOut("zendpageTimers page requested");
         zendpageTimers();
         // now toSend contains the page, we can send it
         httpd_resp_send(req, toSend.c_str(), HTTPD_RESP_USE_STRLEN);
         return;
       } else
     if(strstr(buf, "timer_ind")) 
       {
         consoleOut("individual timer page requested");
         //we need to find out for which timer, this is the argument 'welke'
         if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
              consoleOut("handle_params querystring is copied into buf = " + String(buf));
           
              char param[QUERYKEY_MAX_LEN] = {0}; 
         }
         if (httpd_query_key_value(buf, "timer_ind", param, sizeof(param)) == ESP_OK) tKeuze = atoi(param);
              zendPageRelevantTimers();
             // now toSend contains the page, we can send it
              httpd_resp_send(req, toSend.c_str(), HTTPD_RESP_USE_STRLEN);
         return;
       } else   

    if(strstr(buf, "about")) 
       {
         consoleOut("about page requested");
         handleAbout(req);
         return;
       } else
    if(strstr(buf, "ipconf")) 
       {
         consoleOut("ipconfig page requested");
         toSend = FPSTR(HTML_HEAD);
         toSend += FPSTR(IPCONFIG);
         httpd_resp_send(req, toSend.c_str(), HTTPD_RESP_USE_STRLEN);
         return;
       } 

    if(strstr(buf, "back")) 
       {
         consoleOut("back requested");
         // we must redirect to requestUrl, how to do this?
         //httpd_resp_set_hdr(req, "Location", requestUrl);
         // for now we return to the menu
         consoleOut("going back to menu");
         toSend = FPSTR(HTML_HEAD);
         toSend += FPSTR(MENUPAGE);
         httpd_resp_send(req, toSend.c_str(), HTTPD_RESP_USE_STRLEN);
         //httpd_resp_send(req, NULL, 0);
         //request->redirect( String(requestUrl)
         //httpd_resp_send(req, toSend.c_str(), HTTPD_RESP_USE_STRLEN);
         return;
       } //else

    }
  } // end buf len
}  
