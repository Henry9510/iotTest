#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>
#include <addons/TokenHelper.h> // Para la generación de tokens de Firebase
#include <addons/RTDBHelper.h>  // Para imprimir y ayudar con la base de datos

#define DHTPIN D4  // Pin donde está conectado el sensor DHT11
#define DHTTYPE DHT11  // Tipo de sensor DHT

// Pines de los botones y LEDs
#define BUTTON_PIN_1 D6  // Botón que controla el LED en D2
#define BUTTON_PIN_2 D5  // Botón que controla el LED en D8
#define LED_PIN_1 D2
#define LED_PIN_2 D8

// Configuración de Firebase y WiFi
#define WIFI_SSID "Duvan_Lopez"
#define WIFI_PASSWORD "Dani18*Lopez42*05."
#define API_KEY "AIzaSyCs6WcwyAeudJvg4voHKAfQes68541CcXk"
#define DATABASE_URL "iotproyceto-default-rtdb.firebaseio.com"
#define USER_EMAIL "iotcontrol95@gmail.com"
#define USER_PASSWORD "H-1995CS"

// Objetos de Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Objeto del sensor DHT
DHT dht(DHTPIN, DHTTYPE);

unsigned long sendDataPrevMillis = 0;
unsigned long readSensorPrevMillis = 0;
unsigned long readInterval = 2000;  // Intervalo de lectura del sensor (2 segundos)
unsigned long sendInterval = 10000; // Intervalo para enviar datos a Firebase (10 segundos)

bool led_1_State = false;  // Estado del LED en D2
bool led_2_State = false;  // Estado del LED en D8

void setup() {
  Serial.begin(115200);

  // Configurar pines
  pinMode(BUTTON_PIN_1, INPUT);
  pinMode(BUTTON_PIN_2, INPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);

  // Iniciar sensor DHT
  dht.begin();

  // Conexión WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.print("Conectado con IP: ");
  Serial.println(WiFi.localIP());

  // Configurar Firebase
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);
}

void loop() {
  // Leer los botones
  bool buttonPressed1 = digitalRead(BUTTON_PIN_1) == HIGH;
  bool buttonPressed2 = digitalRead(BUTTON_PIN_2) == HIGH;

  // Controlar LEDs según el estado de los botones
  if (buttonPressed1) {
    // Si el botón 1 (D6) está presionado, enciende el LED en D2 y apaga el LED en D8
    digitalWrite(LED_PIN_1, HIGH);  
    digitalWrite(LED_PIN_2, LOW);  
    led_1_State = true;
    led_2_State = false;
  } 
  else if (buttonPressed2) {
    // Si el botón 2 (D5) está presionado, enciende el LED en D8 y apaga el LED en D2
    digitalWrite(LED_PIN_2, HIGH);  
    digitalWrite(LED_PIN_1, LOW);  
    led_2_State = true;
    led_1_State = false;
  } 
  else {
    // Si no se presionan botones, apaga ambos LEDs
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    led_1_State = false;
    led_2_State = false;
  }

  // Leer el sensor DHT11 cada 2 segundos
  if (millis() - readSensorPrevMillis > readInterval) {
    readSensorPrevMillis = millis();

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Error al leer el sensor DHT11");
    } else {
      Serial.printf("Temperatura: %.2f °C, Humedad: %.2f %%\n", temperature, humidity);
    }
  }

  // Enviar datos a Firebase cada 10 segundos
  if (Firebase.ready() && (millis() - sendDataPrevMillis > sendInterval || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    
    // Enviar estado de los LEDs
    Firebase.setBool(fbdo, F("/status/led_on_D2"), led_1_State);
    Firebase.setBool(fbdo, F("/status/led_on_D8"), led_2_State);

    // Enviar datos del sensor DHT11
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    Firebase.setFloat(fbdo, F("/sensor/temperature"), temperature);
    Firebase.setFloat(fbdo, F("/sensor/humidity"), humidity);

    Serial.println("Datos enviados a Firebase");
  }
}
