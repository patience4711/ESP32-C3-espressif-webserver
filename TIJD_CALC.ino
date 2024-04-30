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
  // switch off if now > switchoffTime but:
  // after 2400 switchoffTime is recalculated and then it goes wrong
  // so we must time this recalculation. (do it when no timer has switched on)
  sun_setrise(); // calculate first sunset and rise
  consoleOut("current time in unixtime expressed: " + String(now()));
  //consoleOut(now());
  //consoleOut("");
  // we calculate unixtime at 00:00 today
  tmElements_t myElements = {0, 0, 0, 0, day(), month(), (year()-1970)};
  daystartTime = makeTime(myElements);
  consoleOut("daystart in unixtime: " + String(daystartTime));
  //consoleOut(daystartTime);
  //consoleOut("");
  // we have sunset eg 1500 414286 these are seconds
  //daystartTime is also seconds so sunsettijd is the seconds this day
  //sunsettijd = (414286 - 336000)/ 60 = 1304 minutes
  //sunrisetijd = (sunrise - daystartTime)/60; // in minutes for the purpose of zendpage
  //sunsettijd = (sunset - daystartTime)/60; //in minutes for the purpose of zendpage
  //sunrise = (daystartTime + sunrise); 
  //sunset = (daystartTime + sunset);

  // wat is the switchonTime of timer 1 expressed in sec ?
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
void timercalc(int whichOne) { // starts at 0
// first check if the calculation is needed.
// mutswitch can only be true if weekDag = y  and there are values in sw_on and sw_off and timer[whichOne]=true but otherwise we wouldn't be here
// timer[whichOne] = should be true otherwise we wouldn't be here
// there is always a value in ws_on and ws_off, unless 00:00 of something else
// no need to check that

// if weekday() + 0*7 -1 (that is sun=1 so di =3 -1 = 2
consoleOut("must we switch today timer " + String(whichOne));
consoleOut("weekday + whichOne*7 -1 = " + String(weekday() -1 + whichOne*7) ); 

if (weekDag[weekday()+whichOne*7-1] == 'y' && timerActive[whichOne] == '1') {
mustSwitch[whichOne] = true;
consoleOut("mustSwitch = true");
} else {
mustSwitch[whichOne] = false; 
consoleOut("mustSwitch = false");
}
// we calculate from weekDag 0 tm 6, relToSun 0 and 1 and switchOnOff 0 to 4 and 5 to 9
//time_t schakeltijd = 0;
int swOnuur = 0;
int sw_on = sw_calc(switchOn, whichOne*5); // get the switch on time from switchOnOff for timer 0 = 0*5
consoleOut ("sw_on = " + String(sw_on));
//consoleOut (sw_on);           

switchonTime[whichOne] = schakel_tijd(relToSunOn, whichOne, sw_on); //zonattaan1 komt in een char zonat[1] in de functie
consoleOut("the calulated switchonTime[whichOne] = " + String(switchonTime[whichOne]));
//consoleOut(switchonTime[whichOne]);
 
int sw_off = sw_calc(switchOff, whichOne*5);
consoleOut ("sw_off = " + String(sw_off));
//consoleOut (sw_off);
switchoffTime[whichOne] = schakel_tijd(relToSunOff, whichOne, sw_off);

consoleOut("the calulated switchoffTime[nummur] = " + String(switchoffTime[whichOne]) );
//consoleOut(switchoffTime[whichOne]);

// if the times are equal we cannot switch
if(switchoffTime[whichOne] == switchonTime[whichOne]){ // when equal we check if there is a 'y'
  // we start at whichOne = 0
     for(int x = whichOne*7; x < whichOne*7+7; x++){// de eerste keer 0 1 2 3 4 5 6 en de 2e keer 7 8 etc
          if (weekDag[x] == 'y') { // is there a y in this serie it would be an error
                 mustSwitch[whichOne] = false;
                 return; // terminate calculating  
           }
    }
}
// if switchof earlyer tha switchon it must be meant for the next day so we add 24 hrs
if(switchoffTime[whichOne] < switchonTime[whichOne]){
switchoffTime[whichOne] += 86400; // we add 24 hrs  
consoleOut("The switchoff time plus 24 hr expressed in unixtime =" + String(switchoffTime[whichOne]) );
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
  
  zonat[0] = relToSun[z];

  consoleOut("we are in the function schakel_tijd");
  consoleOut("relToSun[z] = ");
  consoleOut(String(relToSun[z]));
  
  consoleOut("zonat = ");
  consoleOut(String(zonat[0]));
  consoleOut("sw = " + String(sw));

  switch (zonat[0]) { // this is not understandable but seems to work, reads only the 1st position?
  case '0': //absolute
    consoleOut("case was 0 ");
    schakeltijd = sw + daystartTime;
    break;
  case '1': // before sunset
     consoleOut("case was 1 ");
     schakeltijd = sunset - sw;
     break;
  case '2': // after sunset
    schakeltijd = sunset + sw;
    consoleOut("case was 2 ");
    break;
  case '3': // before sunrise
    schakeltijd = sunrise - sw;
    consoleOut("case was 3 ");
    break;
  case '4': // after sunrise
    schakeltijd = sunrise + sw;
    consoleOut("case was 4 ");
    break;
}
return schakeltijd;
}
// *****************************************************************************
// retrieve the switchtimes out of switchOn en SwitchOff
int sw_calc(char swits[21], int x) { // x is het mummer van de timer *5 dus 0 ,5 ,10 ets
// als swits = switchOn dan
// if x = 0 this is 0 to 1 and 3 to 4 2 == de :
// if x = 5         5 to 7 and 8 to 10
   int swuur = 0;
          for (int i = x; i < x+2; i++) {
          char c = swits[i];
               if (c < '0' || c > '9') break;
               swuur *= 10;
               swuur += (c - '0');
   //how does this work? say swits=2315
   // in the loop swOnuur the 1st iteration '2' - '0'
   // the 2nd iteration 2*10 ad then + '3' - '0' = 23
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


// a universal funtion to get number of hrs and minutes from the char in a string
// if wat true it returns hrs else minutes
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

// * * * *  deze function calculats if dst is applicable * * * *
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
    // dow goes from 0 to 6, sunday is 0
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

int dow(int y, int m, int d) // returns the daynr for a specific date
{
static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
y -= m < 3;
return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}
