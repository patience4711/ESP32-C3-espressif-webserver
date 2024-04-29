const char HOMEPAGE[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8'>
<title>ESP32 SWITCH</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link href="data:image/x-icon;base64,AAABAAEAEBAQAAEABAAoAQAAFgAAACgAAAAQAAAAIAAAAAEABAAAAAAAgAAAAAAAAAAAAAAAEAAAAAAAAAAu29YA0Q8YALTd3gBm4eMA0+blAIHh4wCa2tsAdd/gALUUnQC24+IAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAARERERERERERJkZmRGZkZlEkhIhIhIhKUSSFmFmFmEpRJIVUVUVUSlEkhV3EXcRGUSSZQAAB1YpRJJlczMHVilEiGWIOAdWKESIZYgIh1aIRIhliHiIiIhEiIiIVYhYiUSIZohmaCgpRIgiiCIoiClEiJmImZmJmUREREREREREQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" rel="icon" type="image/x-icon"/>
<link rel="stylesheet" type="text/css" href="/SHOWHTML?style">
<style>
#demoLPM:hover {
  cursor: pointer;
  background-color: yellow;
  font-size: 20px;
}
.btlabel {text-align:center; color: red;font-size:25px;}
/* toggle in label designing */
.toggle {
    position : relative ;
    display : inline-block;
    width : 72px;
    height : 130px;
    background-color: #e8f5f7;
    border-radius: 36px;
    border: 2px solid blue;
}
       
/* After slide changes */
.toggle:after {
    content: '';
    position: absolute;
    width: 70px;
    height: 70px;
    border-radius: 50%;
    background-color: black;
    top: 58px; 
    left: 1px;
    transition:  all 0.5s;
}

       
/* Checkbox checked effect */
.checkbox:checked + .toggle::after {
    transform: translate(0px, -58px);
    background:orange;
    box-shadow: 0px 0px 20px 10px red;
}
       
/* Checkbox checked toggle label bg color */
.checkbox:checked + .toggle {
    background-color: #e8f5f7;
}
       
/* Checkbox vanished */
.checkbox { 
    display : none;
}

.bt2 {
  font-size:16px;
  padding: 10px;
  margin:6px;
  text-align:center;
  border-radius:8px;
  color:white;
  background: #8742f5;
  box-shadow: 0 6px #302442;
  display: block;
  }

@media only screen and (max-width: 600px) {

}
</style>
</head>
<body onload='loadScript()'>
<div id='msect'>
<div id="menu">
<a href="SHOWHTML?menu" style="float:right">menu</a>
</div>
</div>
<div id='msect'>
<br>
<kop><span id="NAME">HAIPS</span></kop>
<br></div>
<div id='msect'>
  <div class='divstijl' id = 'maindiv' style='height:74vh; background: grey;'>
  <center> <br><span id='pwdby'style='font-size:11px; margin:auto; display:table; color:white;'>powered by Hansiart</span>
<br>

<table><tr><td class="btlabel">on</td></tr>
<tr><td><input type="checkbox" id="knop" class="checkbox" onclick="myFunction()" />
<label for="knop" class="toggle"></label></td>
<tr><td class="btlabel">off</td></tr></table></center>
<br>
<center><input type="range" id="duty" name="DT" min="0" max="8192" value="{DT}" class="slider" onchange="sliderUpdate()" list="tickmarks">
<datalist id="tickmarks"><option>0</option><option>5</option><option>10</option><option>15</option><option>20</option></datalist>
<br><br><br><button class="bt2" onClick='savePWM()' id='saveBt'>dim value : <span id="demoLPM"</span></button>
</div><br><br>
</div></body>

<script>

document.addEventListener("visibilitychange", function() {
    if (!document.hidden){
        console.log("Browser tab is visible")
        getData(); // update the page 
    } 
});

function savePWM() {
var pwm = document.getElementById('demoLPM').innerHTML;
if(!confirm("dimvalue = " + pwm + ", make this default?") ) {return};
var xhttp = new XMLHttpRequest();  
var url = "/parameters?pwmsave=" + pwm;
xhttp.open("GET", url, true);
xhttp.send();
}

var sldr = document.getElementById('duty');
var veld = document.getElementById('demoLPM');
// deze functie reageert op de sliderverandering

function sliderUpdate() {
  console.log("sliderUpdate");
  veld.style.color = "red";
  var outputLPM = document.getElementById("demoLPM");
  veld.innerHTML = sldr.value;
  sldr.oninput = function() { veld.innerHTML = this.value; }
  
  var xhttp = new XMLHttpRequest();  
  var sldrPos = document.getElementById("duty").value;
  console.log("sldrPos = " + sldrPos);
  var url = "getData?sldr=" + sldrPos;
  xhttp.open("GET", url, true);
  xhttp.send();
  // if the slider is set to 0, we cannot save that value as default, it
  // will be confusing. So we hide the button then
  if (sldrPos==0) {
    btn2.style.display="none"; 
  } else {
    btn2.style.display="block"; 
  }

}

function loadScript() {
// the first time we load the settings at once
getData();
}  
// this function reacts on the onclick of the toggleswitch
const btn = document.getElementById('knop');
const btn2 = document.getElementById('saveBt');
function myFunction() {
var xhttp = new XMLHttpRequest();  
if(btn.checked == true) {
      xhttp.open("GET", "/RUNACTION?sw=on", true);
      xhttp.send();
  } else {
      xhttp.open("GET", "/RUNACTION?sw=off", true);
      xhttp.send();
  }
}

// request is made when notified about a change
function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    
    if (this.readyState == 4 && this.status == 200) {
      var antwoord = this.responseText;  
      var obj = JSON.parse(antwoord);
      //var state = obj.state;
      document.getElementById("duty").value=obj.duty;
      document.getElementById("demoLPM").innerHTML=obj.duty;
      document.getElementById("NAME").innerHTML=obj.name;
      console.log("duty = " + duty);
      if (obj.duty == "0") {
        btn.checked=false;
        // if the dutyvalue null we hide the savebutton
        btn2.style.display="none";
      } else {
        btn.checked=true;
        btn2.style.display="block";
      }      
    }
  };
  xhttp.open("GET", "getData?knop", true);
  xhttp.send();
}

if (!!window.EventSource) {
 var source = new EventSource('/events');

 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);

 source.addEventListener('message', function(e) {
  console.log("message", e.data);
  if(e.data == "getdata") {
     getData();
  }

 }, false);

}

</script>
</html>
)=====";
