/*******************************************************************************
 * COMMENTS:
 * From internet reverse engineering:
 *
 * https://www.waitingforfriday.com/?p=586
 *
 * Green  - 415 Hz - G#4 (true pitch 415.305 Hz)
 * Red    - 310 Hz - D#4 (true pitch 311.127 Hz)
 * Yellow - 252 Hz - B3  (true pitch 247.942 Hz)
 * Blue   - 209 Hz - G#3 (true pitch 207.652 Hz)
 *
 *******************************************************************************/

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <Arduino.h>

/*******************************************************************************
 * DEFINES, MACROS, CONSTANTS
 ******************************************************************************/
// clang-format off
const int BLUE_LED_PIN      = 13;
const int GREEN_LED_PIN     = 12;
const int RED_LED_PIN       = 11;
const int YELLOW_LED_PIN    = 10;

const int BLUE_BUTTON_PIN   = 7;
const int GREEN_BUTTON_PIN  = 6;
const int RED_BUTTON_PIN    = 5;
const int YELLOW_BUTTON_PIN = 4;

const int BUZZER_PIN        = 3;
const int BLUE_TONE         = 415;
const int GREEN_TONE        = 310;
const int RED_TONE          = 252;
const int YELLOW_TONE       = 209;
const int TONE_LENGTH       = 100;

const long BAUD_RATE         = 9600;
// clang-format on

/*******************************************************************************
 * GLOBALS
 ******************************************************************************/

/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/

void setup()
{
    Serial.begin(BAUD_RATE);
    // initilize output pins
    pinMode(BLUE_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);

    pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
    if (!digitalRead(BLUE_BUTTON_PIN))
    {
        digitalWrite(BLUE_LED_PIN, HIGH);
        tone(BUZZER_PIN, BLUE_TONE, TONE_LENGTH);
        Serial.write("\nBLUE button pressed");
    }
    else
    {
        digitalWrite(BLUE_LED_PIN, LOW);
    }

    if (!digitalRead(GREEN_BUTTON_PIN))
    {
        digitalWrite(GREEN_LED_PIN, HIGH);
        tone(BUZZER_PIN, GREEN_TONE, TONE_LENGTH);
        Serial.write("\nGREEN button pressed");
    }
    else
    {
        digitalWrite(GREEN_LED_PIN, LOW);
    }

    if (!digitalRead(RED_BUTTON_PIN))
    {
        digitalWrite(RED_LED_PIN, HIGH);
        tone(BUZZER_PIN, RED_TONE, TONE_LENGTH);
        Serial.write("\nRED button pressed");
    }
    else
    {
        digitalWrite(RED_LED_PIN, LOW);
    }

    if (!digitalRead(YELLOW_BUTTON_PIN))
    {
        digitalWrite(YELLOW_LED_PIN, HIGH);
        tone(BUZZER_PIN, YELLOW_TONE, TONE_LENGTH);
        Serial.write("\nYELLOW button pressed");
    }
    else
    {
        digitalWrite(YELLOW_LED_PIN, LOW);
    }
}
