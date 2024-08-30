#include "webserver.h"

WebServer::WebServer(String _mdnsName): 
server(80), 
mdnsName(_mdnsName),
lastMDNSUpdate(millis()),
lastConnectionHandling(millis())
{}

void WebServer::init() {
    Serial.print("Connecting to ");
    Serial.println(SSID);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (!MDNS.begin(this->mdnsName)) {  // Start the mDNS responder
        Serial.println("Error setting up MDNS responder!");
    }
    Serial.println("mDNS responder started for " + this->mdnsName);

    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);

    server.begin();
}

void WebServer::tick() {
    // Update mDNS every MDNS_UPDATE_INTERVAL milliseconds
    if(millis() - lastMDNSUpdate > MDNS_UPDATE_INTERVAL) {
        lastMDNSUpdate = millis();

        // Call MDNS.update() to make sure queries are processed
        MDNS.update();
    }

    WiFiClient client = server.available(); // Check for incoming clients

    if (!client) {
        return; // Early exit if no client is connected
    }

    Serial.println("New Client.");
    while (client.connected()) {
        if (client.available()) {
            char c = client.read();
            //Serial.write(c);
            header += c;

            if (c == '\n' && currentLine.isEmpty()) {
                sendHttpResponse(client);
                break; // Exit if the end of the client HTTP request
            }
            handleIncomingChar(c);
        }
    }

    cleanupClient(client);
}

void WebServer::handleIncomingChar(char c) {
    if (c == '\n') {
        currentLine = ""; // Clear current line if new line
    } else if (c != '\r') {
        currentLine += c; // Append to current line if not carriage return
    }
}

void WebServer::sendHttpResponse(WiFiClient& client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:application/json");
    client.println("Access-Control-Allow-Origin: *"); // Allows all domains
    client.println("Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS"); // Specifies allowed methods
    client.println("Access-Control-Allow-Headers: Content-Type, Authorization, X-Requested-With"); // Specifies allowed headers
    client.println("Connection: close");
    client.println();

    processApiRoutes(client);
    client.println(); // End the HTTP response with a blank line
}

void WebServer::processApiRoutes(WiFiClient& client) {
    for(int i = 0; i < 10; i++) {
        if(routes[i] != NULL) {
            if(header.indexOf(routes[i]->route) >= 0) {
                routes[i]->handle(header, client);
            }
        }
    }
}

void WebServer::cleanupClient(WiFiClient& client) {
    header = "";
    client.stop();
    Serial.println("Client disconnected.\n");
}

void WebServer::registerRoute(RouteHandler* routeHandler) {
    Serial.println("Registering route: " + routeHandler->route);

    // Find the first available route slot
    for (int i = 0; i < 10; i++) {
        if (routes[i] == NULL) {
            routes[i] = routeHandler;
            break;
        }
    }
}