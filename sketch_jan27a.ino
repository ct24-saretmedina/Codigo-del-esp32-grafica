/*
 * ESP32-LOGGER-TO-GOOGLE-SHEETS
 * Versión estable
 */

#include <WiFi.h> // Asegúrate de usar la librería WiFi específica para ESP32
#include <HTTPClient.h>

unsigned long timeRead = 0;

// Credenciales de WiFi
const char* ssid = "INFOPLAZA CLUB HOUSE"; // Nombre de la red WiFi
const char* password = "1nf0plazas";       // Contraseña de la red WiFi

// URL de tu Google Apps Script
const char* serverName = "https://script.google.com/macros/s/AKfycbwV5PvM97xp8NiU_cZbqB0mLKpao2CPysa298wGC6ZcVA_3ShtRx2Ei1mRa7cL85ZCo6Q/exec"; // Sustituye con la URL de tu script

void setup() {
    Serial.begin(115200);
    initWiFi();
}

void loop() {
    // Leer datos del sensor cada 2 segundos
    if (millis() - timeRead > 2000) {
        // Generar datos aleatorios simulados
        float randomTemperature = random(18, 40); // Temperatura entre 18 y 40 grados
        int randomHumidity = random(30, 100);     // Humedad entre 30% y 100%

        // Mostrar datos en el Monitor Serie
        Serial.print("Temperature = ");
        Serial.print(randomTemperature);
        Serial.println(" *C");

        Serial.print("Humidity = ");
        Serial.print(randomHumidity);
        Serial.println(" %");

        // Enviar datos al servidor
        sendToGoogleSheet(randomTemperature, randomHumidity);

        // Actualizar el tiempo de lectura
        timeRead = millis();
    }
}

// Función para conectarse a la red WiFi
void initWiFi() {
    Serial.print("Connecting to: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    int timeout = 40; // Tiempo de espera: 10 segundos (40 x 250ms)

    while (WiFi.status() != WL_CONNECTED && (timeout-- > 0)) {
        delay(250);
        Serial.print(".");
    }

    Serial.println("");
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("WiFi connected with IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Failed to connect to WiFi!");
        // Opcional: reiniciar si no se conecta
        // ESP.restart();
    }
}

// Función para enviar datos a Google Sheets
void sendToGoogleSheet(float temp, int hum) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        // Iniciar conexión con el servidor
        http.begin(serverName);
        http.addHeader("Content-Type", "application/json");

        // Crear datos en formato JSON
        String jsonData = "{\"temp\":\"" + String(temp) + "\",\"hum\":\"" + String(hum) + "\"}";

        // Enviar solicitud POST
        int httpResponseCode = http.POST(jsonData);

        // Verificar respuesta del servidor
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            Serial.println("Response: " + response);
        } else {
            Serial.print("Error in HTTP request: ");
            Serial.println(httpResponseCode);
        }

        // Finalizar conexión HTTP
        http.end();
    } else {
        Serial.println("WiFi Disconnected. Cannot send data.");
    }
}