#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SD.h>

// WiFi credentials
const char* ssid = "fenilpc";  // Replace with your WiFi SSID
const char* password = "12345678";  // Replace with your WiFi password

// Firebase credentials
const char* firebaseUrl = "https://macut-api-default-rtdb.firebaseio.com/data.json";  // Firebase database URL
const char* firebaseAuth = "AIzaSyCW39BT9fof5O0dq2s0rl5vbIyDnnomaz0Y"; // Replace with your Firebase secret or API key

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

  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // Send the content of file2.txt from SD to Firebase
  sendFile2ContentToFirebase();
}

void loop() {
  fetchDataFromFirebase(); // Fetch the latest data from Firebase
  delay(2000); // Wait for 10 seconds before checking again
}


// Function to read content from SD card and send it to Firebase
void sendFile2ContentToFirebase() {
  // Open the file for reading
  File file = SD.open("/file2.txt");

  if (file) {
    String fileContent = "";  // Variable to store file content

    // Read the content of the file and store it in fileContent
    while (file.available()) {
      fileContent += (char)file.read();
    }

    file.close();  // Close the file after reading

    // Now send the file content to Firebase
    sendDataToFirebase("file2", fileContent);
    Serial.println("Sent file2 content to Firebase.");
  } else {
    Serial.println("Error opening file2.txt for reading.");
  }
}

// Function to send updated data to Firebase
void sendDataToFirebase(const char* fileKey, const String& content) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Append authentication token to the Firebase URL
    String firebaseUrlWithAuth = String(firebaseUrl) + "?auth=" + firebaseAuth;

    http.begin(firebaseUrlWithAuth.c_str()); // URL with authentication
    http.addHeader("Content-Type", "application/json");

    // Prepare the payload as a JSON object
    String payload = "{\"" + String(fileKey) + "\":\"" + content + "\"}";

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

// Function to update file2 on SD card with the new content from the website
void updateFile2OnSD(const String& newContent) {
  // Remove the old file2.txt before writing new content
  if (SD.exists("/file2.txt")) {
    SD.remove("/file2.txt");
    Serial.println("Old file2.txt deleted.");
  }
  
  // Create the file again and write new content to it
  File file = SD.open("/file2.txt", FILE_WRITE);
  
  // Check if the file was opened successfully
  if (!file) {
    Serial.println("Error opening file for writing");
    return;
  }

  // Write the new content to the file
  file.print(newContent);
  
  // Close the file after writing
  file.close();
  
  // Confirm the SD card write
  Serial.println("file2 updated on SD card with new content.");
}

String lastFile2Content = "";  // Store the last content of file2

void handleIncomingData(const String& payload) {
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (!error) {
    if (doc.containsKey("file2")) {
      String newFile2Content = doc["file2"].as<String>();

      // Debugging: Print the received content
      Serial.println("Received content from Firebase for file2: ");
      Serial.println(newFile2Content);

      // Only update the file if content has changed
      if (newFile2Content != lastFile2Content) {
        Serial.println("Content has changed. Updating file2...");
        
        // Delete old file2 and create new one with updated content
        updateFile2OnSD(newFile2Content);
        
        // Update the stored content
        lastFile2Content = newFile2Content;

        // Optionally, send the update to Firebase if needed
        sendDataToFirebase("file2", newFile2Content);
      } else {
        Serial.println("No changes in content. No update needed.");
      }
    }
  } else {
    Serial.println("Failed to parse incoming JSON payload.");
  }
}

void fetchDataFromFirebase() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    String firebaseUrlWithAuth = String(firebaseUrl) + "?auth=" + firebaseAuth;
    http.begin(firebaseUrlWithAuth.c_str());
    int httpResponseCode = http.GET();  // Use GET to fetch data
    
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("Received data from Firebase:");
      Serial.println(payload);
      
      // Handle the incoming data and update SD card
      handleIncomingData(payload);
    } else {
      Serial.print("Error in HTTP request: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi disconnected.");
  }
}

void readFileContentFromSD() {
  File file = SD.open("/file2.txt");
  if (file) {
    Serial.println("file2 content from SD card:");
    while (file.available()) {
      Serial.write(file.read());
    }
    file.close();
  } else {
    Serial.println("Error opening file2.txt to read.");
  }
}
