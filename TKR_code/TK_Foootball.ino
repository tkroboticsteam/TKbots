#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "TKRobotics_FOOTBALL";
const char* password = "TKRT7282";

WebServer server(80);

// ===== МОТОРЫ =====
int M1A = 13, M1B = 14;   // Front Left
int M2A = 25, M2B = 26;   // Front Right
int M3A = 27, M3B = 32;   // Back Left
int M4A = 33, M4B = 23;   // Back Right

// ===== МОТОР =====
void motor(int a, int b, int dir) {
  if (dir == 1) {
    digitalWrite(a, HIGH);
    digitalWrite(b, LOW);
  } else if (dir == -1) {
    digitalWrite(a, LOW);
    digitalWrite(b, HIGH);
  } else {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
  }
}

// ===== STOP =====
void stopAll() {
  motor(M1A,M1B,0);
  motor(M2A,M2B,0);
  motor(M3A,M3B,0);
  motor(M4A,M4B,0);
}

// ===== MECANUM =====
void forward() {
  motor(M1A,M1B,1); motor(M2A,M2B,1);
  motor(M3A,M3B,1); motor(M4A,M4B,1);
}

void backward() {
  motor(M1A,M1B,-1); motor(M2A,M2B,-1);
  motor(M3A,M3B,-1); motor(M4A,M4B,-1);
}

void left() {
  motor(M1A,M1B,-1); motor(M2A,M2B,1);
  motor(M3A,M3B,1);  motor(M4A,M4B,-1);
}

void right() {
  motor(M1A,M1B,1);  motor(M2A,M2B,-1);
  motor(M3A,M3B,-1); motor(M4A,M4B,1);
}

void rotateLeft() {
  motor(M1A,M1B,-1); motor(M2A,M2B,1);
  motor(M3A,M3B,-1); motor(M4A,M4B,1);
}

void rotateRight() {
  motor(M1A,M1B,1);  motor(M2A,M2B,-1);
  motor(M3A,M3B,1);  motor(M4A,M4B,-1);
}

// ===== WEB UI (телефон + ноутбук) =====
void handleRoot() {
server.send(200,"text/html",R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body{background:#020617;color:white;font-family:Arial;text-align:center}
.grid{display:grid;grid-template-columns:repeat(3,120px);gap:15px;justify-content:center;margin-top:25px}
button{width:120px;height:120px;font-size:24px;border-radius:25px;border:none;background:#22c55e;font-weight:bold}
button:active{background:#16a34a}
.rotate{background:#38bdf8}
.stop{background:#ef4444}
</style>
</head>

<body>
<h1>TKRobotics Football</h1>
<p>dream team!<br>Laptop: W-GO A-BACK S-mecL D-mecR | Q-razL E-razR | SPACE-stop</p>

<div class="grid">
<div></div>
<button ontouchstart="cmd('f')" ontouchend="cmd('s')">mecR</button>
<div></div>

<button ontouchstart="cmd('l')" ontouchend="cmd('s')">Back</button>
<button class="stop" onclick="cmd('s')">STOP</button>
<button ontouchstart="cmd('r')" ontouchend="cmd('s')">GO</button>

<button class="rotate" ontouchstart="cmd('rl')" ontouchend="cmd('s')">razL</button>
<button ontouchstart="cmd('b')" ontouchend="cmd('s')">mecL</button>
<button class="rotate" ontouchstart="cmd('rr')" ontouchend="cmd('s')">razR</button>
</div>

<script>
let pressed = {};

function cmd(c){
  fetch('/'+c);
}

document.addEventListener('keydown', e => {
  if(pressed[e.code]) return;
  pressed[e.code] = true;

  switch(e.code){
    case 'KeyD': cmd('f'); break;
    case 'KeyS': cmd('b'); break;
    case 'KeyA': cmd('l'); break;
    case 'KeyW': cmd('r'); break;
    case 'KeyQ': cmd('rl'); break;
    case 'KeyE': cmd('rr'); break;
    case 'Space': cmd('s'); break;
  }
});

document.addEventListener('keyup', e => {
  delete pressed[e.code];
  cmd('s');
});
</script>

</body>
</html>
)rawliteral");
}

// ===== SETUP =====
void setup() {
  pinMode(M1A,OUTPUT); pinMode(M1B,OUTPUT);
  pinMode(M2A,OUTPUT); pinMode(M2B,OUTPUT);
  pinMode(M3A,OUTPUT); pinMode(M3B,OUTPUT);
  pinMode(M4A,OUTPUT); pinMode(M4B,OUTPUT);

  stopAll();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid,password);

  server.on("/",handleRoot);
  server.on("/f",[]{forward();server.send(200);});
  server.on("/b",[]{backward();server.send(200);});
  server.on("/l",[]{left();server.send(200);});
  server.on("/r",[]{right();server.send(200);});
  server.on("/rl",[]{rotateLeft();server.send(200);});
  server.on("/rr",[]{rotateRight();server.send(200);});
  server.on("/s",[]{stopAll();server.send(200);});

  server.begin();
}

// ===== LOOP =====
void loop() {
  server.handleClient();
}
