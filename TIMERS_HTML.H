const char TIMERCONFIG_START[] PROGMEM = R"=====(

<body>

<div id='msect'>
<div id='bo'></div>
<div id="menu">
<a href="#" class='close' onclick='cl();'>&times;</a>
<a href="#" id="sub" onclick='submitFunction("/SHOWHTML?back")'>save</a></div>
</div>
</div>

<div id='msect'>
  <center><kop>TIMER SETTINGS </kop></center><br>
</div>

<div id='msect'>
<center><div class='divstijl'>

<form id='formulier' method='get' action='parameters' oninput='showSubmit()'>
  <center><irame name>
  </div>
<div><br></form>
<div id='msect'>
<div id="menu">
<b>timers</b>
<a href='/SHOWHTML?timer_ind=0' > 0</a>
<a href='/SHOWHTML?timer_ind=1' > 1</a>
<a href='/SHOWHTML?timer_ind=2' > 2</a>
<a href='/SHOWHTML?timer_ind=3' > 3</a>
</div>
</div>
</body></html>
 )=====";
                                                                        
 const char TIMER_GENERAL[] PROGMEM = R"=====(
<center><table><tr><td style='width:180px;color:#e207fa;'><h1> TIMER {nr} </h1>
<td style='width:40px;'>active?<td><input type='checkbox' name='ta' tActive>
</table>
<table>
<tr><td>on:<td><input class='inp4' type='time' name='inw' value='{inx}' title='hh:mm'>
<td><select name='zonattaan' class='sb1'>
<option value='0' zonattaan_1>absolute</option>
<option value='3' zonattaan_4>before sunrise</option>
<option value='4' zonattaan_5>after sunrise</option>
<option value='1' zonattaan_2>before sunset</option>
<option value='2' zonattaan_3>after sunset</option>

</select></td></tr>
<tr><td>off:<td><input class='inp4' name='uitw' type='time' value='{uitx}' title='hh:mm' >
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
