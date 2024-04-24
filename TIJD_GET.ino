
void getTijd() {

// Variable to save current epoch time
unsigned long epochTime;

timeRetrieved = false; // stays false until time is retrieved

// Set timezone to GMT
//setenv("TZ", "GMT0BST,M3.5.0/1,M10.5.0", 1);
//tzset();

  consoleOut("Setting up time");
  configTime(0, 0, "pool.ntp.org");    // First connect to NTP server, with 0 TZ offset

  epochTime=getTime();
  //epochTime=getLocalTime();
  // now should be the unixtime
  Serial.println("epochtime = " + String(epochTime));
  // we must correct this with the timezone offset
  epochTime += atoi(gmtOffset) * 60;
  Serial.println("epochtime added gmtOffset * 1min = " + String(epochTime));
  setTime(epochTime); // dit moeten we doen omdat anders zomertijd() niet werkt
    //Serial.print("epoch gecorrigeerd voor timezone = "); Serial.println(epochTime);
    if ( zomerTijd == true ) {
      if (zomertijd() == true) {  
        epochTime += 3600; // een uur erbij
        consoleOut("epochTime added 1 hr");
        setTime(epochTime);
      }
    }   

//}

    datum = day();
    delay(10);
    switchCalc(); // calculate the switchtimes
    tijd_convert(); // to set asoTime, only when spiffs is read
}

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
 
  if (!getLocalTime(&timeinfo)) {
    consoleOut("Failed to obtain time");
    return(0);
  }
  consoleOut("  Got the time from NTP");
  timeRetrieved = true; // stays false until time is retrieved
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
  time(&now);
  return now; // returns a time_t, shouldn't this be an unsigned long?
}


//void getTijd() {
//
//  timeRetrieved = false; // stays false until time is retrieved  
//  timeClient.begin();
//  //unsigned long epochTime = 0;
//  //get the time, if fails we try again during healthcheck
//
//  timeClient.update();
//  unsigned long epochTime = timeClient.getEpochTime();
//
//
//  //Serial.print("Epoch Time: ");
//  //Serial.println(epochTime);
//
//    // now convert NTP time into unix tijd:
//    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
//    //const unsigned long seventyYears = 2208988800UL;
//    // subtract seventy years:
////    unsigned long epoch = secsSince1900 - seventyYears + atof(timezone) * 60; // * 60 weggehaald omdat timezone in minuten is
////    unsigned long epochTime = timeClient.getEpochTime;
//    // we have to do this conditional, if time retrieving failed
//    if (epochTime < 1000) {
//    ntpUDP.stop();
//    return;
//  } else {
//   
//    epochTime += atoi(gmtOffset) * 60;
//    setTime(epochTime); // dit moeten we doen omdat anders zomertijd() niet werkt
//    //Serial.print("epoch gecorrigeerd voor timezone = "); Serial.println(epochTime);
//    if ( zomerTijd == true ) {
//    //Serial.print("zomerTijd[0] = een o dus on "); Serial.println(String(zomerTijd));  
//      if (zomertijd() == true) {  
//        epochTime += 3600; // een uur erbij
//        setTime(epochTime);
//        //consoleOut("epoch corrected with dts = "); //consoleOutln(epochTime);
//      }
//    }
//    timeRetrieved=true;  
//    //Update_Log(1, "got time");
//    }
//    //consoleOut(" Unix time epoch = ");
//    //consoleOutln(epochTime);
//  
//    ntpUDP.stop();
////
////  // de tijd is nu opgehaald en in setTime gestopt
////  // dus met de tijden die met setTime zijn opgeslagen gaan we  alle berekeningen doen
////  
////consoleOut("het uur is ");  //consoleOut(hour());
////consoleOut("   aantal minuten "); //consoleOutln(minute());
//    datum = day();
////
////yield();
//    delay(10);
//    switchCalc(); // calculate the switchtimes
//
//}
