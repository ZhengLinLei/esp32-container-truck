// Import required libraries (Websockets)
#include <WiFi.h>
#include <WebSocketClient.h> // https://github.com/morrissinger/ESP8266-Websocket/tree/master


// Import GPS library
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// JSON
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "The Best Project";
const char* password = "";

// Buzzer config
#define BUZZER_PIN        26
#define CHN               0   //define the pwm channel

// Container size
#define CONTAINER_SIZE    30

// Ultrasonic distance measure
int Trigger[] = {13, 14};
int Echo[] = {12, 27};

int tiempo[] = {0, 0};
int distancia[] = {0, 0};

// Create WebSocket client object
WebSocketClient webSocketClient;
WiFiClient client;

// GPS
/* Create object named bt of the class SoftwareSerial */
SoftwareSerial GPS_SoftSerial(21, 18);/* (Rx, Tx) */
/* Create an object named gps of the class TinyGPSPlus */
TinyGPSPlus gps;

char path[] = "/";
char host[] = "192.168.1.2";

void setup() {
  // Start comunication
  Serial.begin(9600);
  GPS_SoftSerial.begin(9600); /* Define baud rate for software serial communication */

  pinMode(BUZZER_PIN, OUTPUT);
  ledcAttachPin(BUZZER_PIN, CHN);  //attach the led pin to pwm channel

  // Init
  for(int i = 0; i < 2; i++){
    pinMode(Trigger[i], OUTPUT);
    pinMode(Echo[0], INPUT);
    digitalWrite(Trigger[0], LOW);
  }

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

        if(data == "scan"){
          ledcWriteTone(CHN, 2000);
          delay(100);
          ledcWriteTone(CHN, 1000);
          delay(100);
          ledcWriteTone(CHN, 3000);
          delay(100);
          ledcWriteTone(CHN, 0);
          for(int i = 0; i < 2; i++){
            digitalWrite(Trigger[i], HIGH);
            delayMicroseconds(10);
            digitalWrite(Trigger[i], LOW);
            // Distance
            tiempo[i] = pulseIn(Echo[i], HIGH);
            distancia[i] = tiempo[i]/59;

            // Serial.print("Distancia");
            // Serial.print(i);
            // Serial.print(": ");
            // Serial.print(distancia[i]);
            // Serial.println(" cm");
          }
          // Capacity value

          double c_val = (distancia[0] + distancia[1]) / 2;
          double capacity = (c_val * 100) / CONTAINER_SIZE;

          // GPS data
          double lat_val, lng_val;
          smartDelay(1000); /* Generate precise delay of 1ms */

          lat_val = gps.location.lat() || 0; /* Get latitude data */
          lng_val = gps.location.lng() || 0; /* Get longtitude data */

          // Create the JSON
          StaticJsonDocument<1024> doc;
          doc["name"] = "RealContainer-DP01";
          JsonArray data = doc.createNestedArray("coord");
          data.add(lat_val);
          data.add(lng_val);
          JsonArray data2 = doc.createNestedArray("capa");
          data2.add(capacity);
          data2.add(c_val);

          serializeJson(doc, out);

          // Serial.println(out);

          webSocketClient.sendData(out);
            
          delay(100);
        } else {
          webSocketClient.sendData("DP01 Active!");
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
  }else {
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
static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do
  {
    while (GPS_SoftSerial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
    /* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}