#include <WiFi.h>
#include "HarborClient.h"
#include <time.h>

// WiFi credentials
const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";

// Telemetry Harbor API info
const char* harborEndpoint = "API_ENDPOINT";
const char* harborApiKey   = "API_KEY";
const char* shipId         = "Plant 1";

// Harbor client
HarborClient harbor(harborEndpoint, harborApiKey);

// Soil sensor setup
const int soilPin = 34;  // ESP32 ADC pin
const int ledPin = 2;    // onboard LED

// Offsets / Calibration
const float SOIL_OFFSET = 0.0; // optional, in %

// NTP setup
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

// Sleep timing
const uint64_t uS_TO_S_FACTOR = 1000000ULL;
const int SLEEP_INTERVAL_SEC = 5 * 60; // Read every 5 min

// Soil moisture conversion
float readSoilMoisture() {
  int raw = analogRead(soilPin);       // 0–4095
  Serial.printf("Raw ADC reading: %d\n", raw);
  float moisture = map(raw, 4095, 0, 0, 100); // invert
  moisture += SOIL_OFFSET;
  if (moisture < 0) moisture = 0;
  if (moisture > 100) moisture = 100;
  Serial.printf("Converted moisture: %.2f%%\n", moisture);
  return moisture;
}

// Get current UTC time as ISO8601
String getISOTime() {
  time_t now = time(NULL);
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);

  char buf[30];
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
  return String(buf);
}

// Connect WiFi with minimal retries
bool connectWiFi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 15) {
    delay(200);
    retries++;
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wi-Fi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("Wi-Fi connection FAILED");
    return false;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("==== ESP32 Starting ====");

  setCpuFrequencyMhz(80); // reduce power
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect Wi-Fi
  if (!connectWiFi()) {
    Serial.println("Cannot connect to Wi-Fi. Would sleep now (disabled for debugging).");
    //esp_deep_sleep(SLEEP_INTERVAL_SEC * uS_TO_S_FACTOR);
  }

  // Sync time
  Serial.println("Syncing NTP time...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to get NTP time!");
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    //esp_deep_sleep(SLEEP_INTERVAL_SEC * uS_TO_S_FACTOR);
  } else {
    Serial.println("NTP time synced.");
    Serial.printf("Current UTC time: %s\n", getISOTime().c_str());
  }

  // Read soil sensor
  Serial.println("Reading soil sensor...");
  float soilMoisture = readSoilMoisture();

  String isoTime = getISOTime();

  // Prepare reading
  GeneralReading readings[1];
  readings[0] = {shipId, "SoilMoisture", soilMoisture, isoTime};

  // Send batch
  Serial.printf("Sending data: Moisture = %.2f%% at time %s\n", soilMoisture, isoTime.c_str());
  bool sent = harbor.sendBatch(readings, 1);
  Serial.printf("Data sent: %s\n", sent ? "SUCCESS" : "FAILED");

  // Disconnect Wi-Fi before sleep
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("Wi-Fi disconnected.");

  // Deep sleep (disabled for debugging)
  //esp_sleep_enable_timer_wakeup(SLEEP_INTERVAL_SEC * uS_TO_S_FACTOR);
  //esp_deep_sleep_start();
  Serial.println("==== End of setup ====");
}

void loop() {
  // never used
}
