void sun_setrise() {

float OUR_longtitude = longi;
float OUR_latitude = lati;
float OUR_timezone = atof(gmtOffset);  //   120                     // localtime with UTC difference in minutes

sunMoon  sm;

//  tmElements_t  tm;                             // specific time
//  tm.Second = 0;
//  tm.Minute = 12;
//  tm.Hour   = 12;
//  tm.Day    = 3;
//  tm.Month  = 8;
//  tm.Year   = 2016 - 1970;
//  time_t s_date = makeTime(tm);
//  Serial.println("RTC has set the system time");
  sm.init(OUR_timezone, OUR_latitude, OUR_longtitude);


      sunrise = sm.sunRise();
      sunset  = sm.sunSet();
      
      if ( zomerTijd ) { // is time savings applicable
            if (zomertijd() == true) { // check if it is summertime now
              sunrise = sunrise + 3600; // seconds
              sunset  = sunset + 3600;
              dst = 1;
              } else {
              dst = 2;  
              }
       } else { 
           dst = 0; 
           }
 }

void printDate(time_t date) {
  char buff[20];
  sprintf(buff, "%2d-%02d-%4d %02d:%02d:%02d",
  day(date), month(date), year(date), hour(date), minute(date), second(date));
  Serial.print(buff);
}

// ********** calculate the switch moment for today *********************************
void switchCalc() {
  // what happens here:
  // we know sunset for today
  // we know switchtime (if after 2400 we add 2400 
  // if now > sunset dan inschakelen
  // uitschakelen als now > uitschakeltijd maar:
  // na 2400 wordt de uitschakeltijd opnieuw berekend en dan gaat het mis
  // dus deze berekening moeten we timen.
sun_setrise(); // calculate first sunset and rise
consoleOut("current time in unixtime expressed: " + String(now()));
//consoleOut(now());
//consoleOut("");
 // we caalculate unixtime at 00:00 today
tmElements_t myElements = {0, 0, 0, 0, day(), month(), (year()-1970)};
dagbegintijd = makeTime(myElements);
consoleOut("daystart in unixtime: " + String(dagbegintijd));
//consoleOut(dagbegintijd);
//consoleOut("");
// we have sunset eg 1500 414286 these are seconds
//dagbegintijd is also seconds so sunsettijd is the seconds this day
//sunsettijd = (414286 - 336000)/ 60 = 1304 minutes
//sunrisetijd = (sunrise - dagbegintijd)/60; // in minutes for the purpose of zendpage
//sunsettijd = (sunset - dagbegintijd)/60; //in minutes for the purpose of zendpage
//sunrise = (dagbegintijd + sunrise); 
//sunset = (dagbegintijd + sunset);

// wat is the inschakeltijd of timer 1 expressed in sec ?
// we know sunset in unixtime. we add or sustract th amount
// of seconds from the provided time 
// we have switchon1 and switchoff1, should be converted to seconds
// weekday gvves a number, zondag = 1
//char b = timer[0];

          timercalc(0); 
          timercalc(1);          
          timercalc(2);
          timercalc(3);          
      

}  // end switchcalc

// *****************************************************************
//                    calculations timers van 0 t/m ...
//******************************************************************
void timercalc(int nummer) { // begint bij 0
// first check if the calculation is needed.
// mutswitch can only be true if weekDag = y  and there are values in sw_on and sw_off and timer[nummer]=true but otherwise we wouldn't be here
// timer[nummer] = should be true otherwise we wouldn't be here
// there is always a value in ws_on and ws_off, unless 00:00 of something else
// no need to check that

// als weekday() + 0*7 -1 (dat is sun=1 dus di =3 -1 = 2
consoleOut("must we switch today timer " + String(nummer));
//consoleOut(nummer);
consoleOut("weekday + nummer*7 -1 = " + String(weekday() -1 + nummer*7) ); 

if (weekDag[weekday()+nummer*7-1] == 'y' && timerActive[nummer] == '1') {
mustSwitch[nummer] = true;
consoleOut("mustSwitch = true");
} else {
mustSwitch[nummer] = false; 
consoleOut("mustSwitch = false");
}
// we calculate form weekDag 0 tm 6, relToSun 0 and 1 and switchOnOff 0 to 4 and 5 to 9
//time_t schakeltijd = 0;
int swOnuur = 0;
int sw_on = sw_calc(switchOn, nummer*5); // get the switch on time from switchOnOff for timer 0 = 0*5
consoleOut ("sw_on = " + String(sw_on));
//consoleOut (sw_on);           

inschakeltijd[nummer] = schakel_tijd(relToSunOn, nummer, sw_on); //zonattaan1 komt in een char zonat[1] in de functie
consoleOut("the calulated inschakeltijd[nummer] = " + String(inschakeltijd[nummer]));
//consoleOut(inschakeltijd[nummer]);
 
int sw_off = sw_calc(switchOff, nummer*5);
consoleOut ("sw_off = " + String(sw_off));
//consoleOut (sw_off);
uitschakeltijd[nummer] = schakel_tijd(relToSunOff, nummer, sw_off);

consoleOut("the calulated uitschakeltijd[nummur] = " + String(uitschakeltijd[nummer]) );
//consoleOut(uitschakeltijd[nummer]);

// als de tijden aan elkaar gelijk zijn mogen we niet schakelen
// checken of er een y in 
if(uitschakeltijd[nummer] == inschakeltijd[nummer]){ // als ze gelijk zijn dan kijken of er een y is
  // we beginnen bij nummer = 0
     for(int x = nummer*7; x < nummer*7+7; x++){// de eerste keer 0 1 2 3 4 5 6 en de 2e keer 7 8 etc
          if (weekDag[x] == 'y') { // staat er een y in deze reeks dan is dat fout
                 mustSwitch[nummer] = false;
                 return; // niet verder rekenen  
           }
    }
}
if(uitschakeltijd[nummer] < inschakeltijd[nummer]){
uitschakeltijd[nummer] += 86400; // we tellen er 24 uur bij 
consoleOut("The switchoff time plus 24 hr expressed in unixtime =" + String(uitschakeltijd[nummer]) );
//consoleOut(uitschakeltijd[nummer]); // sunset is in minuten
//consoleOut("");
}

consoleOut ("*************************************************************************");
// 
}



// *********************************************************************************
// this function calculates the switchtimes relative to sun-rise or -set
// *********************************************************************************
time_t schakel_tijd(char relToSun[5], int z, int sw) {
  // we feed this function with the position in relToSun 0 ,1 ,2 ,3 ,4 etc and sw_of/on 
  char zonat[1];
  time_t schakeltijd = 0;
  
  // z = number  so 0 , 1 , 2, etc
  
  zonat[0] = relToSun[z];

  consoleOut("we are in the function schakel_tijd");
  consoleOut("relToSun[z] = ");
  consoleOut(String(relToSun[z]));
  
  consoleOut("zonat = ");
  consoleOut(String(zonat[0]));
  consoleOut("sw = " + String(sw));
  //consoleOut(sw);

//zonat[0] = relToSun[z];
switch (zonat[0]) { // dit is onbegrijpelijk maar lijkt te werken leest alleen de 1e positie?
  case '0': //absoluut
    consoleOut("case was 0 ");
   schakeltijd = sw + dagbegintijd;
//    zonstring_uit = " hr";
    break;
  case '1': //voor zons ondergang
     consoleOut("case was 1 ");
    schakeltijd = sunset - sw;
//    zonstring_uit = " voor zonsondergang";
    break;
  case '2': //na zons ondergang
    schakeltijd = sunset + sw;
       consoleOut("case was 2 ");
 //   zonstring_uit = " na zonsondergang";
    break;
  case '3': // voor zons opkomst
    schakeltijd = sunrise - sw;
       consoleOut("case was 3 ");
//    zonstring_uit = " voor zonsopkomst";
    break;
  case '4': // na zons opkomst
    schakeltijd = sunrise + sw;
       consoleOut("case was 4 ");
//    zonstring_uit = " na zonsopkomst";
    break;
}
return schakeltijd;
}
// *****************************************************************************
// haalt de schakeltijden op uit switchOn en SwitchOff
int sw_calc(char swits[21], int x) { // x is het mummer van de timer *5 dus 0 ,5 ,10 ets
// als swits = switchOn dan
//als x = 0 dan wordt dit 0 tm 1 en 3 tm 4 2 == de :
//als x = 5               5 tm 7 en 8 tm 10
   int swuur = 0;
          for (int i = x; i < x+2; i++) {
          char c = swits[i];
               if (c < '0' || c > '9') break;
               swuur *= 10;
               swuur += (c - '0');
   //hoe werkt dit? Stel swits=2315
   // in de lus wordt swOnuur de eerste cyclus '2' - '0'
   // de 2e cyclus 2*10 en dan + '3' - '0' = 23
          }

   int swminuut = 0;
          for (int i = x+3; i < x+5; i++) {
          char c = swits[i];
               if (c < '0' || c > '9') break;
               swminuut *= 10;
               swminuut += (c - '0');
          }
            int sw = (swuur * 60 + swminuut) * 60;
            return sw;
}


void tijd_convert () {
int asouur = tijd_cvrt( aso, true );
int asominuut = tijd_cvrt( aso, false );
asoTime = (asouur * 60 + asominuut) * 60; //seconds
consoleOut("asoTime = " + String(asoTime)); 
}


// een universele funtie om het aantal uren en minuten uit de char in een string te zetten
// als wat true dan geeft ie de uren en anders de minuut
int tijd_cvrt(char TIJD[6], bool wat) {
     int uur = 0;
     for (int i = 0; i < 2; i++) {
          char c = TIJD[i];
               if (c < '0' || c > '9') break;
               uur *= 10;
               uur += (c - '0');
          }
     int minuut = 0;
     for (int i = 3; i < 5; i++) { // 2 = de :
          char c = TIJD[i];
               if (c < '0' || c > '9') break;
               minuut *= 10;
               minuut += (c - '0');
          }
if (wat) { return uur; } else { return minuut;}
}

// ************************* deze functie bepaalt of zomertijd van toepassing is
bool zomertijd() {

    int eerstemrt = dow(year(), 3 ,1);
    int zdmrt;

    if (eerstemrt == 0) {
     zdmrt = 1;
    } else {
     zdmrt = 1 + (7 - eerstemrt);
    }

    while(zdmrt <= 24){
      zdmrt = zdmrt + 7;
    }

    int eersteoct = dow(year(), 10 ,1);
    int zdoct ;
    //Serial.print(eersteoct);
    //Serial.println("");
    // dow gaat van 0 naar 6, zondag is 0
    //Serial.print("de eerste zondag van zondag van oct is dag ");
    if (zdoct == 0) {
    zdoct = 1;
    } else {
    zdoct = 1+(7-eersteoct);
    }

    while(zdoct <= 24){
      zdoct = zdoct + 7;
    }

    if(((month() == 3 and day() >= zdmrt) or month() > 3) and ((month() == 10 and day() < zdoct) or month() < 10)) {  
    consoleOut("it is dst time");
    return true;
    } else {
    consoleOut(F("it is not dst time"));
    return false; 
    }
}

int dow(int y, int m, int d) // returns what daynr for a specific date
{
static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
y -= m < 3;
return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}
