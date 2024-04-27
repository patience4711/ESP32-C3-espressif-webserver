
void getTijd() {

  unsigned long epochTime; // Variable to save current epoch time in

  timeRetrieved = false; // stays false until time is retrieved

  consoleOut("Setting up time");
  configTime(0, 0, "pool.ntp.org");    // First connect to NTP server, with 0 TZ offset

  epochTime=getTime(); // retrive the time from ntp server
  // if this failed we return.
  if(!timeRetrieved) return;
  Serial.println("epochtime = " + String(epochTime)); // unixtime
  // we must correct this with the timezone offset
  epochTime += atoi(gmtOffset) * 60;
  Serial.println("epochtime added gmtOffset * 1min = " + String(epochTime));
  setTime(epochTime); // this is needed because we must run zomertijd()
  if ( zomerTijd == true ) {
     if (zomertijd() == true) {  
        epochTime += 3600; // add one hr
        consoleOut("epochTime added 1 hr");
        setTime(epochTime);
     }
  }   
  datum = day(); // set the new date
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
  timeRetrieved = true; 
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
  time(&now);
  return now; 
}
