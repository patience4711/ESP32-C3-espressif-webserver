
void handle_data(httpd_req_t *req) 
{
// this handles all data requests from the homepage or other webpages
// first decide for what the request is, we do this by evaluating the argument

// first read the request into a char
  consoleOut("we are in handle_data 1");
  char*  buf;
  size_t buf_len;
  buf_len = httpd_req_get_url_query_len(req) + 1;
 
  if (buf_len > 1) // there is data in buf
  {
     consoleOut("buf_len = " + String(buf_len) );
     delay(500);
     buf = (char*)malloc(buf_len);
     consoleOut("handle_data 2");
     if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
     consoleOut("handle_params querystring copied");  

    // now we place the filters to decide what is requested
    // the uri looks like getData with an argument eg getData?knop or getData?sldr=123   
    
    if(strstr(buf, "sldr")) {
       consoleOut("slider update");
       char param[QUERYKEY_MAX_LEN] = {0};
    
      if (httpd_query_key_value(buf, "sldr", param, sizeof(param)) == ESP_OK) duty = atoi(param);
      consoleOut("Found URL query parameter sldr");
      consoleOut("param = " + String(param));
      inschakeltijdstip = now()-10;
      if(duty == 0) ledState = 0; else ledState = 1;
      set_pwm(duty);
      consoleOut("duty cycle set to " + String( ledc_get_duty(LEDC_MODE, LEDC_CHANNEL) ) ); 
      eventSend(0); //update the webpage
      httpd_resp_send(req, NULL, 0);

      } else    
    if(strstr(buf, "knop")) {
    // link = getData?knop
    consoleOut("knop update");
     consoleOut("getData?knop dvName = " + String(dvName));
     // We face a problem here: While fading, the value get_duty isn't correct
     // so we use the fade target which is duty. In case of ledState=0 this should be 0
     //otherwise w'sd wait too long before sending the event that trigger this request
     int target = 0;
     if (ledState != 0 ) target = duty;
     // so if target == 0, ledState = always 0 too, we dont need to send state 
     String json = "{";
     json += "\"name\":\"" + String(dvName) + "\",";
     // json += "\"state\":\"" + String(ledState) + "\",";
     json += "\"duty\":\"" + String(target) + "\"";
     json += "}";
     httpd_resp_send(req, json.c_str(), HTTPD_RESP_USE_STRLEN);
     json = String(); 
     }
    }
  } 
free(buf);
}
