const char BASISCONFIG[] PROGMEM = R"=====(
<body>
<div id='msect'>
  <div id='bo'></div>
    <ul><li id='fright'><span class='close' onclick='cl();'>&times;</span>
    <li id='sub'><a href='#' onclick='submitFunction("/SHOWHTML?back")'>save</a></li></ul><br></div>

<div id='msect'>
<kop>GENERAL SETTINGS</kop>
</div>

<div id='msect'>
  <div class='divstijl'>
  <form id='formulier' method='get' action='parameters' oninput='showSubmit()'>
  <center><table>
  <tr><td>switch name:<tr><td><input class='inp6' name='swname' maxlength='21' title='no spaces, see help' value='{nm}'></input></tr>
  <tr><td>user passwd:<tr><td><input  class='inp5' name='pw1' length='11' placeholder='max. 10 char' value='{pw1}' pattern='.{4,10}' title='between 4 en 10 characters'></input> 
  </td></tr>
  <tr><td>security:<tr><td><input class='inp4' name='bev' value='{bev}' type='time'></input></tr>

  </td></tr></table></form>
  </table>
  </div><br>
</div>
</body></html>
)=====";

void zendPageBasis(httpd_req_t *req) {
  String(webPage)="";
    //consoleOutln("we zijn nu op zendPageBasis");
    webPage = FPSTR(HTML_HEAD);
    webPage += FPSTR(BASISCONFIG);
    
Serial.println("config basis dvName : " + String(dvName));
    // replace data
    webPage.replace( "'{nm}'" , "'" + String(dvName) + "'") ;
    webPage.replace( "'{pw1}'" , "'" + String(userPwd) + "'") ;
    webPage.replace("'{bev}'" , "'" + String(aso) + "'") ;

    httpd_resp_send(req, webPage.c_str(), HTTPD_RESP_USE_STRLEN);
    //request->send(200, "text/html", webPage);
    webPage=""; // free up
}


//void handleBasisconfig(AsyncWebServerRequest *request) { // form action = handleConfigsave
//// verzamelen van de serverargumenten   
//   strcpy(userPwd, request->arg("pw1").c_str());
//   strcpy(dvName, request->arg("dvName").c_str());
//   // we invert this value to comply with the webpage
//   //del = request->arg("delay").toInt();
//   strcpy(aso, request->arg("bev").c_str());    
//// BEWARE checkbox !!!!!!!
////     String dag="";
////     if( request->hasArg("timer") ) {
////     dag = request->getParam("timer")->value();  // mqselect
////     }
////     if ( dag == "on") { timersEnabled = true; } else { timersEnabled = false; }
//   
//
//   basisConfigsave();  // alles opslaan
//
//  actionFlag=25; // recalculates the time with these new values 
//}
