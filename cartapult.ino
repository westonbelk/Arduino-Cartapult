#include <SPI.h>
#include <WiFi.h>

char ssid[] = "";
char pass[] = "";
int status = WL_IDLE_STATUS;
WiFiServer server(23);
boolean alreadyConnected = false;



void setup() {
    Serial.begin(9600);
    powerMotors();
    startServer();
}


void loop() {
    getClientMessage();

}


void powerMotors() {
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    Serial.println("Motors Activated");
}




void startServer() {
    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present"); 
        // don't continue:
        while(true);
    }

    // attempt to connect to Wifi network:
    while ( status != WL_CONNECTED) { 
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        
        // Connect to WPA/WPA2 network.
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
    } 
    server.begin();
    printWifiStatus();    
}




void getClientMessage() {
    // wait for a new client:
    WiFiClient client = server.available();
    
    // when the client sends the first byte, say hello:
    if (client) {
        if (!alreadyConnected) {
            // clead out the input buffer:
            client.flush();    
            Serial.println("We have a new client");
            client.println("Hello, client!"); 
            alreadyConnected = true;
        } 
        
        if (client.available() > 0) {
            char thisChar = client.read();
            // echo the bytes back to the client:
            server.write(thisChar);
            // echo the bytes to the server as well:
            Serial.write(thisChar);
        }
    }
}




void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    
    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

