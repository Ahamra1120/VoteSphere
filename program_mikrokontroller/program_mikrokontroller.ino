#include <ESP8266WiFi.h>

//Konfigurasi WiFi
const char *ssid = "qwertypoiu";
const char *password = "1234567890";

//IP Address Server yang terpasang XAMPP
const char *host = "192.168.102.83";

#define b1 5
#define b2 4


void setup() {
  Serial.begin(115200);
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Jika koneksi berhasil, maka akan muncul address di serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  // Proses Pengiriman -----------------------------------------------------------
  delay(100);
  ++value;
  int nilai;
  // Membaca Sensor Analog -------------------------------------------------------
  int datasensor1 = digitalRead(b1);
  int datasensor2 = digitalRead(b2);
  if (datasensor1 = 0 || datasensor2 == 0) {
    nilai = 0;
    Serial.println(nilai);
  } else {
    if (datasensor1 == 1) {
      nilai = 1;
      Serial.println(nilai);
    }

    if (datasensor2 == 1) {
      nilai = 2;
      Serial.println(nilai);
    }
  }
  Serial.print("connecting to ");
  Serial.println(host);

  // Mengirimkan ke alamat host dengan port 80 -----------------------------------
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // Isi Konten yang dikirim adalah alamat ip si esp -----------------------------
  String url = "/iotprojek/write_data.php?data=";  //buat folder php
  url += nilai;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // Mengirimkan Request ke Server -----------------------------------------------
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 1000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}