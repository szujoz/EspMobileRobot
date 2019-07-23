#include "webserver.h"

ESP8266WebServer* WEBSERVER::server    = NULL;
WebSocketsServer* WEBSERVER::webSocket = NULL;
uint8_t* WEBSERVER::rxBuffer = new uint8_t [256];
bool WEBSERVER::dataReceived = false;

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
     String html ="<!DOCTYPE html> <html> <head> <meta content=\"width=device-width, initial-scale=1\"> <style> .switch { position: relative; display: inline-block; width: 50px; height: 20px; } .switch input { opacity: 0; width: 0; height: 0; } .slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; -webkit-transition: .4s; transition: .4s; } .slider:before { position: absolute; content: \"\"; height: 12px; width: 12px; left: 4px; bottom: 4px; background-color: white; -webkit-transition: .4s; transition: .4s; } input:checked+.slider { background-color: #2196F3; } input:focus+.slider { box-shadow: 0 0 1px #2196F3; } input:checked+.slider:before { -webkit-transform: translateX(28px); -ms-transform: translateX(28px); transform: translateX(28px); } .slider.round { border-radius: 34px; } .slider.round:before { border-radius: 50%; } </style> <script id=\"globals\"> var systemData = { engine: '', l_en: '', l_cp1: '', l_cp2: '', r_en: '', r_cp1: '', r_cp2: '', imu_ax: '', imu_ay: '', imu_az: '', imu_wx: '', imu_wy: '', imu_wz: '', imu_Bx: '', imu_By: '', imu_Bz: '', imu_P: '', lightsOn: '', navi_x: '', navi_y: '', navi_w: '' }; var dataID = { engine: \"00\", l_en: \"01\", l_cp1: \"02\", l_cp2: \"04\", r_en: \"05\", r_cp1: \"06\", r_cp2: \"07\", imu_ax: \"08\", imu_ay: \"09\", imu_az: \"0A\", imu_wx: \"0B\", imu_wy: \"0C\", imu_wz: \"0D\", imu_Bx: \"0E\", imu_By: \"0F\", imu_Bz: \"10\", imu_P: \"11\", lightsOn: \"12\", navi_x: \"13\", navi_y: \"14\", navi_w: \"15\" }; </script> <script id=\"functions\"> var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']); connection.onopen = function () { connection.send('Connect ' + new Date()); }; connection.onerror = function (error) { console.log('WebSocket Error ', error); }; connection.onmessage = function (e) { console.log('Server: ', e.data); }; function sendDataElement(id, dataMember, msgId, bool = false) { if (bool == true) { var val = document.getElementById(id).checked; (val == true) ? val = '1' : val = '0'; } else { var val = parseInt(document.getElementById(id).value).toString(16); } if (val.length < 2) { val = '0' + val; } systemData[dataMember] = val; var msg = '#' + msgId + systemData[dataMember]; // DEBUG document.getElementById(\"demo\").innerHTML = \"Msg: \" + msg; printObject(systemData); // END DEBUG console.log('Msg: ' + msg); connection.send(msg); } // DEBUG function printObject(o) { var msg = \"\"; for (var p in o) { msg += p + ': ' + o[p] + '\n'; } alert(msg); document.getElementById(\"demo2\").innerHTML = \"Msg: \" + msg; } // END DEBUG </script> </head> <body> <div id=\"Main\"> <h1>Engine on</h1> <label class=\"switch\" oninput=\"sendDataElement('engine', 'engine', dataID.engine, true)();\"> <input type=\"checkbox\" id=\"engine\"> <span class=\"slider round\"></span> </label> <br /><br /><br /><br /> </div> <div id=\"Motors\"> <table class=\"fixed\"> <h2>Motors:</h2> <col width=\"295px\" /> <col width=\"195px\" /> <tr> <td> <p> <font size=\"4\" bold>Left Motor:</font> </p> </td> <td> <p> <font size=\"4\" bold>Right Motor:</font> </p> </td> </tr> </table> <table class=\"fixed\"> <col width=\"60px\" /> <col width=\"135px\" /> <col width=\"100px\" /> <col width=\"60px\" /> <col width=\"135px\" /> <tr> <td>Enable:</td> <td> <label class=\"switch\" oninput=\"sendDataElement('l_en', 'l_en', dataID.l_en, true)();\"> <input type=\"checkbox\" id=\"l_en\"> <span class=\"slider round\"></span> </label> </td> <td></td> <td>Enable:</td> <td> <label class=\"switch\" oninput=\"sendDataElement('r_en', 'r_en', dataID.r_en, true)();\"> <input type=\"checkbox\" id=\"r_en\"> <span class=\"slider round\"></span> </label> </td> </tr> <tr> <td>Direction:</td> <td><input id=\"l_cp1\" type=\"range\" min=\"0\" max=\"1\" step=\"1\" oninput=\"sendDataElement('l_cp1', 'l_cp1', dataID.l_cp1)();\"></td> <td></td> <td>Direction:</td> <td><input id=\"r_cp1\" type=\"range\" min=\"0\" max=\"1\" step=\"1\" oninput=\"sendDataElement('r_cp1', 'r_cp1', dataID.r_cp1)();\"></td> </tr> <tr> <td>Speed:</td> <td><input id=\"l_cp2\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" value=\"0\" oninput=\"sendDataElement('l_cp2', 'l_cp2', dataID.l_cp2);\"></td> <td></td> <td>Speed:</td> <td><input id=\"r_cp2\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" value=\"0\" oninput=\"sendDataElement('r_cp2', 'r_cp2', dataID.r_cp2)();\"></td> </tr> <table /> <br /><br /> </div> <div id=\"Navigation\"> <h2>Navigation: (communication doesn't work in this direction yet)</h2> <table> <col width=\"60px\" /> <col width=\"135px\" /> <tr> <td>x:</td> <td> <p id=\"navi_x\"></p> </td> </tr> <tr> <td>y:</td> <td> <p id=\"navi_y\"></p> </td> </tr> <tr> <td>omega:</td> <td> <p id=\"navi_w\"></p> </td> </tr> </table> </div> <!-- DEBUG --> <p id=\"demo\"></p> <p id=\"demo2\"></p> </body> </html>";
     server->send(200, "text/html", html);
  });

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
                Serial.println(rxBuffer[3]);
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
