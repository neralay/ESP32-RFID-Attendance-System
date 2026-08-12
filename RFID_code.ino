#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D2
#define RST_PIN D1
#define BUZZER D0

MFRC522 mfrc522(SS_PIN, RST_PIN);

// ✅ WiFi (your hotspot)
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ✅ Your Google Script Deployment URL
String scriptURL = "YOUR_GOOGLE_SHEET_URL";

void setup() {
  Serial.begin(115200);
  delay(1000);

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  Serial.println("\n🚀 System Starting...");

  connectWiFi();
}

// 🔌 WiFi Connect Function
void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("📶 Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi Connected!");
  Serial.print("📡 IP Address: ");
  Serial.println(WiFi.localIP());

  // 🔊 Beep on connect
  digitalWrite(BUZZER, HIGH);
  delay(300);
  digitalWrite(BUZZER, LOW);
}

void loop() {

  // 🔄 Auto reconnect WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("🔄 Reconnecting WiFi...");
    connectWiFi();
    delay(2000);
    return;
  }

  // 🔍 RFID Card Detect
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  String rfid = "";

  // 📌 Read UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    rfid += String(mfrc522.uid.uidByte[i], HEX);
  }

  rfid.toUpperCase();

  Serial.print("\n📌 Card UID: ");
  Serial.println(rfid);

  // 🔊 Scan beep
  digitalWrite(BUZZER, HIGH);
  delay(200);
  digitalWrite(BUZZER, LOW);

  // 🌐 Send Data to Google Sheet
  WiFiClientSecure client;
  client.setInsecure();  // HTTPS fix

  HTTPClient http;

  String url = scriptURL + "?rfid=" + rfid;

  Serial.println("📤 Sending Data...");
  Serial.println(url);

  http.begin(client, url);
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();

    Serial.print("✅ Server Response: ");
    Serial.println(payload);

    // 🔊 Success beep
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);

  } else {
    Serial.print("❌ Error Code: ");
    Serial.println(httpCode);

    // 🔊 Error beep
    for (int i = 0; i < 3; i++) {
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
      delay(100);
    }
  }

  http.end();

  delay(2000);
}
