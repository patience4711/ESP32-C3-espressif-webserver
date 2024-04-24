const char PORTAL_STYLESHEET[] = R"=====(
#msect {display: flex; flex-basis: 100%; align-items: center; margin: 0px 0px; width:96vw;}
#msect:before, #msect:after {content: ""; flex-grow: 1; margin: 0px 0px;}
.table td{padding:.5em;text-align:left}
.table tbody>:nth-child(2n-1){background:#ddd}
.btn,h2{font-size: 20px}
.btn{width:300px;height:60px;background:#0066FF; outline: none;}
.btn2{width:260px;height:60px;background:#CC00FF; outline: none; font-size: 14px}
#together {display: block; margin;left:10px; width:336px; border: 3px solid #ff3467;} 

.c{text-align: center;} div,input{padding:5px;font-size:1em;} 
input{width:95%;} 
body{ text-align: center; font-family:verdana;} 
button{border:0;border-radius:0.3rem;background-color:#009933;color:#fff;line-height:2.4rem;font-size:30px;width:100%;}  
.q{float: right;width: 64px;text-align: right;} 
.l{background: }
.btn a{text-decoration: none}
  )=====";


const char PORTAL_HOME[] PROGMEM = R"=====( 
<!DOCTYPE html><html><html lang=\"en\"><head><meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
<title>hansiart wificonfig</title>
<link rel="stylesheet" type="text/css" href="/PORTAL?portalstyle">
</head>
<body><script>
function c(l){
  document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();
  }
</script>
<script>
if (window.location.hostname != "192.168.4.1") {
   window.location.href = 'http://192.168.4.1'; 
} 
</script>
<h2>HANSIART WIFI CONFIG</h2>
<h3>device mac adres : {ma}<h3>
<div hadden><a href="/PORTAL?wifiForm"><button class="btn">WIFI CONFIGURATION</button></a></div>
<br><br>
<div id='close' hidden><a href="/PORTAL?portalClose"><button class='btn'>CLOSE</button></a>
<br>
<br><br></div>
<strong>Currently not connected to a wifi network!</strong><br>
<br><br>
</body></html>
  )=====";
// ******************************************************************************************
//                                the page with the wifi form
// ******************************************************************************************
const char PORTAL_FORM[] PROGMEM = R"=====( 
<!DOCTYPE html><html><html lang=\"en\"><head><meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
<title>hansiart wificonfig</title>
<link rel="stylesheet" type="text/css" href="/PORTAL?portalstyle">
</head>
<body><center>
<script>
function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}
function showdiv() { document.getElementById('submitdiv').style.display = 'block';}
</script>

<div id='submitdiv' style='opacity:1.0; color:white; position:fixed; top:0; left:0; height:100%; width:100%; background-color:rgba(100,100,100,0.75);z-index:1000; display:none;'>
<br><br><br><h1>please wait while trying to connect..</h1></div>
<div id="msect"><center>

<div style='background:lightblue; width:340px;'><H3 style='text-align: center;'> HANSIART CONFIG PORTAL </h3></div>
<div id='together' style=' overflow-y:scroll; height:300px;'><h3>wifi networks</h3>
aplijst
</div>
<div id='together' style='background:lightyellow;'>
<form method='get' action='PORTAL'>
<table><tr><td style='width:120px;'>wifi netwerk<td><input id='s' name='ssid' length=32 placeholder='SSID' required></input></td></tr>
<tr><td>wifi passwd<td><input id='p' name='pass' length=64 type='text' placeholder='password' required></input></tr>
<tr><td>admin passwd<td><input  name='pswd' length='11' placeholder='max. 10 tekens' value='{pw}' pattern='.{4,10}' title='tussen 4 en 10 karakters'></input></tr> 
</table><br/>
<button type='submit' onclick='showdiv()' style='float:left'>save</button>
</form></div></div></div><br></body></html>
  )=====";

//<script type='text/javascript'>setTimeout(function(){ window.location.href='/CONCLUDE'; }, 3000 ); </script>
const char PORTAL_CONFIRM[] PROGMEM = R"=====( 
<!DOCTYPE html><html><html lang=\"en\"><head><meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
<title>hansiart wificonfig</title>
<link rel="stylesheet" type="text/css" href="/PORTAL?portalstyle">
</head>
<body style='font-size:16px;'><center>
<h1>  CONNECT TO WIFI </h1><br><br>
<strong>{text}</strong><br><br>
<strong>{ip}</strong><br><br>
click on OK<br><br><br>
<a href="/WIFIPORTAL"><button class='btn'>OK</button></a>
<br><br>
</body></html>
  )=====";  

const char PORTAL_LIST[] PROGMEM = R"=====( 
  <div><a href="#p" onclick="c(this)">{v}</a>&nbsp;<span class="q {i}">{r}dBm</span></div>
  )====="; 
