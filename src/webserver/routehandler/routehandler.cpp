#include "routehandler.h"

RouteHandler::RouteHandler(HTTPMethod method, String route, std::function<void(String, WiFiClient&)> handler) {
    String methodString = method == GET ? "GET" : "POST";
    this->route = methodString + " " + route;
    this->handle = handler;
}