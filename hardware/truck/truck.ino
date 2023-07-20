// Import required libraries (Websockets)
#include <WiFi.h>
#include <WebSocketClient.h> // https://github.com/morrissinger/ESP8266-Websocket/tree/master

// Replace with your network credentials
const char* ssid = "The Best Project";
const char* password = "";

// Buzzer config
#define BUZZER_PIN        16
#define CHN               0   //define the pwm channel


char path[] = "/";
char host[] = "192.168.1.2";

// Create WebSocket client object
WebSocketClient webSocketClient;
WiFiClient client;

void setup() {
    // Start comunication
    Serial.begin(9600);
    pinMode(BUZZER_PIN, OUTPUT);
    ledcAttachPin(BUZZER_PIN, CHN);  //attach the led pin to pwm channel

    // WiFi connection
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    delay(5000);

    // Server connection
    connectWs();

    // On
    ledcWriteTone(CHN, 2000);
    delay(300);
    ledcWriteTone(CHN, 0);
}

void loop() {
    String data;
    char out[256];
    if (client.connected()) { 
        // webSocketClient.sendData("DP01 Active!");
        if(webSocketClient.getData(data)) {
            if (data.length() > 0) {
                Serial.print("Received data: ");
                Serial.println(data);

                if(data == "load"){
                    ledcWriteTone(CHN, 2000);
                    delay(100);
                    ledcWriteTone(CHN, 1000);
                    delay(100);
                    ledcWriteTone(CHN, 3000);
                    delay(100);
                    ledcWriteTone(CHN, 0);

                    webSocketClient.sendData("OK");
                        
                    delay(100);
                } else {
                    webSocketClient.sendData("TR01 Active!");
                }
            }
        }
        
    
    } else {
        connectWs();
    }
    
    delay(3000);
}

static void connectWs() {
    if(client.connect(host, 7890)) {
        Serial.println("Connected");
    } else {
        Serial.println("Connection failed.");
    }

    webSocketClient.path = path;
    webSocketClient.host = host;
    if (webSocketClient.handshake(client)) {
        Serial.println("Handshake successful");
    } else {
        Serial.println("Handshake failed.");
    }
    webSocketClient.sendData("container");
}