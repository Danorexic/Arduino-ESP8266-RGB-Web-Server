const uint16_t gamma_adjust[] {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
  
  const int colorNum [26] [3] = {
    { 0, 0, 255  },
    { 0, 128, 0 },
    { 255, 0, 0 },
    { 210, 105, 30},
    { 128, 0, 128},
    { 0, 0, 128},
    { 230, 230, 250},
    { 255, 255, 0},
    { 255, 0, 255},
    { 128, 0, 0},
    { 0, 255, 255},
    { 0, 128, 128},
    { 255, 192, 203},
    { 238, 130, 238},
    { 255, 105, 180},
    { 75, 0, 130},
    { 255, 127, 80},
    { 240, 230, 140},
    { 0, 255, 0},
    { 128, 128, 0},
    { 221, 160, 221},
    { 65, 105, 225},
    { 192, 192, 192},
    { 135, 206, 235},
    { 165, 42, 42},
    { 0, 255, 255},
  
    };
  const String colorType [26] = {
    { "Blue" },
    { "Green" },
    { "Red" },
    {"Chocolate"},
    {"Purple"},
    {"Navy"},
    {"Lavender"},
    {"Yellow"},
    {"Magenta"},
    {"Maroon"},
    {"Cyan"},
    {"Teal"},
    {"Pink"},
    {"Violet"},
    {"Hot Pink"},
    {"Indigo"},
    {"Coral"},
    {"Khaki"},
    {"Lime"},
    {"Olive"},
    {"Plum"},
    {"Royal Blue"},
    {"Silver"},
    {"Sky Blue"},
    {"Brown"},
    {"Aqua"},

    };
    
const char page_contents[] PROGMEM = R"=====(
 <!doctype html>
 <html>
   <head>
     <title>Home screen</title>
     <meta name="viewport" content="width=device-width, initial-scale=1.0">
     <style type="text/css">
html, body{
    font-family: Arial;
    margin: 0;

    padding: 0;

    min-width: 100%;
    width: 100%;
    max-width: 100%;

    min-height: 100%;
    height: 100%;
    max-height: 100%;
}
#container {
    width:100%;
    height:100%;
    position:fixed;
}
#middle {height:100%;}
/*
button {
  width:100%;
  height:12%;
  -webkit-appearance:none;
  border-radius: 0;
  }
*/
button.off{
  width:100%;
  height:12%;
  -webkit-appearance:none;
  border-radius: 0;
  background:red;
  text-shadow: none;
  border: none;
  box-shadow: none;
  
}
button.on{
  width:100%;
  height:12%;
  -webkit-appearance:none;
  border-radius: 0;
  background:green;
  text-shadow: none;
  border: none;
  box-shadow: none;
}

button {
  width:100%;
  height:12%;
  -webkit-appearance: none;
	-moz-box-shadow:inset 0px 1px 0px 0px #efdcfb;
	-webkit-box-shadow:inset 0px 1px 0px 0px #efdcfb;
	box-shadow:inset 0px 1px 0px 0px #efdcfb;
	background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #dfbdfa), color-stop(1, #bc80ea));
	background:-moz-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-webkit-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-o-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-ms-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:linear-gradient(to bottom, #cfb1e8 5%, #bc80ea 100%);
	filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#cfb1e8', endColorstr='#bc80ea',GradientType=0);
	background-color:#cfb1e8;
	-moz-border-radius:0px;
	-webkit-border-radius:0px;
	border-radius:0px;
	border:1px solid #c584f3;
	display:inline-block;
	cursor:pointer;
	color:#ffffff;
	font-family:Arial;
	font-size:20px;
	font-weight:bold;
	padding:6px 24px;
	text-decoration:none;
	text-shadow:0px 1px 0px #9752cc;
}

</style>
   </head>
 <body>
 <div id="container">
   <div id="middle">
 <p align='center'><id='brightness_page'><strong style='font-size:200%'>Wifi Lightbox</strong></p>
 
 <button type='button' class='off' id='offBtn' onclick="location.href='/switch_off'">Off</button>
 
 <button type='button' class='on' id='onBtn'  onclick="location.href='/switch_on'">On</button>
 
<button type='button' id='colourBtn' onclick="location.href='/set_colour'">Set Colour</button>

<button type='button'id='brightnessBtn' onclick="location.href='/set_brightness'">Set Brightness</button>
 
<button type='button'id='modeBtn' onclick="location.href='/select_mode'">Select Mode</button>

<button type='button'id='modeBtn' onclick="location.href='/select_palette'">Select Palette</button>
</div>
</div>
</body>

</html>)=====";


const char colour_picker[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
        <script src="jscolor.js"></script>
        <title>Color Selector</title>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <style type="text/css">
        /*
    button {
      width:100%;
      height:12%;
      -webkit-appearance:none;
      border-radius: 0;
      }
*/
button {
  width:100%;
  height:12%;
  -webkit-appearance: none;
	-moz-box-shadow:inset 0px 1px 0px 0px #efdcfb;
	-webkit-box-shadow:inset 0px 1px 0px 0px #efdcfb;
	box-shadow:inset 0px 1px 0px 0px #efdcfb;
	background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #dfbdfa), color-stop(1, #bc80ea));
	background:-moz-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-webkit-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-o-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-ms-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:linear-gradient(to bottom, #cfb1e8 5%, #bc80ea 100%);
	filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#cfb1e8', endColorstr='#bc80ea',GradientType=0);
	background-color:#cfb1e8;
	-moz-border-radius:0px;
	-webkit-border-radius:0px;
	border-radius:0px;
	border:1px solid #c584f3;
	display:inline-block;
	cursor:pointer;
	color:#ffffff;
	font-family:Arial;
	font-size:20px;
	font-weight:bold;
	padding:6px 24px;
	text-decoration:none;
	text-shadow:0px 1px 0px #9752cc;
}

    #container {
      width:100%;
      height:100%;
      position:fixed;
      }
    #middle {height:100%;}
    html, body{
        font-family: Arial;
        margin: 0;
    
        padding: 0;
    
        min-width: 100%;
        width: 100%;
        max-width: 100%;
    
        min-height: 100%;
        height: 100%;
        max-height: 100%;
    }
    </style>
  </head>
<body>
    <div id="container">
        <div id="middle">

      
    
<p align='center'><id='colour_page'><BR><strong style='font-size:200%'>Select a color</strong></p><BR><BR>


<p id="place_holder_1"></p>


<button type='button' id='homeBtn'style='font-size:19px;font-weight:bold;font-size:16px'onclick="location.href='/'">Main Menu</button>


<script type='text/javascript'>
var hash_value=unescape((''+self.location.search).substring(1));
if(hash_value==""){hash_value="#0000ff"};

function loadForm()
{
  
  document.getElementById("place_holder_1").innerHTML ="<form id='colourForm' action='/set_colour_hash' method='GET'><p align='left'><input class='jscolor' name='chosen_colour' onchange='clickColor()' value=" + hash_value + " style='width:15%;height:50px' readonly></button></form><BR><BR>"
  
}


function clickColor()
{
  document.getElementById('colourForm').submit();
}

loadForm();


</script>
</div>
</div>
</body>
</html>)=====";


const char bright_set[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <title>Brightness Selector</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style type="text/css">
input[type=range]::-webkit-slider-thumb {
  -webkit-appearance: none;
  border: 2px solid #000000;
  height: 55px;
  width: 55px;
  border-radius: 1px;
  background: #ffffff;
}
input[type=range]::-webkit-slider-runnable-track {
  height: 40px;
}
/*
button {
  width:100%;
  height:12%;
  -webkit-appearance:none;
  border-radius: 0;
  }
*/
button {
  width:100%;
  height:12%;
  -webkit-appearance: none;
	-moz-box-shadow:inset 0px 1px 0px 0px #efdcfb;
	-webkit-box-shadow:inset 0px 1px 0px 0px #efdcfb;
	box-shadow:inset 0px 1px 0px 0px #efdcfb;
	background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #dfbdfa), color-stop(1, #bc80ea));
	background:-moz-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-webkit-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-o-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-ms-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:linear-gradient(to bottom, #cfb1e8 5%, #bc80ea 100%);
	filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#cfb1e8', endColorstr='#bc80ea',GradientType=0);
	background-color:#cfb1e8;
	-moz-border-radius:0px;
	-webkit-border-radius:0px;
	border-radius:0px;
	border:1px solid #c584f3;
	display:inline-block;
	cursor:pointer;
	color:#ffffff;
	font-family:Arial;
	font-size:20px;
	font-weight:bold;
	padding:6px 24px;
	text-decoration:none;
	text-shadow:0px 1px 0px #9752cc;
}

html, body{
    font-family: Arial;
    margin: 0;

    padding: 0;

    min-width: 100%;
    width: 100%;
    max-width: 100%;

    min-height: 100%;
    height: 100%;
    max-height: 100%;
}
#container {
      width:100%;
      height:100%;
      position:fixed;
      }
#middle {height:100%;}
</style>
  </head>

<body>
    <div id="container">
        <div id="middle">

<p align='center'><id='colour_page'><BR><strong style='font-size:200%'>Select the brightness</strong></p><BR><BR>


<p id="place_holder_1"></p>

<button type='button' id='homeBtn'style='font-size:19px;font-weight:bold;font-size:16px'onclick="location.href='/'">Main Menu</button>




<script type='text/javascript'>
  
var brightness_val=unescape((''+self.location.search).substring(1));
if(brightness_val==""){brightness_val=254};

function loadForm()
{
  document.getElementById("place_holder_1").innerHTML ="<form id='brightForm' action='/set_bright_val' method='get'><p align='center'><input type='range' name='chosen_brightness' onchange='clickBright()' min='1' max='255' value='" + brightness_val + "' style='width:75%;height:40%'></p></form><BR><BR>"
}



function clickBright()
{
  document.getElementById('brightForm').submit();
}

loadForm();


</script>
</div>
</div>
</body>
</html>)=====";


const char mode_page[] PROGMEM = R"=====(
<!doctype html>
<html>
  <head>
     <title>Mode Selection</title>
     <meta name="viewport" content="width=device-width, initial-scale=1.0">
     <style type="text/css">
html, body{
    font-family: Arial;
    margin: 0;

    padding: 0;

    min-width: 100%;
    width: 100%;
    max-width: 100%;

    min-height: 100%;
    height: 100%;
    max-height: 100%;
}
#container {
    width:100%;
    height:100%;
    position:fixed;
}
#middle {height:100%;}
/*
button {
  width:100%;
  height:12%;
  -webkit-appearance:none;
  border-radius: 0;
  }
*/
button {
  width:100%;
  height:12%;
  -webkit-appearance: none;
	-moz-box-shadow:inset 0px 1px 0px 0px #efdcfb;
	-webkit-box-shadow:inset 0px 1px 0px 0px #efdcfb;
	box-shadow:inset 0px 1px 0px 0px #efdcfb;
	background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #dfbdfa), color-stop(1, #bc80ea));
	background:-moz-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-webkit-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-o-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-ms-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:linear-gradient(to bottom, #cfb1e8 5%, #bc80ea 100%);
	filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#cfb1e8', endColorstr='#bc80ea',GradientType=0);
	background-color:#cfb1e8;
	-moz-border-radius:0px;
	-webkit-border-radius:0px;
	border-radius:0px;
	border:1px solid #c584f3;
	display:inline-block;
	cursor:pointer;
	color:#ffffff;
	font-family:Arial;
	font-size:20px;
	font-weight:bold;
	padding:6px 24px;
	text-decoration:none;
	text-shadow:0px 1px 0px #9752cc;
}

</style>
   </head>
<body>
<div id="container">
  <div id="middle">
  <p align='center'><id='brightness_page'><strong style='font-size:200%'>Select Mode</strong></p>

<button type='button' id='mode1Btn' onclick="location.href='/set_mode1'">Fixed Colour</button>

<button type='button'id='mode2Btn' onclick="location.href='/set_mode2'">Colour Fade</button>

<button type='button'id='mode3Btn' onclick="location.href='/set_mode3'">Rainbow</button>

<button type='button'id='mode3Btn' onclick="location.href='/set_mode4'">Random</button>

<button type='button' id='homeBtn'onclick="location.href='/'">Main Menu</button>
</div>
</div>
</body>

</html>)=====";

const char palette_page[] PROGMEM = R"=====(
<!doctype html>
<html>
  <head>
     <title>Palette Selection</title>
     <meta name="viewport" content="width=device-width, initial-scale=1.0">
     <style type="text/css">
html, body{
    font-family: Arial;
    margin: 0;

    padding: 0;

    min-width: 100%;
    width: 100%;
    max-width: 100%;

    min-height: 100%;
    height: 100%;
    max-height: 100%;
}
#container {
    width:100%;
    height:100%;
    position:fixed;
}
#middle {height:100%;}
/*
button {
  width:100%;
  height:12%;
  -webkit-appearance:none;
  border-radius: 0;
  }
  */
button {
  width:100%;
  height:12%;
  -webkit-appearance: none;
	-moz-box-shadow:inset 0px 1px 0px 0px #efdcfb;
	-webkit-box-shadow:inset 0px 1px 0px 0px #efdcfb;
	box-shadow:inset 0px 1px 0px 0px #efdcfb;
	background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #dfbdfa), color-stop(1, #bc80ea));
	background:-moz-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-webkit-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-o-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:-ms-linear-gradient(top, #cfb1e8 5%, #bc80ea 100%);
	background:linear-gradient(to bottom, #cfb1e8 5%, #bc80ea 100%);
	filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#cfb1e8', endColorstr='#bc80ea',GradientType=0);
	background-color:#cfb1e8;
	-moz-border-radius:0px;
	-webkit-border-radius:0px;
	border-radius:0px;
	border:1px solid #c584f3;
	display:inline-block;
	cursor:pointer;
	color:#ffffff;
	font-family:Arial;
	font-size:20px;
	font-weight:bold;
	padding:6px 24px;
	text-decoration:none;
	text-shadow:0px 1px 0px #9752cc;
}

</style>
   </head>
<body>
<div id="container">
  <div id="middle">  
<p align='center'><id='brightness_page'><strong style='font-size:200%'>Select Palettes</strong></p>

<button type='button' id='blendBtn' onclick="location.href='/set_mode5'">Blending Mode</button>

<button type='button'id='noblendBtn' onclick="location.href='/set_mode6'">No Blending Mode</button>

<button type='button'id='cloudBtn' onclick="location.href='/set_cloud'">Cloud Colors</button>

<button type='button'id='OceanBtn' onclick="location.href='/set_ocean'">Ocean Colors</button>

<button type='button'id='PartyBtn' onclick="location.href='/set_party'">Party Colors</button>

<button type='button'id='HeatBtn' onclick="location.href='/set_heat'">Heat Colors</button>

<button type='button' id='homeBtn'onclick="location.href='/'">Main Menu</button>
</div>
</div>
</body></html>)=====";




