#include <Arduino.h>

const uint8_t pins[] = {26, 25, 19, 18, 17, 16};
const int numPins = sizeof(pins) / sizeof(pins[0]);
const uint8_t pwmChannels[] = {0, 1, 2, 3, 4, 5};

const int buttonPin = 23;
const int potPin = 34;

const int pwmFrequency = 5000;
const int pwmResolution = 8;

bool running = true;
bool lastButtonState = HIGH;

unsigned long lastStepTime = 0;
const unsigned long stepDelay = 50;

int currentIndex = 0;

void setAllLedBrightness(uint8_t brightness)
{
    for (int i = 0; i < numPins; i++)
    {
        ledcWrite(pwmChannels[i], 0);
    }

    ledcWrite(pwmChannels[currentIndex], brightness);
}

void setup()
{
    Serial.begin(115200);

    for (int i = 0; i < numPins; i++)
    {
        ledcSetup(pwmChannels[i], pwmFrequency, pwmResolution);
        ledcAttachPin(pins[i], pwmChannels[i]);
        ledcWrite(pwmChannels[i], 0);
    }

    pinMode(buttonPin, INPUT_PULLUP);
}

void loop()
{
    bool currentButtonState = digitalRead(buttonPin);

    if (lastButtonState == HIGH && currentButtonState == LOW)
    {
        running = !running; // toggle
        Serial.println(running ? "STARTED" : "STOPPED");
    }

    lastButtonState = currentButtonState;

    int potValue = analogRead(potPin);
    uint8_t brightness = map(potValue, 0, 4095, 0, 255);

    if (running && millis() - lastStepTime >= stepDelay)
    {
        lastStepTime = millis();

        setAllLedBrightness(brightness);

        Serial.print("Pin ");
        Serial.print(pins[currentIndex]);
        Serial.print(" brightness ");
        Serial.println(brightness);

        currentIndex++;
        if (currentIndex >= numPins)
        {
            currentIndex = 0;
        }
    }
}