#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include "HTMLPage.h"

const char* ssid = "Kubova5"; 
const char* password = "Rezidencekubova5";


WebServer server(80); // Vytvoří objekt serveru na portu 80
const int buzzerPin = 2; // GPIO pin, na který je připojen bzučák
const int ledPin = 4;
int currentTempo = 500; // Výchozí tempo (60000 ms / 120 BPM)
int currentVolume = 128; // Výchozí hlasitost (střední hodnota pro PWM)
bool metronomeActive = false;
unsigned long lastTick = 0;
int currentTimeSignature = 4; // Výchozí takt 4/4
int beatCount = 0; // Počítadlo dob v taktu
int clicksPerBeat = 1; // Výchozí hodnota: 1 klik na jednu dobu

String beatStates[8] = {"normal", "normal", "normal", "normal", "normal", "normal", "normal", "normal"};


void resetBeatStates();

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // Nastavení LEDC pro bzučák
  ledcAttach(buzzerPin, 2000, 10); // Připojí pin, frekvence 2000 Hz, 8-bitové rozlišení

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", HTMLPage::getHTMLContent());
  });

  server.on("/start", HTTP_GET, []() {
    metronomeActive = true;
    beatCount = 0;
    ledcWrite(buzzerPin, currentVolume);
    server.send(200, "text/plain", "Metronome started");
  });

  server.on("/stop", HTTP_GET, []() {
    metronomeActive = false;
    ledcWrite(buzzerPin, 0);
    resetBeatStates();
    server.send(200, "text/plain", "Metronome stopped");
  });

  server.on("/set-tempo", HTTP_GET, []() {
    if (server.hasArg("tempo")) {
      int tempo = server.arg("tempo").toInt();
      if (tempo <= 0) {
        server.send(400, "text/plain", "Invalid tempo");
      } else {
        currentTempo = 60000 / tempo;
        server.send(200, "text/plain", "Tempo set to " + String(tempo) + " BPM");
      }
    } else {
      server.send(400, "text/plain", "Tempo not specified");
    }
  });

  server.on("/set-volume", HTTP_GET, []() {
    if (server.hasArg("volume")) {
      currentVolume = server.arg("volume").toInt();
      if (metronomeActive) {
        ledcWrite(buzzerPin, currentVolume);
      }
      server.send(200, "text/plain", "Volume set to " + String(currentVolume));
    } else {
      server.send(400, "text/plain", "Volume not specified");
    }
  });

  server.on("/set-time-signature", HTTP_GET, []() {
    if (server.hasArg("timeSignature")) {
      currentTimeSignature = server.arg("timeSignature").toInt();
      server.send(200, "text/plain", "Time Signature set to " + String(currentTimeSignature) + "/4");
    } else {
      server.send(400, "text/plain", "Time Signature not specified");
    }
  });

   server.on("/set-beat", HTTP_GET, []() {
    if (server.hasArg("index") && server.hasArg("state")) {
      int index = server.arg("index").toInt();
      String state = server.arg("state");
      beatStates[index] = state;
      server.send(200, "text/plain", "Beat " + String(index) + " set to " + state);
    } else {
      server.send(400, "text/plain", "Index or State not specified");
    }
  });

  server.on("/set-clicks-per-beat", HTTP_GET, []() {
  if (server.hasArg("clicksPerBeat")) {
    clicksPerBeat = server.arg("clicksPerBeat").toInt();
    server.send(200, "text/plain", "Clicks/beat set to " + String(clicksPerBeat));
  } else {
    server.send(400, "text/plain", "Clicks/beat not specified");
  }
});

  server.begin();
  Serial.println("HTTP server started");
}

void resetBeatStates() {
  for (int i = 0; i < 5; i++) {
    beatStates[i] = "normal"; // Reset stavu všech teček na normální
  }
}



void loop() {
  server.handleClient();
  if (metronomeActive) {
    digitalWrite(ledPin, HIGH);
    for (int click = 0; click < clicksPerBeat; click++) {
      int beatIndex = beatCount % currentTimeSignature;


      if (beatStates[beatIndex] == "accent") {
        ledcWrite(buzzerPin, currentVolume); 
      } else if (beatStates[beatIndex] == "silent") {
        ledcWrite(buzzerPin, 0); 
      } else {
        // Normální úder
        ledcWrite(buzzerPin, currentVolume / 2); // Normální úroveň hlasitosti
      }

      digitalWrite(ledPin, HIGH);
      delay(currentTempo / (2 * clicksPerBeat));
      digitalWrite(ledPin, LOW);
      ledcWrite(buzzerPin, 0); // Zastavení tónu
      delay(currentTempo / (2 * clicksPerBeat));
    }
    beatCount++;
    if (beatCount >= currentTimeSignature) {
      beatCount = 0; // Reset počítadla po dosažení taktu
    }
  }
}

