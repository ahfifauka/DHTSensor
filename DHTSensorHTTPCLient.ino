#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>


const char* ssid = "CB11";
const char* password = "infonusa";

#define DHTPIN D2         
#define DHTTYPE DHT22     
DHT dht(DHTPIN, DHTTYPE);

// URL API Laravel yang akan diakses
const char* serverUrl = "http://ip?/update"; 

void setup() {
  Serial.begin(115200);
  
  // Menghubungkan ke Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Tampilkan IP yang diberikan oleh router
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  dht.begin();
}

void loop() {
 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C, Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  sendToServer(t, h);

  delay(1000); 
}

void sendToServer(float temperature, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    String url = String(serverUrl) + String(temperature) + "/" + String(humidity);
    http.begin(client,url);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error on sending GET request: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}
