#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>

class WEBSERVER
{
  private:
    static ESP8266WebServer* server;
    static WebSocketsServer* webSocket;

    char ssid[20];
    char pwd[20];

    static bool     dataReceived;
    static uint8_t* rxBuffer;
    uint8_t txBuffer[256];

  public:
    WEBSERVER(char* const SSID, char* const PWD);
    ~WEBSERVER(void);

    void process(void);
    static uint8_t* getRxData(void);
    static bool newDataReceived(void);

  private:
    static void webSocketEvent(uint8_t const num, WStype_t const type, uint8_t* const payload, size_t const length);
};
