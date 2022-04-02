#include <M5StickCPlus.h>

const int button_A = 37;
const int sharpLEDPin = 26;
const int sharpVoPin = 36;

#define N 100
static unsigned long VoRawTotal = 0;
static int VoRawCount = 0;

// Set the typical output voltage in Volts when there is zero dust.
static float Voc = 0.6;

// Use the typical sensitivity in units of V per 100ug/m3.
const float k = 0.5;

void resetHandler() {
  //Serial.println("resetHandler");
  esp_restart();
}

void setup() {
    M5.begin();
    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(10, M5.Lcd.height()/2 - 20);
    M5.Lcd.print("Hello\n M5StickCPlus");
    //Serial.begin(115200);

    pinMode(sharpLEDPin, OUTPUT);
    pinMode(button_A, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(button_A), resetHandler, FALLING);
    delay(2000);
    M5.Lcd.fillScreen(BLACK);
    delay(500);
}

void loop() {

  // Trun on the dust sensor LED by setting digital pin LOW.
  digitalWrite(sharpLEDPin, LOW);

  // Wait 0.28ms before taking a reading of the output voltage as per spec.
  delayMicroseconds(280);

  // Record the output voltage. This operation takes around 100 microseconds.
  int VoRaw = analogRead(sharpVoPin);

  // Turn the dust sensor LED off by setting digital pin HIGH.
  digitalWrite(sharpLEDPin, HIGH);

  // Wait for remainder of the 10ms cycle = 10000 - 280 - 100 microseconds.
  delayMicroseconds(9620);

  float Vo = VoRaw;
  // Calculate average.
  VoRawTotal += VoRaw;
  VoRawCount++;
  if( VoRawCount >= N ) {
    Vo = 1.0 * VoRawTotal / N;
    VoRawCount = 0;
    VoRawTotal = 0;
  } else {
    return;
  }

  // Compute the output voltage in Volts.
  Vo = Vo / 1024.0 * 5.0;
  //print("Vo", Vo*1000.0, "mV");

  // Convert to Dust Density in units of ug/m3.
  float dV = Vo - Voc;
  if( dV < 0 ) {
    dV = 0;
    Voc = Vo;
  }
  float dustDensity = dV / k * 100.0;

  // Lcd
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(10, M5.Lcd.height()*2/3 - 10);
  M5.Lcd.printf("%6.2f mg/m3", dustDensity);
  M5.Lcd.setCursor(10, M5.Lcd.height()/3 - 10);
  M5.Lcd.printf("Dust Density");

}
