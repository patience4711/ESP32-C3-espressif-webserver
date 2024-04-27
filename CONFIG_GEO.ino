const char GEOPAGE[] PROGMEM = R"=====(
<body>

<div id='msect'><div id='bo'></div>
<div id="menu">
<a href="#" id="sub" onclick='submitFunction("/SHOWHTML?back")'>save</a>
<a href="#" class='close' onclick='cl();'>&times;</a>
</div>

</div>
<div id='msect'>
<br><kop>LOCATION SETTINGS</kop><br><br>
</div>
<div id='msect'>
  <div class='divstijl' style='height:52vh;'>
  <form id='formulier' method='get' action='parameters' oninput='showSubmit()'>
  <center><table>
  
  <tr><td style='width:140px;'>latitude &deg<td><input class='inp3' name='lati' length='8' placeholder='latitude' value='{be}'></input></tr>
  <tr><td>longitude &deg<td><input  class='inp3' name='longi' length='8' placeholder='longitude' value='{le}'></input></tr> 
  <tr><td>timezone<td><input class='inp2' name='tz' length='6' placeholder='minutes relative to GMT' value='{tz}'></input></tr>
  <tr><td>dst y/n<td><input type='checkbox' style='width:30px; height:30px;' name='ts' #check></input></tr></table> 
  
  </div></div><br></form>
<br></body></html>
)=====";
//
//
//
void zendGEOpage( httpd_req_t *req ) {
// this function sends the geo config webpage
    String webPage = FPSTR(HTML_HEAD);
    webPage += FPSTR(GEOPAGE);  
    
    // put back the data
    
    webPage.replace("{le}",  String(longi,3) );
    webPage.replace("{be}",  String(lati,3) );
    
    webPage.replace("{tz}",  String(gmtOffset) );
    
    if (zomerTijd) { 
    //  Serial.println("zomerTijd = true");
      webPage.replace("#check", "checked");
      } 
    httpd_resp_send(req, webPage.c_str(), HTTPD_RESP_USE_STRLEN);

    webPage="";
}
//
void handleGEOconfig(httpd_req_t *req) {
consoleOut("get time settings parameters");
  char*  buf;
  size_t buf_len;
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
        buf = (char*)malloc(buf_len);

        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
        Serial.println("the querystring is copied into buf");
        Serial.println("buf = " + String(buf));  
        char param[QUERYKEY_MAX_LEN], dec_param[QUERYKEY_MAX_LEN] = {0}; 
         if (httpd_query_key_value(buf, "le", param, sizeof(param)) == ESP_OK) {
              Serial.println("Found URL query parameter pwm");
              Serial.println("param = " + String(param));
              longi = atof(param); // String(param).toFloat() ??
              Serial.println("longi now = " + String(longi)); 
          }
        }
  }
//char static_ip2[16] = "";

  //de serverargumenten verzamelen

//longi = req->getParam("le")->value().toFloat();
//lati = req->getParam("be")->value().toFloat();
//strcpy(gmtOffset, req->getParam("tz")->value().c_str());

//BEWARE CHECKBOX
//String dag = "";
//if(req->hasParam("ts")) {
//dag = req->getParam("ts")->value();  
//}
//   if (dag == "on") { zomerTijd = true; } else { zomerTijd = false;}
   
      wifiConfigsave();

      actionFlag=25; // recalculate with these settings 
} 
