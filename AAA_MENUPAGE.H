const char MENUPAGE[] PROGMEM = R"=====(
<body>
<style>
a:link, a:visited { width: 98px;}
</style>

<div id='msect'>
<div id="menu">
<a href='/SHOWHTML?about'>system info</a>
<a href="/" class='close' onclick='cl();'>&times;</a>
</div>
</div>
<br><br>
<div id='msect'><kop>ESP32 SWITCH MENU</kop></div><br><br><br>

<div id='msect'>

<table style="border: 1px solid black'"><tr>
<td style="width:100px"><a href='/SHOWHTML?timers_gen'>timers</a></td><td style="width:60px"></td><td style="width:100px;">
<a href='/SHOWHTML?ipconf'>ip config</a></<td></tr>

<tr><td><a href='/SHOWHTML?basis'>settings</a></td><td></td>
<td><a href='/RUNACTION?sw=fwupdate'>fw update</a></td></tr>

<tr><td><a href='/SHOWHTML?geo'>time config</a></td><td></td>
<td><a onclick="return confirm('are you sure?')" href='/RUNACTION?sw=reboot'>reboot</a></td></tr>

<tr><td><a href='/SHOWHTML?mqtt'>mosquitto</a></td><td></td>
<td><a onclick="return confirm('reset wifi, are you sure?')" href='/RUNACTION?sw=startap'>reset wifi</a></td><tr>

</table>
  
 
</div>
</body></html>
  )=====";
