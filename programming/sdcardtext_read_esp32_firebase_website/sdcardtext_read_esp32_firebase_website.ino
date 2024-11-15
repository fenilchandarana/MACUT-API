#include <WiFi.h>
#include <HTTPClient.h>
#include <SD.h>

// WiFi credentials
const char* ssid = "fenilpc";  // Replace with your WiFi SSID
const char* password = "12345678";  // Replace with your WiFi password

// Firebase URL (replace with your Firebase Realtime Database URL)
const char* firebaseUrl = "https://macut-api-default-rtdb.firebaseio.com/data.json"; // .json is important

// SD card chip select pin
const int chipSelect = 5;

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

  // Initialize the SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // Send data from SD card files to Firebase on setup
  sendDataToFirebase("file1", "/file1.txt");
  sendDataToFirebase("file2", "/file2.txt");
}

void loop() {
  // Continuously check and update Firebase every 10 seconds
  delay(10000);
  sendDataToFirebase("file1", "/file1.txt");
  sendDataToFirebase("file2", "/file2.txt");
}

// Function to read data from SD card and send it to Firebase
void sendDataToFirebase(const char* fileKey, const char* fileName) {
  // Open the file for reading
  File file = SD.open(fileName);
  if (!file) {
    Serial.print("Error opening file: ");
    Serial.println(fileName);
    return;
  }

  // Read the content of the file
  String fileContent = "";
  while (file.available()) {
    fileContent += (char)file.read(); // Read each character
  }
  file.close();

  // Clean up the content (remove trailing spaces or newline characters if any)
  fileContent.trim();  // Ensure no trailing spaces or newlines

  if (fileContent.length() == 0) {
    fileContent = "No content";  // Handle empty files
  }

  // Now send this content to Firebase
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(firebaseUrl); // URL of the Firebase database endpoint
    http.addHeader("Content-Type", "application/json");

    // Prepare the payload as a JSON object
    String payload = "{\"" + String(fileKey) + "\":\"" + fileContent + "\"}";

    int httpResponseCode = http.PATCH(payload);  // Use PATCH for updating specific fields

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println("Response: ");
      Serial.println(http.getString());
    } else {
      Serial.print("Error in sending PATCH: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
