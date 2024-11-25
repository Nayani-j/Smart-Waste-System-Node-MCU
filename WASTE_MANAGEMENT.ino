#include <ESP8266WiFi.h>

const int trigPin = D5;
const int echoPin = D6;
long duration;
int distance;
float level;

const char* ssid = "Wi-Fi Name"; 
const char* password = "Password";

WiFiServer server(80);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  Serial.print("Connecting to WiFi Network: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nSuccessfully connected to WiFi.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
  Serial.println("Server started.");
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0340 / 2;
  level = ((28 - distance) / 28.0) * 100;

  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print("Fill Level: ");
  Serial.println(level);

  delay(1000);

  WiFiClient client = server.available();
  if (client) {
    Serial.println("Web Client connected.");
    String request = client.readStringUntil('\r');
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println("Refresh: 10"); 
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head><title>IoT-Based Dustbin</title></head>");
    client.println("<style>html { font-family: Cairo; text-align: center; color: #333333; background-color: #f3ffee;}</style>");
    client.println("<h1>IoT-Based Dustbin</h1>");
    client.println("<p>Fill Level: ");
    client.print(level);
    client.println("%</p>");
    client.println("</html>");
    delay(1);
  }
}
