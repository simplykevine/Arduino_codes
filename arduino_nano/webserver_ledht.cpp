#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN D3      // Define the DHT11 sensor pin
#define DHTTYPE DHT11  // Define the sensor type (DHT11)

// Create a DHT object
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Kevine_UMUTONI";
const char* password = "12345678";

int green = D4;  // LED 1 (Green)
int red = D5;    // LED 2 (Red)
int blue = D6;   // LED 3 (Blue)

int ldrPin = A0;  // Analog pin A0 for LDR
int ldrValue = 0;

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

bool LEDStatus1 = LOW;  // State for LED 1 (Green)
bool LEDStatus2 = LOW;  // State for LED 2 (Red)
bool LEDStatus3 = LOW;  // State for LED 3 (Blue)

void setup() {
  Serial.begin(115200);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  dht.begin();  // Initialize the DHT sensor

  // Set up Wi-Fi Access Point
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(1000);

  // Define routes
  server.on("/", handle_OnConnect);
  server.on("/toggle", handle_toggleLED);
  server.on("/ldr", handle_LDR);
  server.on("/dht", handle_DHT);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  digitalWrite(green, LEDStatus1);
  digitalWrite(red, LEDStatus2);
  digitalWrite(blue, LEDStatus3);
}

String getHTML() {
  ldrValue = analogRead(ldrPin);  // Read LDR value

  String htmlCode = "<!DOCTYPE html>\n";
  htmlCode += "<html>\n";
  htmlCode += "<head>\n";
  htmlCode += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n";
  htmlCode += "<title>ESP8266 Web Server @Kevine</title>\n";
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
  htmlCode += "  xhttp.open(\"GET\", \"/toggle?action=\" + action + \"&led=\" + led, true);\n";
  htmlCode += "  xhttp.send();\n";
  htmlCode += "}\n";
  htmlCode += "function refreshLDR() { \n";
  htmlCode += "  var xhttp = new XMLHttpRequest();\n";
  htmlCode += "  xhttp.onreadystatechange = function() { \n";
  htmlCode += "    if (this.readyState == 4 && this.status == 200) { \n";
  htmlCode += "      document.getElementById(\"ldrValue\").innerText = this.responseText;\n";
  htmlCode += "    }\n";
  htmlCode += "  };\n";
  htmlCode += "  xhttp.open(\"GET\", \"/ldr\", true);\n";
  htmlCode += "  xhttp.send();\n";
  htmlCode += "}\n";
  htmlCode += "function refreshDHT(action) { \n";
  htmlCode += "  var xhttp = new XMLHttpRequest();\n";
  htmlCode += "  xhttp.onreadystatechange = function() { \n";
  htmlCode += "    if (this.readyState == 4 && this.status == 200) { \n";
  htmlCode += "      var data = JSON.parse(this.responseText);\n";
  htmlCode += "      if (action === 'temperature') {\n";
  htmlCode += "        document.getElementById(\"temperature\").innerText = data.temperature;\n";
  htmlCode += "      } else if (action === 'humidity') {\n";
  htmlCode += "        document.getElementById(\"humidity\").innerText = data.humidity;\n";
  htmlCode += "      }\n";
  htmlCode += "    }\n";
  htmlCode += "  };\n";
  htmlCode += "  xhttp.open(\"GET\", \"/dht?action=\" + action, true);\n";
  htmlCode += "  xhttp.send();\n";
  htmlCode += "}\n";
  htmlCode += "setInterval(refreshLDR, 2000); // Refresh LDR value every 2 seconds\n";
  htmlCode += "setInterval(function() { refreshDHT('temperature'); refreshDHT('humidity'); }, 5000); // Refresh DHT values every 5 seconds\n";
  htmlCode += "</script>\n";
  htmlCode += "</head>\n";
  htmlCode += "<body>\n";
  htmlCode += "<h1>ESP8266 Web Server</h1>\n";
  htmlCode += "<h3>Simple demo using Access Point (AP) Mode</h3>\n";

  // LED 1 (Green) Box
  htmlCode += "<div class=\"box\" onclick=\"toggleOptions('ledOptions1')\">\n";
  htmlCode += "<h2>Monitor LED 1 (room)</h2>\n";
  htmlCode += "<div id=\"ledOptions1\" class=\"options\">\n";
  htmlCode += "<button onclick=\"toggleLED('on', 1)\">Switch On the Light</button>\n";
  htmlCode += "<button onclick=\"toggleLED('off', 1)\">Switch Off the Light</button>\n";
  htmlCode += "</div>\n";
  htmlCode += "<div id=\"ledStatus1\" class=\"value\">" + String(LEDStatus1 ? "LED On" : "LED Off") + "</div>\n";
  htmlCode += "</div>\n";

  // LED 2 (Red) Box
  htmlCode += "<div class=\"box\" onclick=\"toggleOptions('ledOptions2')\">\n";
  htmlCode += "<h2>Monitor LED 2 (Living room)</h2>\n";
  htmlCode += "<div id=\"ledOptions2\" class=\"options\">\n";
  htmlCode += "<button onclick=\"toggleLED('on', 2)\">Switch On the Light</button>\n";
  htmlCode += "<button onclick=\"toggleLED('off', 2)\">Switch Off the Light</button>\n";
  htmlCode += "</div>\n";
  htmlCode += "<div id=\"ledStatus2\" class=\"value\">" + String(LEDStatus2 ? "LED On" : "LED Off") + "</div>\n";
  htmlCode += "</div>\n";

  // LED 3 (Blue) Box
  htmlCode += "<div class=\"box\" onclick=\"toggleOptions('ledOptions3')\">\n";
  htmlCode += "<h2>Monitor LED 3 (Outside the house)</h2>\n";
  htmlCode += "<div id=\"ledOptions3\" class=\"options\">\n";
  htmlCode += "<button onclick=\"toggleLED('on', 3)\">Switch On the Light</button>\n";
  htmlCode += "<button onclick=\"toggleLED('off', 3)\">Switch Off the Light</button>\n";
  htmlCode += "</div>\n";
  htmlCode += "<div id=\"ledStatus3\" class=\"value\">" + String(LEDStatus3 ? "LED On" : "LED Off") + "</div>\n";
  htmlCode += "</div>\n";

  // LDR Value Box
  htmlCode += "<div class=\"box\" onclick=\"refreshLDR()\">\n";
  htmlCode += "<h2>Monitor Light intensity(ldr)</h2>\n";
  htmlCode += "<div id=\"ldrValue\" class=\"value\">" + String(ldrValue) + "</div>\n";
  htmlCode += "</div>\n";

  // Temperature Box
  htmlCode += "<div class=\"box\" onclick=\"refreshDHT('temperature')\">\n";
  htmlCode += "<h2>Monitor Temperature</h2>\n";
  htmlCode += "<div id=\"temperature\" class=\"value\">" + String(dht.readTemperature()) + " &deg;C</div>\n";
  htmlCode += "</div>\n";

  // Humidity Box
  htmlCode += "<div class=\"box\" onclick=\"refreshDHT('humidity')\">\n";
  htmlCode += "<h2>Monitor Humidity</h2>\n";
  htmlCode += "<div id=\"humidity\" class=\"value\">" + String(dht.readHumidity()) + " %</div>\n";
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
  bool* LEDStatus;

  // Determine which LED's status to update
  if (led == 1) {
    LEDStatus = &LEDStatus1;
  } else if (led == 2) {
    LEDStatus = &LEDStatus2;
  } else if (led == 3) {
    LEDStatus = &LEDStatus3;
  } else {
    server.send(400, "text/plain", "Invalid LED");
    return;
  }

  if (action == "on") {
    *LEDStatus = HIGH;
  } else if (action == "off") {
    *LEDStatus = LOW;
  }

  server.send(200, "text/plain", *LEDStatus ? "LED On" : "LED Off");
}

void handle_LDR() {
  ldrValue = analogRead(ldrPin);
  server.send(200, "text/plain", String(ldrValue));
}

void handle_DHT() {
  String action = server.arg("action");
  String response = "{}";

  if (action == "temperature") {
    float temperature = dht.readTemperature();
    response = "{\"temperature\":\"" + String(temperature) + "\"}";
  } else if (action == "humidity") {
    float humidity = dht.readHumidity();
    response = "{\"humidity\":\"" + String(humidity) + "\"}";
  }

  server.send(200, "application/json", response);
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
