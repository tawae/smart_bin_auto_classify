#include <ESP32Servo.h>
#include <WebServer.h>
#include <WiFi.h>

const char* ssid = "999";
const char* password = "88888888";

// ESP32-CAM (server nhận lệnh chụp ảnh)
const char* esp32cam_ip = "10.245.61.32";
const int esp32cam_port = 8080;

// LM393
const int sensorPin = 4;

// Servo
Servo myservo;
const int servoPin = 13;
int posCW = 0;     // quay phải
int posCCW = 180;  // quay trái
int posMID = 90;   // nghỉ

bool chup = true;

WiFiClient client;
WebServer server(80);

// ===================== HANDLE SERVO REQUEST =====================
void handleServo() {
  if (!server.hasArg("mode")) {
    server.send(400, "text/plain", "Missing mode param");
    Serial.println("Missing");
    return;
  }

  String mode = server.arg("mode");

  if (mode == "CW") {
    chup = false;
    for(int i = 0; i < 91; i+=10) {
      myservo.write(posMID - i);
      delay(300);
    }
    server.send(200, "text/plain", "Servo CW");
    Serial.println("CW, khong nhan cam bien");
    delay(500);
    myservo.write(posMID);
    delay(3000);
    chup = true;
    Serial.println("nhan cam bien");

  } else if (mode == "CCW") {
    chup = false;
    for(int i = 0; i < 91; i+=10) {
      myservo.write(posMID + i);
      delay(300);
    }
    server.send(200, "text/plain", "Servo CCW");
    Serial.println("CCW, khong nhan cam bien");
    delay(500);
    myservo.write(posMID);
    delay(3000);
    chup = true;
    Serial.println("nhan cam bien");

  } else if (mode == "open") {
    myservo.write(posCW);
    server.send(200, "text/plain", "Servo CW");
    chup = false;
    Serial.println("Open, khong nhan cam bien");
  } else {
    chup = false;
    myservo.write(posMID);
    server.send(200, "text/plain", "Servo CLOSE");
    delay(3000);
    chup = true;
    Serial.println("close, bat dau nhan cam bien");
  }
}

// ========================== SETUP ==========================
void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);

  myservo.attach(servoPin);
  myservo.write(posMID);

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  // HTTP server route
  server.on("/servo", handleServo);
  server.begin();
}

// ========================== LOOP ==========================
void loop() {
  server.handleClient();

  int sensorValue = digitalRead(sensorPin);

  if (sensorValue == LOW) {
    Serial.println("LM393 phát hiện vật!");

    // Gửi yêu cầu chụp ảnh sang ESP32-CAM
    if (client.connect(esp32cam_ip, esp32cam_port) && chup == true) {
      client.println("capture");
      client.stop();
      chup = true;
    } else {
      Serial.println("Không kết nối được ESP32-CAM");
    }

    delay(7000);  // tránh spam
  }
}
