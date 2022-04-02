#include <M5StickCPlus.h>

#define LED_PIN 10 // G10

void setup() {
    M5.begin();
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // M5.update();

    digitalWrite(LED_PIN, LOW); // LED点灯
    delay(500);
    digitalWrite(LED_PIN, HIGH); // LED消灯
    delay(500);

}
