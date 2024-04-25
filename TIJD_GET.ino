
void getTijd() {

// Variable to save current epoch time
unsigned long epochTime;

timeRetrieved = false; // stays false until time is retrieved

  consoleOut("Setting up time");
  configTime(0, 0, "pool.ntp.org");    // First connect to NTP server, with 0 TZ offset

  epochTime=getTime();
  // now should be the unixtime
  Serial.println("epochtime = " + String(epochTime));
  // we must correct this with the timezone offset
  epochTime += atoi(gmtOffset) * 60;
  Serial.println("epochtime added gmtOffset * 1min = " + String(epochTime));
  setTime(epochTime); // dit moeten we doen omdat anders zomertijd() niet werkt
    //Serial.print("epoch corrected for timezone = "); Serial.println(epochTime);
    if ( zomerTijd == true ) {
      if (zomertijd() == true) {  
        epochTime += 3600; // add one hr
        consoleOut("epochTime added 1 hr");
        setTime(epochTime);
      }
    }   
    datum = day();
    delay(10);
    switchCalc(); // calculate the switchtimes
    tijd_convert(); // to set asoTime, only when spiffs is read
}

// Function that gets current epoch time from ntp server
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
 
  if (!getLocalTime(&timeinfo)) {
    consoleOut("\n\nFailed to obtain time");
    return(0);
  }
  consoleOut("  Got the time from NTP");
  timeRetrieved = true; // stays false until time is retrieved
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
  time(&now);
  return now; 
}
