#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "Fiberstream";
const char* password = "12345678";

// Server address
const char* serverName = "http://192.168.1.12:5000/air_quality";  

// DHT sensor configuration
#define DHTPIN 14     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// Variable to count time relative
unsigned long startTime;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  dht.begin();

  // after connected, write time from (start time)
  startTime = millis();
}

void loop() {
  // Update value millis()
  unsigned long currentTime = millis() - startTime;

  // time format
  unsigned int seconds = currentTime / 1000;
  unsigned int minutes = seconds / 60;
  unsigned int hours = minutes / 60;

  String timestamp = String(hours) + ":" + String(minutes % 60) + ":" + String(seconds % 60);

  // Wifi verif connectivity
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    Serial.println("Connecting to server:");
    Serial.println(serverName);

    // Start HTTP Connection
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Create JSON data to Send
    String httpRequestData = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + ",\"timestamp\":\"" + timestamp + "\"}";
    Serial.print("Sending data: ");
    Serial.println(httpRequestData);
    
    // Send the data with POST method
    int httpResponseCode = http.POST(httpRequestData);

    // Check response from server
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code:");
      Serial.println(httpResponseCode);
      Serial.println("Response body:");
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    // end HTTP connection
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(60000); // Send data every 60 detik
}
