
void zendpageTimers() {
consoleOut("we are in zendPageTimers");

toSend = FPSTR(HTML_HEAD);
toSend += FPSTR(TIMERCONFIG_START);

toSend.replace("tieTel", dvName );

// initial the last visited timer is opened
//tKeuze=0;
zendPageRelevantTimers();

}
// ****************************************************************************
// we gaan nu een pagina tonen met de voor te timer relevante inputs
// ****************************************************************************
void zendPageRelevantTimers() {
  toSend = FPSTR(HTML_HEAD);
  toSend += FPSTR(TIMERCONFIG_START);
  //toSend.replace("tieTel", dvName );
  //eerst weer de select en keuze terugzetten
  //replace_timerselectbox();
  
  consoleOut("we zijn op zendPageRelevantTimers");
  consoleOut("tKeuze = "); consoleOut(String(tKeuze));
  plaats_timerpage();  //plaats de timerpage voor de gekozen timer
}



// this function returns the value of vervang 
// input instace  "1" en zooattaan
String zonatt_replace( String argument1, String argument2) {

String vervang = "";
switch (argument1[0]) {
  case '0': //absolute
    vervang = argument2 + "_1";
    break;
  case '1': //before sunset
    vervang = argument2 + "_2";
    break;
  case '2': //after sunset
    vervang = argument2 + "_3";
    break;
  case '3': //before sunrise
    vervang = argument2 + "_4";
    break;
  case '4': //after sunrise
    vervang = argument2 + "_5";
    break;
    }
return vervang;
}


// **********************************************************************
//        replace the timerpage with the actuele values
// **********************************************************************
void plaats_timerpage() {
// place the timerpage 
    toSend.replace("<irame name>" , FPSTR(TIMER_GENERAL));  
    toSend.replace("{nr}" , String(tKeuze)); // replace timer nr
    if(timerActive[tKeuze] == '1') toSend.replace("tActive", "checked");
    // put back "selected" for the option in the select zonattaanwelke_1 2 3 4 of 5 
    consoleOut("pagina_replace");
    // put back where 'selected'should come
    toSend.replace(zonatt_replace(String(relToSunOn[tKeuze]), "zonattaan"), "selected"); //door zonatt_replace wordt vervang gereturned
    toSend.replace(zonatt_replace(String(relToSunOff[tKeuze]), "zonattuit"), "selected"); //door zonatt_replace wordt vervang gereturned
    
     // replace the checked boxes 
    char *grap[] = {"selzo", "selma", "seldi", "selwo", "seldo", "selvr", "selza"};
    String vervang = "";
    //weekDag
      consoleOut("replace checkboxes to show the checked ones");
    // do for this time, 7x
    for(int x=0; x<7; x++)
    { // bij 3 is dit van 21 tot 27 kleiner dan 28
          vervang = String(grap[x]); // als i=3 dan van 21-21 naar 27
          if (weekDag[tKeuze*7+x] == 'y') { toSend.replace(vervang, "checked");}
    }
    
    // we have a char with all times, can we translate to a field in the page?
    //String temp = "";
    //temp = String (switchOnOff[0],5);
    consoleOut("replace time values");
    // replace the null values with the values from spiffs, if non exists it becomes 00:00
    toSend.replace("{inx}"  , "" + String(switchOn[tKeuze*5]) + String(switchOn[tKeuze*5+1]) + String(switchOn[tKeuze*5+2]) + String(switchOn[tKeuze*5+3]) + String(switchOn[tKeuze*5+4]));
    toSend.replace("{uitx}" , "" + String(switchOff[tKeuze*5]) + String(switchOff[tKeuze*5+1]) + String(switchOff[tKeuze*5+2]) + String(switchOff[tKeuze*5+3]) + String(switchOff[tKeuze*5+4])); 
    }  
// selected timer page placed


// ********************************************************************************************
// *                   switch by timers                                              *
// ********************************************************************************************
void schakelen() {
    // *******************************************************************************************
    //                                   switch by timer timer 0  
    // *******************************************************************************************
    if (timerActive[0]=='1' && mustSwitch[0] && !hasSwitched[1] && !hasSwitched[2] && !hasSwitched[3] ) {  // if not switched on by timer 1 2 3
    //Serial.println("timer 0 should switch");
    test_schakel_in(0);
    test_schakel_uit(0);
    }
    // *******************************************************************************************
    //                                   switch by timer 1  
    // *******************************************************************************************
    if (timerActive[1]=='1' && mustSwitch[1] && !hasSwitched[0] && !hasSwitched[2] && !hasSwitched[3] ) {  // if not switched on by timer 0 2 3  
    test_schakel_in(1);
    test_schakel_uit(1);
    }
    //// *******************************************************************************************
    ////                                  switch by timer 2 
    //// *******************************************************************************************
    if (timerActive[2]=='1' && mustSwitch[2] && !hasSwitched[0] && !hasSwitched[1] && !hasSwitched[3] ) { // if not switched on by timer 0 1 3 
    test_schakel_in(2);
    test_schakel_uit(2);
    }
    //// *******************************************************************************************
    ////                                  switch by timer 3 
    //// *******************************************************************************************
    if (timerActive[3]=='1' && mustSwitch[3] &&  !hasSwitched[0] && !hasSwitched[1] && !hasSwitched[2])  { // if not switched on by timer 0 1 2 
    test_schakel_in(3);
    test_schakel_uit(3);
    }
}


void test_schakel_in(int whichOne) {
// we are here only when not switched by a timer so we don't need to check that
// we weten ook dat mustswitch true is en timersEnabled true
            if ( now() >= (switchonTime[whichOne]) && now() < switchoffTime[whichOne] && !hasSwitched[whichOne]) { 
                ledsOnNow(true, false, "timer"+String(whichOne)); 
                // whichOne == 0 or 1 if 0 then event = 5 + whichOne == 5 else 6
                //event = 3 + whichOne; // 3 of 4 of 5 of 6
                hasSwitched[whichOne] = true;
                consoleOut("switched on by timer " + String(whichOne));
            }
}
 
  void test_schakel_uit(int whichOne) {
  // if switch manual switched on it should not switch of by a timer, checkTimers() takes care for that
  // whichOne is the number of the timer
         if ( now() >= switchoffTime[whichOne] && hasSwitched[whichOne] ) { 
              ledsOffNow(true, false, "timer"+String(whichOne)); //
              event = 23 + whichOne; // 13 or 14 or 15 or 16
              // if switched off the flags are set false so we know its done
              mustSwitch[whichOne] = false;
              hasSwitched[whichOne] = false; 
              consoleOut("switched off by timer "+ String(whichOne));
             }
    }

  void disarmTimers() {
    for(int z=0;z<4;z++) {
        mustSwitch[z] = false;
        hasSwitched[z] = false;    
    }
  }
