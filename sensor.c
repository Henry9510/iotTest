
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>

// Configuración del sensor DHT11
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Configuración de la red Wi-Fi
const char* ssid = "PocoWifi";
const char* password = "H3nry1995";

// Dirección IP del servidor local (XAMPP)
const char* serverName = "http://192.168.87.188/sensores/guardar_datos.php";  // Cambia la IP por la de tu PC

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Conectar a la red Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Conectando a Wi-Fi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("Conexión Wi-Fi establecida");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {  // Verifica si está conectado a Wi-Fi

    // Leer los datos del sensor
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float tiempoEncendido = 10.5; // Simulando el tiempo de encendido de la bomba

    if (isnan(h) || isnan(t)) {
      Serial.println("Error al leer el sensor");
      return;
    }

    // Crear un objeto HTTP
    HTTPClient http;

    // Formatear los datos como JSON
    String jsonPayload = "{\"sensor_id\":\"esp8266_v2\",\"temperatura\":";
    jsonPayload += t;
    jsonPayload += ",\"humedad\":";
    jsonPayload += h;
    jsonPayload += ",\"tiempo_encendido\":";
    jsonPayload += tiempoEncendido;
    jsonPayload += "}";

    // Enviar datos al servidor
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Respuesta del servidor:");
      Serial.println(response);
    } else {
      Serial.print("Error en la solicitud POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Finaliza la conexión HTTP
  } else {
    Serial.println("No hay conexión Wi-Fi");
  }

  delay(10000);  // Enviar los datos cada 10 segundos
}
