
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



//// deze functie wordt opgeroepen na timer instellingen
////*******************************************************************************************
////              voorbereiden voor opslaan van de gegevens
//// *****************************************************************************************
//void handleTimerconfig(httpd_req_t *req) { // form action = handletimerConfig
//// we krijgen alleen serverargumenten van de betreffende timer bijv zonattaan0 dus
////lees de serverargumenten en plaats deze in de betreffende variabelen
//// de waarde in de select voor de actieve autom slaan we altijd op
//  // strcpy(timer, request->arg("timer").c_str());
//ledState = 0; // anders vertraagt alles
//String dag = "";
//if(request->hasParam("ta")) {
//dag = request->getParam("ta")->value();  
//}
//   if (dag == "on") { timerActive[tKeuze] = '1'; } else { timerActive[tKeuze] = '0';}
//    
//    // eerst terugzetten welke geselecteerd zijn in zonattaan en zonnatuit
//             String temp = "";
//             char tempChar[1]="";
//    
//              temp = request->arg("zonattaan").c_str(); // arumenten zijn 0 tm 6
//              temp.toCharArray(tempChar, 2);
//              relToSunOn[tKeuze]=tempChar[0];  // de variabele relToSunOn bijwerken
//    
//              consoleOut("tempChar = "); consoleOut(String(tempChar));
//              consoleOut("relToSunOn = "); consoleOutln(String(relToSunOn));
//    
//              temp = request->arg("zonattuit").c_str(); // arumenten zijn 0 tm 4
//              temp.toCharArray(tempChar, 2);
//              relToSunOff[tKeuze]=tempChar[0];         
//              consoleOut("relToSunOff = "); consoleOutPrintln(String(relToSunOff));                    
//            
//              //nu de inschakeltijd
//              temp = request->arg("inw").c_str();
//              temp.toCharArray(tempChar, 6);
//              for (int i=0; i<5; i++){
//                 switchOn[tKeuze*5+i] = tempChar[i];
//              }
//              temp = request->arg("uitw").c_str();
//              temp.toCharArray(tempChar, 6);
//              for (int i=0; i<5; i++){
//                 switchOff[tKeuze*5+i] = tempChar[i];
//              }      
//              // de waarden van de dagen in een array weekdag opslaan
//              char *grap[] = {"zo", "ma", "di", "wo", "do", "vr", "za"};
//              String wd = ""; 
////              String dag = "";
//              //Char ddag[7] = "  
//              //for (int i = 0; i<aantal; i++) {    
//                   for (int x = 0; x < 7; x++) {
//                    dag = request->arg(grap[x]) ; //dus zo+1
//                    //bijv dag=server.arg(zo1)
//                     if (dag == "on") {
//                    // als tKeuze=5 dan begint het bij 35 en eindigt bij 42
//                    // als tKeuze=0 dan begint het bij 0 en eindigt bij 6  
//                    weekDag[tKeuze*7+x] = 'y'; 
//                    } else {
//                    weekDag[tKeuze*7+x] = 'n';
//                    }  
//                  }
//// deze funties werken
//// nu zenden we de confirm 
// //consoleOutln("we gaan een nieuwe webpage inlezen in toSend");
// // toSend = FPSTR(CONFIRM);
//  
// // request->send(200, "text/html", toSend); //send the html code to the client
//
// timerConfigsave(); // alles opslaan in SPIFFS
// Serial.println("timer instellingen opgeslagen");
////CANDLE_OFF(); // anders reboot ie pas aan het eind van een programma
//actionFlag = 25; // recalculate the timmers
//}
  


// deze functie geeft de waarde van vervang terug
// input is bijvoorbeeld "1" en zooattaan
String zonatt_replace( String argument1, String argument2) {

String vervang = "";
switch (argument1[0]) {
  case '0': //absoluut
    vervang = argument2 + "_1";
    break;
  case '1': //voor zons ondergang
    vervang = argument2 + "_2";
    break;
  case '2': //na zons ondergang
    vervang = argument2 + "_3";
    break;
  case '3': // voor zons opkomst
    vervang = argument2 + "_4";
    break;
  case '4': // na zons opkomst
    vervang = argument2 + "_5";
    break;
    }
return vervang;
}


// **********************************************************************
//        de timerpagina met de actuele waarden terugzetten
// **********************************************************************
void plaats_timerpage() {
// place the timerpage 
    toSend.replace("<irame name>" , FPSTR(TIMER_GENERAL));  
    toSend.replace("{nr}" , String(tKeuze)); // vervang timer nummer
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
    //int i = tKeuze;
        for(int x=0; x<7; x++){ // bij 3 is dit van 21 tot 27 kleiner dan 28

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
    if (timerActive[0]=='1' && mustSwitch[0] && !hasSwitched[1] && !hasSwitched[2] && !hasSwitched[3] ) {  //als niet door timer1 of 2 is ingeschakeld
    //Serial.println("timer 0 moet schakelen");
    test_schakel_in(0);
    test_schakel_uit(0);
    }
    // *******************************************************************************************
    //                                   switch by timer 1  
    // *******************************************************************************************
    if (timerActive[1]=='1' && mustSwitch[1] && !hasSwitched[0] && !hasSwitched[2] && !hasSwitched[3] ) {  //als niet door timer0 of 2is ingeschakeld  
    test_schakel_in(1);
    test_schakel_uit(1);
    }
    //// *******************************************************************************************
    ////                                  switch by timer 2 
    //// *******************************************************************************************
    if (timerActive[2]=='1' && mustSwitch[2] && !hasSwitched[0] && !hasSwitched[1] && !hasSwitched[3] ) { // als niet door timer 0 of 1 of 3 
    test_schakel_in(2);
    test_schakel_uit(2);
    }
    //// *******************************************************************************************
    ////                                  switch by timer 3 
    //// *******************************************************************************************
    if (timerActive[3]=='1' && mustSwitch[3] &&  !hasSwitched[0] && !hasSwitched[1] && !hasSwitched[2])  {  
    test_schakel_in(3);
    test_schakel_uit(3);
    }
}


void test_schakel_in(int welke) {
// we are here only when not switched by a timer so we don't need to check that
// we weten ook dat mustswitch true is en timersEnabled true
            if ( now() > (inschakeltijd[welke]) && now() < uitschakeltijd[welke] && !hasSwitched[welke]) { 
                ledsOnNow(true, false, "timer"+String(welke)); 
                // welke == 0 or 1 if 0 then event = 5 + welke == 5 else 6
                //event = 3 + welke; // 3 of 4 of 5 of 6
                hasSwitched[welke] = true;
                consoleOut("switched on by timer " + String(welke));
            }
}
 
void test_schakel_uit(int welke) {
  // if switch manual switched on it should not switch of by a timer, checkTimers takes care for that
  // alleen uitschakelen als event = 3 of 4 of 5 0f 6 : als now() groter is dan de uitschakeltijd
  // welke is het nummer van de timer
         if ( now() > uitschakeltijd[welke] && hasSwitched[welke] ) { // als event 3 4 5 of 6 is
              ledsOffNow(true, false, "timer"+String(welke)); //
              event = 23 + welke; // 13 or 14 or 15 or 16
              // if witched off the flags are set false to prevent repetitions
              mustSwitch[welke] = false;
              hasSwitched[welke] = false; 
              consoleOut("switched off by timer "+ String(welke));
             }
    }
