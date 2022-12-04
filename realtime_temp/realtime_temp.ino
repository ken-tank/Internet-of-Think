#include <DHT.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <stdlib.h>

#define dhtPin D4
#define DHTTYPE DHT11

DHT dht (dhtPin, DHTTYPE);

float suhu() { return dht.readTemperature(); }
float kelembapan() { return dht.readHumidity(); }

const char* ssid = "UGShare v2";
const char* password = "speed_5_GHz";

const char* mqtt_server = "riset.revolusi-it.com";

WiFiClient espClient;
PubSubClient client(espClient);
String messages;

void konek_wifi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); // cetak wifi terkoneksi
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  // Mengambil Pesan dari mqqt server
  Serial.print("Pesan dari MQTT [");
  Serial.print(topic);
  Serial.print("] ");
  messages="";
  for (int i=0;i<length;i++) { 
    char receivedChar = (char)payload[i];
    messages=messages+(char)payload[i]; // ambil pesannya masukkan dalam string
  }
  Serial.println(messages);
}

void reconnect() 
{
  // Ulang terus sampai terkoneksi
  while (!client.connected()) 
  {
    Serial.print("Menghubungkan diri ke MQTT Server : "+(String)mqtt_server);
    // Attempt to connect
    if (client.connect("G.211.19.0079")) {
      Serial.println(" [CONNECTED]");
    } 
    else  {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 1 detik...");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}

void SendMessages(char* topic, String msg) {
  char attributes[100];
  msg.toCharArray(attributes, 100);
  client.publish(topic,attributes,true);
  Serial.print("[");
  Serial.print(topic);
  Serial.print("] : ");
  Serial.print(msg);
  Serial.println(" [TERKIRIM]");
}

void GetMessages(char* topic)
{
  client.subscribe(topic);
}

void setup() {
  Serial.begin(9600);
  dht.begin(); // Memulai Sensor DHT11
  client.setServer(mqtt_server, 1883); // sambungkan client ke mqtt
  client.setCallback(callback); // interaksi callback
}

void loop() {
  if(WiFi.status() != WL_CONNECTED) { konek_wifi(); } // jika tidak konek wifi maka di reconnect
  
  if (!client.connected()) { reconnect(); } // reconnect apabila belum konek 
  else {
    Serial.println();
    SendMessages("iot/suhu79", (String) suhu());
    SendMessages("iot/kelembapan79", (String) kelembapan());
  }

  client.loop();
  delay(1000);
}
