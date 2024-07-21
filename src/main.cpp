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
#include <stdlib.h>

/*******************************************************************************
 * DEFINES, MACROS, CONSTANTS
 ******************************************************************************/

// clang-format off
enum class LedPins
{
    blue                = 10,
    green               = 11,
    red                 = 12,
    yellow              = 13,
};
constexpr LedPins led_pin_order[] = {
    LedPins::blue,
    LedPins::green,
    LedPins::red,
    LedPins::yellow,
};

const int BLUE_BUTTON_PIN       = 4;
const int GREEN_BUTTON_PIN      = 5;
const int RED_BUTTON_PIN        = 6;
const int YELLOW_BUTTON_PIN     = 7;

const int BUZZER_PIN            = 3;
const int TEST_BUTTON_PIN       = 2;

const int BLUE_TONE             = 209;
const int GREEN_TONE            = 415;
const int RED_TONE              = 310;
const int YELLOW_TONE           = 252;

const int TONE_LENGTH           = 100;
const int DELAY_LENGTH          = 500;

struct ColorSettings {
    int pin;
    int tone;
    String name;
};

const ColorSettings blue        = {static_cast<int>(LedPins::blue), BLUE_TONE, "Blue"};
const ColorSettings green       = {static_cast<int>(LedPins::green), GREEN_TONE, "Green"};
const ColorSettings red         = {static_cast<int>(LedPins::red), RED_TONE, "Red"};
const ColorSettings yellow      = {static_cast<int>(LedPins::yellow), YELLOW_TONE, "Yellow"};
const ColorSettings colors[]    = {blue, green, red, yellow};

const int MAX_PATTERN_LENGTH    = 10;

const long BAUD_RATE            = 9600;
// clang-format on

/*******************************************************************************
 * GLOBALS
 ******************************************************************************/

int g_computer_pattern[MAX_PATTERN_LENGTH];
int g_player_pattern[MAX_PATTERN_LENGTH];
int g_computer_count = 0;
int g_player_count = 0;
int g_button_pressed = 0;
bool g_pattern_played = false;

/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/

void play_blue();
void play_green();
void play_red();
void play_yellow();
void play_color(ColorSettings color);

void add_to_game_pattern();
void add_to_player_pattern();
bool check_player_pattern();
void play_pattern();
void print_pattern();
void restart_game();

using ColorPointers = void (*)();

void setup()
{
    Serial.begin(BAUD_RATE);
    srand(analogRead(A0)); // Initialize the seed for rand() using noise from pin A0

    // initilize output pins
    pinMode(BUZZER_PIN, OUTPUT);

    for (auto pin : led_pin_order)
    {
        pinMode(static_cast<int>(pin), OUTPUT);
    }

    // Initialize game pattern
    add_to_game_pattern();
}

void loop()
{

    if (!digitalRead(BLUE_BUTTON_PIN))
    {
        play_color(blue);
        g_button_pressed = 1;
        add_to_player_pattern();
    }
    else if (!digitalRead(GREEN_BUTTON_PIN))
    {
        play_color(green);
        g_button_pressed = 2;
        add_to_player_pattern();
    }
    else if (!digitalRead(RED_BUTTON_PIN))
    {
        play_color(red);
        g_button_pressed = 3;
        add_to_player_pattern();
    }
    else if (!digitalRead(YELLOW_BUTTON_PIN))
    {
        play_color(yellow);
        g_button_pressed = 4;
        add_to_player_pattern();
    }

    if (!digitalRead(TEST_BUTTON_PIN))
    {
        Serial.print("\nTest button pressed! Pattern is: ");
        for (int i = 0; i < MAX_PATTERN_LENGTH; i++)
        {
            Serial.print(g_computer_pattern[i]);
            Serial.print(", ");
        }
        g_pattern_played = false;
    }

    if (!g_pattern_played)
    {
        g_pattern_played = true;
        delay(DELAY_LENGTH);
        play_pattern();
    }

    if (!check_player_pattern())
    {
        restart_game();
    }
    else
    {
        if (g_player_count == g_computer_count)
        {
            add_to_game_pattern();
            for (int i = 0; i < g_player_count; i++)
            {
                g_player_pattern[i] = 0;
            }
            g_player_count = 0;

            g_pattern_played = false;
        }
    }

    g_button_pressed = 0;
}

void restart_game()
{
    // for (auto color : colors)
    // {
    //     play_color(color);
    // }
    Serial.println("\nRESTARTING GAME **********");
    digitalWrite(static_cast<int>(LedPins::blue), HIGH);
    digitalWrite(static_cast<int>(LedPins::green), HIGH);
    digitalWrite(static_cast<int>(LedPins::red), HIGH);
    digitalWrite(static_cast<int>(LedPins::yellow), HIGH);
    tone(BUZZER_PIN, 500, TONE_LENGTH * 3);
    delay(DELAY_LENGTH * 3);
    digitalWrite(static_cast<int>(LedPins::blue), LOW);
    digitalWrite(static_cast<int>(LedPins::green), LOW);
    digitalWrite(static_cast<int>(LedPins::red), LOW);
    digitalWrite(static_cast<int>(LedPins::yellow), LOW);

    for (int i = 0; i < MAX_PATTERN_LENGTH; i++)
    {
        g_computer_pattern[i] = 0;
        g_player_pattern[i] = 0;
    }

    g_computer_count = 0;
    g_player_count = 0;
    g_button_pressed = 0;
    g_pattern_played = false;
    add_to_game_pattern();
    delay(DELAY_LENGTH * 2);
}

void add_to_game_pattern()
{
    g_computer_pattern[g_computer_count] = rand() % 4 + 1; // Generate a random number between 1 and 4
    g_computer_count++;
}

void add_to_player_pattern()
{
    g_player_pattern[g_player_count] = g_button_pressed;
    g_player_count++;
}

bool check_player_pattern()
{
    for (int i = 0; i < g_player_count; i++)
    {
        if (g_player_pattern[i] != g_computer_pattern[i])
        {
            return false;
        }
    }
    return true;
}

void print_pattern()
{
    Serial.print("\n       Pattern elements: ");
    for (int i = 0; i < MAX_PATTERN_LENGTH; i++)
    {
        Serial.print(g_computer_pattern[i]);
        if (i < MAX_PATTERN_LENGTH - 1)
        {
            Serial.print(", ");
        }
    }
    Serial.println();

    Serial.print("Player Pattern elements: ");
    for (int i = 0; i < MAX_PATTERN_LENGTH; i++)
    {
        Serial.print(g_player_pattern[i]);
        if (i < MAX_PATTERN_LENGTH - 1)
        {
            Serial.print(", ");
        }
    }
    Serial.println();
}

void play_pattern()
{
    ColorPointers colors[4] = {play_blue, play_green, play_red, play_yellow};
    for (int i = 0; i < MAX_PATTERN_LENGTH; i++)
    {
        if (g_computer_pattern[i] > 0)
        {
            colors[(g_computer_pattern[i] - 1)]();
        }
    }
}

void play_color(ColorSettings color)
{
    digitalWrite(color.pin, HIGH);
    tone(BUZZER_PIN, color.tone, TONE_LENGTH);
    Serial.print("\n");
    Serial.print(color.name);
    Serial.print(" button pressed");

    delay(DELAY_LENGTH);
    digitalWrite(color.pin, LOW);
}

void play_blue()
{
    digitalWrite(static_cast<int>(LedPins::blue), HIGH);
    tone(BUZZER_PIN, BLUE_TONE, TONE_LENGTH);
    Serial.write("\nBLUE button pressed");

    delay(DELAY_LENGTH);
    digitalWrite(static_cast<int>(LedPins::blue), LOW);
    delay(DELAY_LENGTH);
}

void play_green()
{
    digitalWrite(static_cast<int>(LedPins::green), HIGH);
    tone(BUZZER_PIN, GREEN_TONE, TONE_LENGTH);
    Serial.write("\nGREEN button pressed");

    delay(DELAY_LENGTH);
    digitalWrite(static_cast<int>(LedPins::green), LOW);
    delay(DELAY_LENGTH);
}

void play_red()
{
    digitalWrite(static_cast<int>(LedPins::red), HIGH);
    tone(BUZZER_PIN, RED_TONE, TONE_LENGTH);
    Serial.write("\nRED button pressed");

    delay(DELAY_LENGTH);
    digitalWrite(static_cast<int>(LedPins::red), LOW);
    delay(DELAY_LENGTH);
}

void play_yellow()
{
    digitalWrite(static_cast<int>(LedPins::yellow), HIGH);
    tone(BUZZER_PIN, YELLOW_TONE, TONE_LENGTH);
    Serial.write("\nYELLOW button pressed");

    delay(DELAY_LENGTH);
    digitalWrite(static_cast<int>(LedPins::yellow), LOW);
    delay(DELAY_LENGTH);
}
