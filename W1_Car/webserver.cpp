#include "webserver.h"
#include <FS.h>

ESP8266WebServer* WEBSERVER::server       = NULL;
WebSocketsServer* WEBSERVER::webSocket    = NULL;
uint8_t*          WEBSERVER::rxBuffer     = new uint8_t [256];
bool              WEBSERVER::dataReceived = false;

WEBSERVER::WEBSERVER(char* const SSID, char* const PWD)
{
  // Initialize class members.
  strncpy(ssid, SSID, strlen(SSID)+1);
  strncpy(pwd,  PWD,  strlen(PWD)+1);
  memset(rxBuffer, 0, sizeof(rxBuffer));
  memset(txBuffer, 0, sizeof(txBuffer));
  server    = new ESP8266WebServer(80);
  webSocket = new WebSocketsServer(81);

  // Configure ESP as Access Point.
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pwd);

  // Debug.
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("HotSpt IP:");
  Serial.println(myIP);

  // start webSocket server
  webSocket->begin();
  webSocket->onEvent(webSocketEvent);

  if(MDNS.begin("esp8266"))
  {
      Serial.println("MDNS responder started");
  }
  
  // handle index
  server->on("/", [this]() {
     // send index.html
     //server->send(200, "text/html", html);
     File file = SPIFFS.open("/webpage.html","r");
     server->streamFile(file, "text/html");
     file.close();
    // server->send(200, "text/html", "/webpage.html");
  });

  // Start the webserver.
  server->begin();

  // Add service to MDNS
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);
}

WEBSERVER::~WEBSERVER(void)
{
  free(server);
  free(webSocket);
  free(rxBuffer);
}

void WEBSERVER::webSocketEvent(uint8_t const num, WStype_t const type, uint8_t* const payload, size_t const length)
{
    Serial.println("event");
    switch(type)
    {
        case WStype_DISCONNECTED:
        {
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        }
        case WStype_CONNECTED:
        {
            IPAddress ip = webSocket->remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

            // send message to client
            webSocket->sendTXT(num, "Connected");
            break;
        }            
        case WStype_TEXT:
        {
            Serial.printf("[%u] get Text: %s\n", num, payload);

            if(payload[0] == '#')
            {
                // we get data
                uint32_t msg = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
                memcpy(rxBuffer, payload+1, 3*2);

                Serial.print("Received data: ");
                Serial.println(msg);

                dataReceived = true;
                Serial.print("Rx buff: ");
                Serial.print(rxBuffer[0]);
                Serial.print(" ");
                Serial.print(rxBuffer[1]);
                Serial.print(" ");
                Serial.print(rxBuffer[2]);
                Serial.print(" ");
                Serial.print(rxBuffer[3]);
                Serial.print(" ");
                Serial.println(rxBuffer[4]);
            }
            break;
        }
    }
}

void WEBSERVER::process(void)
{
    dataReceived = false;
    webSocket->loop();
    server->handleClient();
}

uint8_t* WEBSERVER::getRxData(void)
{
  return rxBuffer;
}

bool WEBSERVER::newDataReceived(void)
{
  return dataReceived;
}
