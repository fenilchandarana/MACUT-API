#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>  // Include the ArduinoJson library

// Replace with your network credentials
const char* ssid = "fenilpc";
const char* password = "12345678";

// Firebase URL (adjust if needed)
const String firebaseUrl = "https://macut-api-default-rtdb.firebaseio.com/data.json";

// Variables to store the last received message
String lastMessage = "";

void setup() {
  // Start serial communication
  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Fetch the message from Firebase when starting up
  fetchDataFromFirebase();
}

void loop() {
  // Periodically check for new data (every 10 seconds in this case)
  fetchDataFromFirebase();
  delay(10000);
}

void fetchDataFromFirebase() {
  // Create an HTTP client
  HTTPClient http;
  
  // Make a GET request to Firebase
  http.begin(firebaseUrl);
  int httpCode = http.GET();

  // If the GET request is successful
  if (httpCode == 200) {
    String payload = http.getString();  // Get the response body

    // Parse the JSON response
    DynamicJsonDocument doc(1024);  // Create a Dynamic JSON document
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {
      String message = doc["message"].as<String>();  // Extract the message field

      // Check if the new message is different from the last one
      if (message != lastMessage) {
        Serial.println("New message: " + message);  // Print the new message
        lastMessage = message;  // Update the last received message
      }
    } else {
      Serial.println("Failed to parse JSON");
    }
  } else {
    Serial.println("Error on HTTP request");
  }

  // End the HTTP request
  http.end();
}
