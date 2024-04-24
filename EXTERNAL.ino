//// find out if the request comes from inside the network
//bool checkRemote(String url) {
////check if the first 9 characters of the router 192.168.0
//    if(securityLevel == 0) return false; // never remote
//    if ( url.indexOf(WiFi.gatewayIP().toString().substring(0, securityLevel)) == -1 ) return true; else return false;
//}
//
//// we come here when an unknown request is done
//void handleNotFound(AsyncWebServerRequest *request) {
//  
//bool intern = false;
//if(!checkRemote( request->client()->remoteIP().toString()) ) intern = true;
//
//// **************************************************************************
////             R E S T R I C T E D   A R E A
//// **************************************************************************
//// access only from inside the network
//if ( intern ) {    //consoleOutln("the request comes from inside the network");
//     String serverUrl = request->url().c_str();
//     Serial.println("serverUrl = " + serverUrl);             
//
//     if(serverUrl == "/SW=ON") {
//      Serial.println("sw=on found");
//      //ledsOnNow(true,false,"web");
//      actionFlag=31;
//      request->send ( 200, "text/plain", "switched on" );
//      return; 
//     }
//     if(serverUrl == "/SW=OFF") {
//      Serial.println("sw=off found");
//      actionFlag=32;
//      //ledsOffNow(true,true,"web");
//      request->send ( 200, "text/plain", "switched off" );
//      return; 
//     }
//     // if we are here, no valid api was found    
//     request->send ( 200, "text/plain", "ERROR this link is invalid, go back <--" );//send not found message
//     }             
//    else 
//     { // not intern
//      //consoleOut("\t\t\t\t unauthorized, not from inside the network : ");
//      request->send ( 200, "text/plain", "ERROR you are not authorised, go back <--" );//send not found message
//     }
//}
