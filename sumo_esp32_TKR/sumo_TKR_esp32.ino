#include <WiFi.h>
#include <WebServer.h>


const char* ssid = "Robot_Control";
const char* password = "12345678";


const int IN1 = 2;  // Левый мотор
const int IN2 = 3;  
const int IN3 = 4;  // Правый мотор  
const int IN4 = 5;

// PWM настройки
const int freq = 5000;
const int resolution = 8;

WebServer server(80);


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">
  <style>
    body {
      font-family: Arial;
      text-align: center;
      margin: 0;
      padding: 20px;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    }
    h1 { color: white; }
    .controls {
      display: grid;
      grid-template-columns: repeat(3, 100px);
      gap: 10px;
      justify-content: center;
      margin: 20px auto;
    }
    button {
      width: 100px;
      height: 100px;
      font-size: 30px;
      border: none;
      border-radius: 15px;
      background: white;
      box-shadow: 0 4px 6px rgba(0,0,0,0.1);
      cursor: pointer;
      user-select: none;
      -webkit-user-select: none;
    }
    button:active {
      background: #4CAF50;
      transform: scale(0.95);
    }
    .empty { visibility: hidden; }
    .speed-control {
      margin: 20px;
      color: white;
    }
    input[type="range"] {
      width: 200px;
    }
    #speedValue {
      font-size: 24px;
      color: white;
    }
  </style>
</head>
<body>
  <h1>🤖 Управление роботом</h1>
  
  <div class="speed-control">
    <label>Скорость: <span id="speedValue">70</span>%</label><br>
    <input type="range" id="speed" min="30" max="100" value="70" oninput="updateSpeed(this.value)">
  </div>
  
  <div class="controls">
    <button class="empty"></button>
    <button ontouchstart="sendCommand('forward')" ontouchend="sendCommand('stop')" 
            onmousedown="sendCommand('forward')" onmouseup="sendCommand('stop')">⬆️</button>
    <button class="empty"></button>
    
    <button ontouchstart="sendCommand('left')" ontouchend="sendCommand('stop')"
            onmousedown="sendCommand('left')" onmouseup="sendCommand('stop')">⬅️</button>
    <button ontouchstart="sendCommand('stop')" ontouchend="sendCommand('stop')"
            onmousedown="sendCommand('stop')" onmouseup="sendCommand('stop')">⏹️</button>
    <button ontouchstart="sendCommand('right')" ontouchend="sendCommand('stop')"
            onmousedown="sendCommand('right')" onmouseup="sendCommand('stop')">➡️</button>
    
    <button class="empty"></button>
    <button ontouchstart="sendCommand('backward')" ontouchend="sendCommand('stop')"
            onmousedown="sendCommand('backward')" onmouseup="sendCommand('stop')">⬇️</button>
    <button class="empty"></button>
  </div>

  <script>
    let currentSpeed = 70;
    
    function sendCommand(cmd) {
      fetch('/control?cmd=' + cmd + '&speed=' + currentSpeed);
    }
    
    function updateSpeed(value) {
      currentSpeed = value;
      document.getElementById('speedValue').innerText = value;
    }
    
    // Предотвращаем зум на мобильных
    document.addEventListener('touchstart', function(e) {
      if (e.touches.length > 1) {
        e.preventDefault();
      }
    });
  </script>
</body>
</html>
)rawliteral";

int motorSpeed = 180; 

void setup() {
  Serial.begin(115200);
  

  ledcAttach(IN1, freq, resolution);
  ledcAttach(IN2, freq, resolution);
  ledcAttach(IN3, freq, resolution);
  ledcAttach(IN4, freq, resolution);
  

  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP адрес: ");
  Serial.println(IP);
  

  server.on("/", handleRoot);
  server.on("/control", handleControl);
  
  server.begin();
  Serial.println("Сервер запущен!");
  Serial.println("Подключитесь к WiFi: " + String(ssid));
  Serial.println("Пароль: " + String(password));
  Serial.println("Откройте в браузере: http://192.168.4.1");
  
  stopMotors();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleControl() {
  String cmd = server.arg("cmd");
  String speedStr = server.arg("speed");
  
  if (speedStr.length() > 0) {
    int speedPercent = speedStr.toInt();
    motorSpeed = map(speedPercent, 0, 100, 0, 255);
  }
  
  Serial.println("Команда: " + cmd + " Скорость: " + motorSpeed);
  
  if (cmd == "forward") {
    forward();
  } else if (cmd == "backward") {
    backward();
  } else if (cmd == "left") {
    turnLeft();
  } else if (cmd == "right") {
    turnRight();
  } else {
    stopMotors();
  }
  
  server.send(200, "text/plain", "OK");
}


void forward() {
  ledcWrite(IN1, motorSpeed);
  ledcWrite(IN2, 0);
  ledcWrite(IN3, motorSpeed);
  ledcWrite(IN4, 0);
}

void backward() {
  ledcWrite(IN1, 0);
  ledcWrite(IN2, motorSpeed);
  ledcWrite(IN3, 0);
  ledcWrite(IN4, motorSpeed);
}

void turnLeft() {
  ledcWrite(IN1, 0);
  ledcWrite(IN2, motorSpeed);
  ledcWrite(IN3, motorSpeed);
  ledcWrite(IN4, 0);
}

void turnRight() {
  ledcWrite(IN1, motorSpeed);
  ledcWrite(IN2, 0);
  ledcWrite(IN3, 0);
  ledcWrite(IN4, motorSpeed);
}

void stopMotors() {
  ledcWrite(IN1, 0);
  ledcWrite(IN2, 0);
  ledcWrite(IN3, 0);
  ledcWrite(IN4, 0);
}