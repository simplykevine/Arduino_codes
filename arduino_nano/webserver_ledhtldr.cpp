#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN D3      // Define the DHT11 sensor pin
#define DHTTYPE DHT11  // Define the sensor type (DHT11)

// Create a DHT object
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Kevine_UMUTONI";
const char* password = "12345678";

int green = D4;  // LED1
int yellow = D5; // LED2
int red = D6;    // LED3
int buzzer = A0; // Buzzer pin

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);
bool LEDStatus1 = LOW;
bool LEDStatus2 = LOW;
bool LEDStatus3 = LOW;
bool buzzerStatus = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(buzzer, OUTPUT);
  dht.begin();  // Initialize the DHT sensor

  // Set up Wi-Fi Access Point
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(1000);

  // Define routes
  server.on("/", handle_OnConnect);
  server.on("/toggleLED", handle_toggleLED);
  server.on("/toggleBuzzer", handle_toggleBuzzer);
  server.on("/dht", handle_DHT);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  
  // Control LEDs
  digitalWrite(green, LEDStatus1);
  digitalWrite(yellow, LEDStatus2);
  digitalWrite(red, LEDStatus3);
  
  // Control Buzzer
  digitalWrite(buzzer, buzzerStatus);
}

String getHTML() {
  String htmlCode = "<!DOCTYPE html>\n";
  htmlCode += "<html>\n";
  htmlCode += "<head>\n";
  htmlCode += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n";
  htmlCode += "<title>ESP8266 Web Server</title>\n";
  htmlCode += "<style>\n";
  htmlCode += "body { font-family: Arial; text-align: center; }\n";
  htmlCode += ".box { width: 300px; margin: 20px auto; padding: 20px; border-radius: 8px; box-shadow: 0 0 10px rgba(0,0,0,0.1); position: relative; cursor: pointer; }\n";
  htmlCode += ".box h2 { margin: 0; }\n";
  htmlCode += ".options { display: none; margin-top: 10px; }\n";
  htmlCode += ".options button { margin: 5px; padding: 10px; font-size: 16px; border: none; cursor: pointer; }\n";
  htmlCode += ".value { font-size: 24px; }\n";
  htmlCode += "</style>\n";
  htmlCode += "<script>\n";
  htmlCode += "function toggleOptions(boxId) { \n";
  htmlCode += "  var options = document.getElementById(boxId);\n";
  htmlCode += "  if (options.style.display === 'none') {\n";
  htmlCode += "    options.style.display = 'block';\n";
  htmlCode += "  } else {\n";
  htmlCode += "    options.style.display = 'none';\n";
  htmlCode += "  }\n";
  htmlCode += "}\n";
  htmlCode += "function toggleLED(action, led) { \n";
  htmlCode += "  var xhttp = new XMLHttpRequest();\n";
  htmlCode += "  xhttp.onreadystatechange = function() { \n";
  htmlCode += "    if (this.readyState == 4 && this.status == 200) { \n";
  htmlCode += "      document.getElementById(\"ledStatus\" + led).innerText = this.responseText;\n";
  htmlCode += "    }\n";
  htmlCode += "  };\n";
  htmlCode += "  xhttp.open(\"GET\", \"/toggleLED?action=\" + action + \"&led=\" + led, true);\n";
  htmlCode += "  xhttp.send();\n";
  htmlCode += "}\n";
  htmlCode += "function toggleBuzzer(action) { \n";
  htmlCode += "  var xhttp = new XMLHttpRequest();\n";
  htmlCode += "  xhttp.onreadystatechange = function() { \n";
  htmlCode += "    if (this.readyState == 4 && this.status == 200) { \n";
  htmlCode += "      document.getElementById(\"buzzerStatus\").innerText = this.responseText;\n";
  htmlCode += "    }\n";
  htmlCode += "  };\n";
  htmlCode += "  xhttp.open(\"GET\", \"/toggleBuzzer?action=\" + action, true);\n";
  htmlCode += "  xhttp.send();\n";
  htmlCode += "}\n";
  htmlCode += "setInterval(function() { refreshDHT('temperature'); refreshDHT('humidity'); }, 5000); // Refresh DHT values every 5 seconds\n";
  htmlCode += "</script>\n";
  htmlCode += "</head>\n";
  htmlCode += "<body>\n";
  htmlCode += "<h1>ESP8266 Web Server</h1>\n";
  htmlCode += "<h3>Simple demo using Access Point (AP) Mode</h3>\n";

  // LED1 Control Box
  htmlCode += "<div class=\"box\" onclick=\"toggleOptions('ledOptions1')\">\n";
  htmlCode += "<h2>LED 1 Control</h2>\n";
  htmlCode += "<div id=\"ledOptions1\" class=\"options\">\n";
  htmlCode += "<button onclick=\"toggleLED('on', 1)\">Switch On LED 1</button>\n";
  htmlCode += "<button onclick=\"toggleLED('off', 1)\">Switch Off LED 1</button>\n";
  htmlCode += "</div>\n";
  htmlCode += "<div id=\"ledStatus1\" class=\"value\">" + String(LEDStatus1 ? "LED On" : "LED Off") + "</div>\n";
  htmlCode += "</div>\n";

  // LED2 Control Box
  htmlCode += "<div class=\"box\" onclick=\"toggleOptions('ledOptions2')\">\n";
  htmlCode += "<h2>LED 2 Control</h2>\n";
  htmlCode += "<div id=\"ledOptions2\" class=\"options\">\n";
  htmlCode += "<button onclick=\"toggleLED('on', 2)\">Switch On LED 2</button>\n";
  htmlCode += "<button onclick=\"toggleLED('off', 2)\">Switch Off LED 2</button>\n";
  htmlCode += "</div>\n";
  htmlCode += "<div id=\"ledStatus2\" class=\"value\">" + String(LEDStatus2 ? "LED On" : "LED Off") + "</div>\n";
  htmlCode += "</div>\n";

  // LED3 Control Box
  htmlCode += "<div class=\"box\" onclick=\"toggleOptions('ledOptions3')\">\n";
  htmlCode += "<h2>LED 3 Control</h2>\n";
  htmlCode += "<div id=\"ledOptions3\" class=\"options\">\n";
  htmlCode += "<button onclick=\"toggleLED('on', 3)\">Switch On LED 3</button>\n";
  htmlCode += "<button onclick=\"toggleLED('off', 3)\">Switch Off LED 3</button>\n";
  htmlCode += "</div>\n";
  htmlCode += "<div id=\"ledStatus3\" class=\"value\">" + String(LEDStatus3 ? "LED On" : "LED Off") + "</div>\n";
  htmlCode += "</div>\n";

  // Buzzer Control Box
  htmlCode += "<div class=\"box\" onclick=\"toggleOptions('buzzerOptions')\">\n";
  htmlCode += "<h2>Buzzer Control</h2>\n";
  htmlCode += "<div id=\"buzzerOptions\" class=\"options\">\n";
  htmlCode += "<button onclick=\"toggleBuzzer('on')\">Switch On Buzzer</button>\n";
  htmlCode += "<button onclick=\"toggleBuzzer('off')\">Switch Off Buzzer</button>\n";
  htmlCode += "</div>\n";
  htmlCode += "<div id=\"buzzerStatus\" class=\"value\">" + String(buzzerStatus ? "Buzzer On" : "Buzzer Off") + "</div>\n";
  htmlCode += "</div>\n";

  // Temperature and Humidity Box
  htmlCode += "<div class=\"box\" onclick=\"refreshDHT('temperature')\">\n";
  htmlCode += "<h2>Temperature</h2>\n";
  htmlCode += "<div id=\"temperature\" class=\"value\"></div>\n";
  htmlCode += "</div>\n";

  htmlCode += "<div class=\"box\" onclick=\"refreshDHT('humidity')\">\n";
  htmlCode += "<h2>Humidity</h2>\n";
  htmlCode += "<div id=\"humidity\" class=\"value\"></div>\n";
  htmlCode += "</div>\n";

  htmlCode += "</body>\n";
  htmlCode += "</html>\n";

  return htmlCode;
}

void handle_OnConnect() {
  server.send(200, "text/html", getHTML());
}

void handle_toggleLED() {
  String action = server.arg("action");
  int led = server.arg("led").toInt();

  if (led == 1) {
    LEDStatus1 = (action == "on") ? HIGH : LOW;
  } else if (led == 2) {
    LEDStatus2 = (action == "on") ? HIGH : LOW;
  } else if (led == 3) {
    LEDStatus3 = (action == "on") ? HIGH : LOW;
  }
  server.send(200, "text/plain", (action == "on") ? "LED On" : "LED Off");
}

void handle_toggleBuzzer() {
  String action = server.arg("action");

  if (action == "on") {
    buzzerStatus = HIGH;
  } else if (action == "off") {
    buzzerStatus = LOW;
  }
  server.send(200, "text/plain", buzzerStatus ? "Buzzer On" : "Buzzer Off");
}

void handle_DHT() {
  String action = server.arg("action");
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if (action == "temperature") {
    server.send(200, "application/json", "{\"temperature\":" + String(temperature) + "}");
  } else if (action == "humidity") {
    server.send(200, "application/json", "{\"humidity\":" + String(humidity) + "}");
  } else {
    server.send(400, "text/plain", "Invalid action");
  }
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
