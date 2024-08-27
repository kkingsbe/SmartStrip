#ifndef ROUTE_HANDLER_H
#define ROUTE_HANDLER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <functional>  // Include the header for std::function

enum HTTPMethod {
    GET,
    POST
};

class RouteHandler {
    public:
        RouteHandler(HTTPMethod method, String route, std::function<void(String, WiFiClient&)> handler);
        std::function<void(String, WiFiClient&)> handle;
        String route;
};

#endif