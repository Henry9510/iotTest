#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// Configuración de Firebase y WiFi
#define WIFI_SSID "Duvan_Lopez"
#define WIFI_PASSWORD "Dani18*Lopez42*05."
#define API_KEY "AIzaSyCs6WcwyAeudJvg4voHKAfQes68541CcXk"
#define DATABASE_URL "https://iotproyceto-default-rtdb.firebaseio.com"
#define USER_EMAIL "iotcontrol95@gmail.com"
#define USER_PASSWORD "95HC0110"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

// Función para generar una cadena aleatoria de letras y números
String generateRandomString(int length) {
    const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    String result = "";
    for (int i = 0; i < length; i++) {
        result += alphanum[random(0, sizeof(alphanum) - 1)];
    }
    return result;
}

void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(0)); // Inicializar la semilla para el generador de números aleatorios

    // Conectar a Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    // Inicializar Firebase
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;

    // Inicializar Firebase
    Firebase.begin(&config, &auth);

    // Establecer el número de decimales para float y double
    Firebase.setDoubleDigits(5);
}

void loop() {
    // Firebase.ready() debería ser llamado repetidamente para manejar tareas de autenticación.
    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
        sendDataPrevMillis = millis();

        // Generar y enviar una cadena aleatoria
        String randomData = generateRandomString(10); // Cambia el número para ajustar la longitud de la cadena
        Serial.printf("Set string... %s\n", Firebase.setString(fbdo, F("/test/randomString"), randomData) ? "ok" : fbdo.errorReason().c_str());

        Serial.printf("Generated random string: %s\n", randomData.c_str());
    }
}
