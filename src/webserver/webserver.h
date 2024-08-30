#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include "routehandler/routehandler.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#define SSID     "Fios-DMGk8"
#define PASSWORD "fit56jolly676gum"

#define MDNS_UPDATE_INTERVAL 1000 // Update mDNS every 1s
#define HANDLE_CONNECTIONS_INTERVAL 1000 // Handle connections every 1s

class WebServer {
public:
    WebServer(String);
    void init();
    void tick();
    void handleIncomingChar(char);
    void sendHttpResponse(WiFiClient&);
    void processApiRoutes(WiFiClient&);
    void cleanupClient(WiFiClient&);
    void registerRoute(RouteHandler*);
private:
    WiFiServer server;
    String mdnsName;
    String header;
    String currentLine;
    long lastMDNSUpdate;
    long lastConnectionHandling;
    RouteHandler* routes[10]; // Max of 10 routes
};

#endif