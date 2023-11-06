#include <OneWire.h>
#include <DallasTemperature.h>

// Mengirimkan sensor DS18B20 ke pin A1
OneWire oneWire(4);
DallasTemperature sensors(&oneWire);

int redLedPin = 11;
int yellowLedPin = 10;
int blueLedPin = 9;

bool isRedBlinking = false;
bool isYellowBlinking = false;
bool isBlueBlinking = false;

unsigned long previousMillis = 0;
const long interval = 1000; // Interval waktu untuk efek kedip (1 detik)

void setup() {
  sensors.begin();
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  sensors.requestTemperatures(); 

  float temperatureC = sensors.getTempCByIndex(0);

  if (temperatureC >= 40.0 && temperatureC <= 45.0) {
    // Rentang suhu ruangan server 40°C hingga 45°C: Nyalakan lampu Merah
    analogWrite(redLedPin, map(temperatureC, 40, 45, 0, 255));
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(blueLedPin, LOW);
    // Efek kedip pada lampu Merah
    if (millis() - previousMillis >= interval) {
      previousMillis = millis();
      if (isRedBlinking) {
        analogWrite(redLedPin, 0); // Matikan lampu Merah
      } else {
        analogWrite(redLedPin, map(temperatureC, 40, 45, 0, 255)); // Nyalakan lampu Merah
      }
      isRedBlinking = !isRedBlinking; // Ubah status efek kedip
    }
  } else if (temperatureC >= 35.0 && temperatureC < 40.0) {
    // Rentang suhu ruangan server 35°C hingga 40°C: Nyalakan lampu Kuning
    digitalWrite(redLedPin, LOW);
    digitalWrite(blueLedPin, LOW);
    // Efek fading pada lampu Kuning
    int brightness = map(temperatureC, 35, 40, 255, 0);
    analogWrite(yellowLedPin, brightness);
    // Efek kedip pada lampu Kuning
    if (millis() - previousMillis >= interval) {
      previousMillis = millis();
      if (isYellowBlinking) {
        analogWrite(yellowLedPin, 0); // Matikan lampu Kuning
      } else {
        analogWrite(yellowLedPin, brightness); // Nyalakan lampu Kuning
      }
      isYellowBlinking = !isYellowBlinking; // Ubah status efek kedip
    }
  } else if (temperatureC < 35.0) {
    // Suhu ruangan server kurang dari 35°C: Nyalakan lampu Biru
    digitalWrite(redLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    // Efek fading pada lampu Biru
    int brightness = map(temperatureC, 0, 30, 0, 255);
    analogWrite(blueLedPin, brightness);
    // Efek kedip pada lampu Biru
    if (millis() - previousMillis >= interval) {
      previousMillis = millis();
      if (isBlueBlinking) {
        analogWrite(blueLedPin, 0); // Matikan lampu Biru
      } else {
        analogWrite(blueLedPin, brightness); // Nyalakan lampu Biru
      }
      isBlueBlinking = !isBlueBlinking; // Ubah status efek kedip
    }
  } else {
    // Suhu di atas 30°C: Matikan semua lampu
    digitalWrite(redLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(blueLedPin, LOW);
    // Hentikan efek kedip pada semua lampu
    isRedBlinking = false;
    isYellowBlinking = false;
    isBlueBlinking = false;
  }

  Serial.print("Suhu (Celsius): ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  delay(1000);
}