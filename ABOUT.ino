const char ABOUT [] PROGMEM = R"=====(
<!DOCTYPE html><html><head>
<meta http-equiv="refresh" content="180">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta charset='utf-8'>
<link rel="stylesheet" type="text/css" href="SHOWHTML?style">
<title>ESP32-SWITCH</title>
<style>
table, th, td {border: 1px solid blue; width:font-size:12px; background-color: #ffffcc; }
th, td { padding-left: 4px; padding-right: 4px;}
th {background-color: #22ffb3;}
body {font-size:12px;} tr {height:26px;} 
</style>
<script type="text/javascript" src="SECURITY"></script>
<script> function cl() { window.location.href='/SHOWHTML?menu'; }</script>
</head><body>
<div id='msect'>
<div id="menu"><a href="#" class='close' onclick='cl();'>&times;</a></div>
</div>
<center>
<h2>ESP32-SWITCH SYSTEM DATA</h2>

)=====";

void handleAbout(httpd_req_t *req) {
char page[1536] = {0};
char temp[100]={0};
strcpy_P(page, ABOUT);

  int minutens = millis()/60000;
  int urens = minutens/60;
  int dagen = urens/24;
 
  strcat(page, "<br><table><tr><TH colspan='2'> SYSTEM INFORMATION</th></tr>" );
  strcat(page, "<tr><td>firmware version<td>ESP32-SWITCH-v0_2</tr>");
  if ( timeRetrieved ) strcat(page,"<tr><td>time retrieved<td>yes</tr>"); else strcat(page,"<tr><td>time retrieved<td>n</tr>");
  sprintf(temp, "<tr><td>systemtime<td> %d:%d " , hour(), minute());
  switch (dst) {
    case 1: strncat(temp, "summertime</td>", 19 ); break;
    case 2: strncat(temp, "wintertime</td>", 19 ); break;
    case 0: strncat(temp, "no dst set</td>", 19 ); 
  }
  strcat(page, temp);
  sprintf(temp, "<tr><td>sun-rise/set<td> %d:%d - %d:%d " , hour(sunrise), minute(sunrise) ,hour(sunset), minute(sunset));
  strcat(page, temp);
//  sprintf(temp, "<tr><td>sunset<td> %d:%d " , hour(sunset), minute(sunset));
//  strcat(page, temp);
  sprintf(temp, "<tr><td>system uptime<td>%d d %d h %d m </td>", dagen, urens-dagen*24, minutens-urens*60);
  strcat(page, temp);
  sprintf(temp, "<tr><td>wifi signalstrength<td>%lddB</td>", WiFi.RSSI());
  strcat(page, temp);

    sprintf(temp, "<tr><td>ESP CHIP ID<td>%s</td>", getChipId(true).c_str() );
    strcat(page, temp);
    sprintf(temp, "<tr><td>Free heap<td>%ld bytes</td>", esp_get_free_heap_size() );
    strcat(page, temp);
    if ( Mqtt_Format != 0 ) { //bool == y en er is een mqtt adres, ja kijk dan of er een sensor is ingesteld
     sprintf( temp,"<tr><td>mqtt clientId<td>%ld</td>", getChipId(false).c_str());
     strcat(page, temp);
    // check if connected
    if ( MQTT_Client.connected() ) { //: add a dot
       sprintf(temp, "<tr><td>mqtt connected<td>%s</td>", Mqtt_Broker );
       } else {
       sprintf(temp, "<tr><td>mqtt status<td>not connected</td>");
       }
   } else {
       sprintf(temp, "<tr><td>mqtt status<td>not configured</td>");
   }
   strcat(page, temp);
    
    sprintf(temp, "<tr><td>mustSwitch<td>%d%d%d%d", mustSwitch[0], mustSwitch[1], mustSwitch[2], mustSwitch[3] );
    strcat(page, temp);    
    sprintf(temp, "<tr><td>hasSwitched<td>%d%d%d%d",hasSwitched[0], hasSwitched[1], hasSwitched[2], hasSwitched[3] );
    strcat(page, temp);
   //Serial.println("page = " + String(page));
    //Serial.println("length = " + String(strlen(page)));

    sprintf(temp, "<tr><td>securityLevel<td>%d</td>" , securityLevel );
    strcat(page, temp);


    //Serial.println("page = " + String(page));
    Serial.println("length = " + String(strlen(page)));
    httpd_resp_send(req, page, HTTPD_RESP_USE_STRLEN);
    memset(page, 0, sizeof(page));
}
