const char TIMERCONFIG_START[] PROGMEM = R"=====(
<style>
@media only screen and (max-width: 600px) {
  li a{
    width: 18px;
    padding: 14px 2px 10px 16px;
  }
  li b{width: 22px;}
}
</style>

<body>

<div id='main-sect'>
<div id='bo'></div>
    <ul><li><b>timers:</b></li>
    <li id='fright'><span class='close' onclick='cl();'>&times;</span></li>
    <li><a href='/SHOWHTML?timer_ind=0' > 0</a></li>
    <li><a href='/SHOWHTML?timer_ind=1' > 1</a></li>
    <li><a href='/SHOWHTML?timer_ind=2' > 2</a></li>
    <li><a href='/SHOWHTML?timer_ind=3' > 3</a></li>
    </ul>
</div>

<div id='main-sect'>
  <center><kop>TIMER CONFIG </kop></center><br>
</div>

<div id='main-sect'>
<center><div class='divstijl'>

<form id='formulier' method='get' action='parameters' oninput='showSubmit()'>
  <center><irame name>
  </div>
<div><br></form>
<div id='msect'>
  <ul>
  <li id='sub'><a href='#' onclick='submitFunction("/SHOWHTML?back")'>save</a></li>
  <br></div>
</div>
</body></html>
 )=====";
// voor geen sonoff

 
 const char TIMER_GENERAL[] PROGMEM = R"=====(
<center><table><tr><td style='width:100px;'><h4> TIMER {nr} </h4>
<td style='width:40px;'>active?<td><input type='checkbox' name='ta' tActive>
</table>
<table>
<tr><td>on:<td><input class='inp2' name='inw' value='{inx}' pattern='(?:[01]|2(?![4-9])){1}[0-9]{1}:[0-5]{1}[0-9]{1}' title='hh:mm'>
<td><select name='zonattaan' class='sb1'>
<option value='0' zonattaan_1>absolute</option>
<option value='3' zonattaan_4>before sunrise</option>
<option value='4' zonattaan_5>after sunrise</option>
<option value='1' zonattaan_2>before sunset</option>
<option value='2' zonattaan_3>after sunset</option>

</select></td></tr>
<tr><td>off:<td><input class='inp2' name='uitw' value='{uitx}' pattern='(?:[01]|2(?![4-9])){1}[0-9]{1}:[0-5]{1}[0-9]{1}' title='hh:mm' >
<td><select name='zonattuit' class='sb1'>
<option value='0' zonattuit_1>absolute</option>
<option value='3' zonattuit_4>before sunrise</option>
<option value='4' zonattuit_5>after sunrise</option>
<option value='1' zonattuit_2>before sunset</option>
<option value='2' zonattuit_3>after sunset</option>

</select></td></tr></table>
<br><table>
<thead><tr style='height:20px;'>
<th> su </th>
<th> mo </th>
<th> tu </th>
<th> we </th>
<th> th </th>
<th> fr </th>
<th> sa </th>
</tr></thead>
<tr>
<td><input type='checkbox' name='su' selzo>
<td><input type='checkbox' name='mo' selma>
<td><input type='checkbox' name='tu' seldi>
<td><input type='checkbox' name='we' selwo>
<td><input type='checkbox' name='th' seldo>
<td><input type='checkbox' name='fr' selvr>
<td><input type='checkbox' name='sa' selza>
</table>
</center><br>
 )=====";
