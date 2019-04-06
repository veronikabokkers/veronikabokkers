
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Ticker.h>


Ticker timer;

char* ssid="VGV751987B171";
char* Passwd = "PvyG9Ge7EDKb";

ESP8266WebServer server;
WebSocketsServer webSocket = WebSocketsServer(81);

char webPage[] PROGMEM = R"(<html><body><div>ABC</div></body></html>)";

void setup() {
  // put your setup code here, to run once:
  WiFi.begin(ssid, Passwd);
  Serial.begin(9600);
  while(WiFi.status()!=WL_CONNECTED) {
    Serial.print("."); 
    delay(500);
  }
  Serial.println();
  Serial.println(WiFi.localIP());

  // handle index
  server.on("/", []() {
    // send index.html
    server.send(200, "text/html", webPage);
  });
  
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  timer.attach(1, getData);
}

void loop() {
  // put your main code here, to run repeatedly:
  webSocket.loop();
  server.handleClient();

}

void getData() {
  int lightInput = analogRead(A0); 
  String json = String(lightInput);
  Serial.println(json);
  webSocket.broadcastTXT(json.c_str(), json.length());
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
}
/*
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

            // send message to client
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);
*/
