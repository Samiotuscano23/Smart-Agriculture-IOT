#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <DHT.h>

// Define LCD I2C address and pin setup for ESP8266
LiquidCrystal_PCF8574 lcd(0x27);  // Adjust the address as needed (0x27 or 0x3F)

// Define DHT11 Sensor Pin and Type
#define DHTPIN D3    // DHT11 Data pin connected to D3 (GPIO0)
#define DHTTYPE DHT11  // DHT 11 sensor

// Define Soil Moisture Sensor Pin
#define SOIL_MOISTURE_PIN A0  // Soil moisture sensor connected to A0

// Define LDR Sensor Pin
#define LDR_PIN D0  // LDR sensor connected to D0

// Define Motor and Buzzer Pins
#define MOTOR_PIN D5  // Motor connected to D5 for water pump
#define BUZZER_PIN D4 // Buzzer connected to D4

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);  // Set baud rate to 115200 for Serial Monitor
  Wire.begin(D2, D1);    // SDA -> D2 (GPIO4), SCL -> D1 (GPIO5)
  lcd.begin(16, 2);      // Initialize LCD with 16 columns and 2 rows
  lcd.setBacklight(255); // Turn on the backlight at full brightness

  pinMode(MOTOR_PIN, OUTPUT);  // Set motor pin as output
  pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as output

  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  Serial.println("DHT11 Sensor, Soil Moisture Sensor, LDR Sensor, Motor, Buzzer, and LCD Initialized");
  dht.begin();           // Initialize the DHT11 sensor
}

void loop() {
  // Read temperature and humidity values from DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Read temperature in Celsius
  
  // Check if any readings failed
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DHT Error!");
    return;
  }

  // Read soil moisture value (analog value)
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  int soilMoisturePercent = map(soilMoistureValue, 1023, 0, 0, 100); // Convert to percentage

  // Read LDR value (analog value)
  int ldrValue = analogRead(LDR_PIN);
  int ldrPercent = map(ldrValue, 1023, 0, 0, 100);  // Convert to percentage

  // Check if soil moisture is below 50% to turn on motor and buzzer
  if (soilMoisturePercent < 50) {
    digitalWrite(MOTOR_PIN, HIGH);  // Turn on water pump
    digitalWrite(BUZZER_PIN, HIGH); // Turn on buzzer
  } else {
    digitalWrite(MOTOR_PIN, LOW);   // Turn off water pump
    digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer
  }

  // Print temperature, humidity, soil moisture, and LDR value on Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");

  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisturePercent);
  Serial.println(" %\t");

  Serial.print("LDR Light: ");
  Serial.print(ldrPercent);
  Serial.println(" %");

  // Display the same on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);
  lcd.print(" H:");
  lcd.print(h);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(soilMoisturePercent);
  lcd.print(" % L:");
  lcd.print(ldrPercent);
  lcd.print(" %");

  // Add a delay before taking the next reading
  delay(2000);  // Wait for 2 seconds before next read
}
