#include <SPI.h>
#include <WiFi.h>

// Define the pins that power the motors.
const int Motor1Pin1 = 8;
const int Motor1Pin2 = 9;
const int Motor2Pin1 = 6;
const int Motor2Pin2 = 5;
const int MotorArmPin1 = 3;
const int MotorArmPin2 = 4;


// Initialize some variables for the WiFi
// Includes the network name, password, and port
char ssid[] = "";
char pass[] = "";
int status = WL_IDLE_STATUS;
WiFiServer server(23);
boolean alreadyConnected = false;



void setup() {
    Serial.begin(9600);
    initializeMotors();
    startServer();
}


void loop() {
    String message = getClientMessage();
    
    if(message.length() > 0) {
        Serial.println("[CLIENT]: " + message);
        
        if(message == "/go" || message == "/start") {
            moveForwards();
        }
        else if (message == "/stop") {
            stopWheels();
        }
    }
}


void initializeMotors() {
    pinMode(Motor1Pin1, OUTPUT);
    pinMode(Motor1Pin2, OUTPUT);
    pinMode(Motor2Pin1, OUTPUT);
    pinMode(Motor2Pin2, OUTPUT);
    pinMode(MotorArmPin1, OUTPUT);
    pinMode(MotorArmPin2, OUTPUT);
}

void startServer() {
    // Check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present"); 
        // Don't continue:
        while(true);
    }

    // Attempt to connect to Wifi network:
    while ( status != WL_CONNECTED) { 
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        
        // Connect to WPA/WPA2 network.
        status = WiFi.begin(ssid, pass);

        // Wait 10 seconds for connection:
        delay(10000);
    } 
    server.begin();
    printWifiStatus();    
}




String getClientMessage() {
    // Wait for a new client:
    WiFiClient client = server.available();
    String message;
    
    // When the client sends the first byte, say hello:
    if (client) {
        
        if (!alreadyConnected) {
            // Clean out the input buffer:
            client.flush();
            Serial.println("(LOG) New client connected.");
            client.println("Hello, client!");
            alreadyConnected = true;
        }
        
        while (client.available() > 0) {
            char thisChar = client.read();
            message += thisChar;
        }
    }
    message.trim();
    return message;
}




void printWifiStatus() {
    // Print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    
    // Print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    
    // Print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}


void moveForwards() {
    Serial.println("(LOG) [WHEELS :: START :: FORWARD]");
    digitalWrite(Motor1Pin1, LOW);
    digitalWrite(Motor1Pin2, HIGH);
    digitalWrite(Motor2Pin1, LOW);
    digitalWrite(Motor2Pin2, HIGH);
}

void stopWheels() {
    Serial.println("(LOG) [WHEELS :: STOP]");
    digitalWrite(Motor1Pin2, LOW);
    digitalWrite(Motor1Pin1, LOW);
    digitalWrite(Motor2Pin1, LOW);
    digitalWrite(Motor2Pin2, LOW);
}

void moveArm() {
    Serial.println("(LOG) [ARM :: START]");
    digitalWrite(MotorArmPin1, HIGH);
    digitalWrite(MotorArmPin2, LOW);
}

void stopArm() {
    Serial.println("(LOG) [ARM :: STOP]");
    digitalWrite(MotorArmPin1, LOW);
    digitalWrite(MotorArmPin2, LOW);
}
