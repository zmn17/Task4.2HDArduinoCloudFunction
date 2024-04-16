#include <Arduino.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <Firebase_Arduino_WiFiNINA.h>

#define FIREBASE_HOST "traffic-controller-8b08c-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "P0UOekuGZquLVMWBZt44v8iw46gLdWA0xLAfkS93"

const char* ssid = "Not-Connected...";
const char* password = "ZH9FagdbAdXMNpk3";
const String path = "/status/status";

FirebaseData firebaseData;

void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
  }
  Serial.println("\nYou're connected to the WiFi network");
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(2, OUTPUT);

  connectToWiFi();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, ssid, password);
  Firebase.reconnectWiFi(true);

  if (Firebase.setString(firebaseData, path, "OFF")) {
    Serial.println("Firebase initialization successful");
  } else {
    Serial.println("Firebase initialization failed. Error: " + firebaseData.errorReason());
  }
}

void updateLEDs(const String& status) {
  if (status == "RED") {
    digitalWrite(12, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(2, HIGH);
    Serial.println("RED LED ON");
  } else if (status == "GREEN") {
    digitalWrite(12, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(2, HIGH);
    Serial.println("Green LED on");
  } else if (status == "BLUE") {
    digitalWrite(12, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(2, LOW);
    Serial.println("Blue LED on");
  } else if (status == "OFF") {
    digitalWrite(12, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(2, HIGH);
    Serial.println("OFF");
  } else {
    Serial.println("Unknown status received");
  }
}

void loop() {
  if (Firebase.getString(firebaseData, path)) {
    String status = firebaseData.stringData();
    updateLEDs(status);
  } else {
    Serial.print("Firebase Error: ");
    Serial.println(firebaseData.errorReason());
  }
  delay(1000); // Delay to reduce the frequency of Firebase queries
}
