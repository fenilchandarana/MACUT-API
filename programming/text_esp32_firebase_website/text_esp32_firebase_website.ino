#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "fenilpc";  // Replace with your WiFi SSID
const char* password = "12345678";  // Replace with your WiFi password

// Firebase URL (replace with your Firebase Realtime Database URL)
const char* firebaseUrl = "https://macut-api-default-rtdb.firebaseio.com//data.json"; // .json is important

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");

  // Send simple text to Firebase
  sendDataToFirebase("Hello from ESP32 again!");
}

void loop() {
  // Nothing to do here
}

// Function to send data to Firebase Realtime Database
void sendDataToFirebase(const char* text) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(firebaseUrl); // URL of the Firebase database endpoint
    http.addHeader("Content-Type", "application/json");

    // JSON payload to send (simple text in this case)
    String payload = "{\"message\":\"" + String(text) + "\"}";

    int httpResponseCode = http.PUT(payload);  // Use PUT for updating the database

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println("Response: ");
      Serial.println(http.getString());
    } else {
      Serial.print("Error in sending POST: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
