const char MENUPAGE[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8'>
<title>ESP32-SWITCH</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" type="image/x-icon" href="/favicon.ico" />
<link rel="stylesheet" type="text/css" href="/SHOWHTML?style">
<style> 
li a { width: 130px; } 
.list {width:70%; height:246px;}
@media only screen and (max-width: 600px) {
  li a {
    font-size:14px;
    padding: 14px 6px 10px 2px;
    width: 80px;
  }
.list {width:100%;}
}
</style>
<script type="text/javascript" src="SECURITY"></script>
<script> function cl() { window.location.href='/';} </script></head><body>
<div id='msect'>
  <ul>
    <li><a href='/SHOWHTML?about'>system info</a></li>
    <li id='fright'><span class='close' onclick='cl();'>&times;</span></ul>  
  </ul>
</div>
<br><br>
<div id='msect'><kop>ESP32 SWITCH MENU</kop></div><br><br><br>

<div id='msect'>
  <div style='width: 30vw;'>
  <ul class='list'>

  <li><a href='/SHOWHTML?timers_gen'>timers</a></li>
  <li><a href='/SHOWHTML?basis'>settings</a></li>
  <li><a href='/SHOWHTML?geo'>time config</a></li> 
  <li><a href='/SHOWHTML?mqtt'>mosquitto</a></li>
  </section>
  </ul><br>
  </div>
  <div style="width:20vw"></div>
  <div style='width: 30vw;'>
 
  <ul class='list'>
  <li><a href='/SHOWHTML?ipconf'>ip config</a></li>
  <li><a href='/RUNACTION?sw=fwupdate'>fw update</a></li>
  <li><a onclick="return confirm('are you sure?')" href='/RUNACTION?sw=reboot'>reboot</a></li>
  <li><a onclick="return confirm('reset wifi, are you sure?')" href='/RUNACTION?sw=startap'>res wifi</a></li>
  </ul><br>
  </div>
 
</div>
</body></html>
  )=====";
