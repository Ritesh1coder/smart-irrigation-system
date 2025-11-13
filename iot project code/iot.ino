// Smart Irrigation System with DHT11, OLED (SPI), Soil Moisture Sensor, and Relay
// Developed for NodeMCU ESP8266

#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

// === OLED CONFIG ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI  D3  // Data
#define OLED_CLK   D4  // Clock6
#define OLED_DC    D8  // Data/Command
#define OLED_CS    D6  // Chip Select
#define OLED_RESET -1  // Not connected
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// === DHT Sensor ===
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// === Soil Moisture Sensor ===
const int soilPin = A0;
const int AirValue = 790;     // Calibrated dry value
const int WaterValue = 390;   // Calibrated wet value
int soilMoisturePercent = 0;

// === Relay ===
#define relayPin 5 // Pump control (active LOW relay)

// === WiFi & ThingSpeak ===
const char* ssid = "ABC";
const char* pass = "12345566";
String apiKey = "Y61FOA06BK8SOBWS";
const char* server = "api.thingspeak.com";

WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Turn OFF pump (active LOW)

  // Setup OLED Display
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("OLED failed to initialize");
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("OLED Ready");
    display.display();
  }

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("\nWiFi Connected");
  }
  else {
    Serial.println("WiFi Connection failed");
  }
  Serial.println("\nWiFi Connected");
}

void loop() {
 // digitalWrite(5,1);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT Read Failed");
    return;
  }

  // Read soil sensor and convert to percentage
  int soilVal = analogRead(soilPin);
  soilMoisturePercent = map(soilVal, AirValue, WaterValue, 0, 100);
  soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);

  // Relay logic (Active LOW)
  if (soilMoisturePercent <= 70) {
   digitalWrite(relayPin, LOW); // Turn ON pump
    Serial.println("Pump is ON");
  } else {
   digitalWrite(relayPin, HIGH); // Turn OFF pump
    Serial.println("Pump is OFF");
  }

  // Serial Debug
  Serial.print("Soil Moisture: "); Serial.print(soilMoisturePercent); Serial.println("%");
  Serial.print("Humidity: "); Serial.println(h);
  Serial.print("Temperature: "); Serial.println(t);

  // Display on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Soil RH: "); display.print(soilMoisturePercent); display.println("%");
  display.print("Humidity: "); display.print(h); display.println("%");
  display.print("Temp: "); display.print(t); display.println(" C");
  display.print("Pump: "); display.println(digitalRead(relayPin) == LOW ? "ON" : "OFF");
  display.display();

  // ThingSpeak Update
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=" + String(soilMoisturePercent);
    postStr += "&field2=" + String(h);
    postStr += "&field3=" + String(t);
    postStr += "&field4=" + String(digitalRead(relayPin) == LOW ? 1 : 0);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    client.stop();
    Serial.println("Data sent to ThingSpeak");
  }

  delay(5000); // 5 second delay
}
