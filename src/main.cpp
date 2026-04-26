#include <Arduino.h>

const uint8_t pins[] = {25, 26, 27, 16, 17, 18};
const int numPins = sizeof(pins) / sizeof(pins[0]);

const int buttonPin = 23;
const int potPin = 34;

const int pwmFrequency = 5000;
const int pwmResolution = 8;

bool running = true;
bool lastButtonState = HIGH;
unsigned long lastStepTime = 0;
const unsigned long stepDelay = 100;
int currentIndex = 0;

void setAllLedBrightness(uint8_t brightness)
{
    for (int i = 0; i < numPins; i++)
    {
        ledcWrite(pins[i], 0);
    }

    ledcWrite(pins[currentIndex], brightness);
}

void setup()
{
    Serial.begin(115200);

    Serial.print("ESP32 Arduino Core Version: ");
    Serial.println(ESP_ARDUINO_VERSION_STR);

    for (int i = 0; i < numPins; i++)
    {
        if (!ledcAttach(pins[i], pwmFrequency, pwmResolution))
        {
            Serial.printf("Failed to attach PWM to pin %d\n", pins[i]);
        }

        ledcWrite(pins[i], 0);
    }

    pinMode(buttonPin, INPUT_PULLUP);
}

void loop()
{
    bool currentButtonState = digitalRead(buttonPin);

    if (lastButtonState == HIGH && currentButtonState == LOW)
    {
        running = !running;
        Serial.println(running ? "--- STARTED ---" : "--- STOPPED ---");
        delay(50);
    }
    lastButtonState = currentButtonState;

    int potValue = analogRead(potPin);
    uint8_t brightness = map(potValue, 0, 4095, 0, 255);

    if (running && (millis() - lastStepTime >= stepDelay))
    {
        lastStepTime = millis();

        setAllLedBrightness(brightness);

        Serial.printf("Active Pin: %d | Brightness: %d\n", pins[currentIndex], brightness);

        currentIndex++;
        if (currentIndex >= numPins)
        {
            currentIndex = 0;
        }
    }
}