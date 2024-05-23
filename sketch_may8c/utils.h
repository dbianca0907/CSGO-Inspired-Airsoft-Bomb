#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <PCM.h>

// Adresa I2C a display-ului LCD
#define LCD_ADDRESS 0x27

// Numărul de coloane și rânduri ale display-ului LCD
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Pinii pentru leduri si buzzer
#define led_pin1 23 // red
#define led_pin2 25 // green
#define buzzer 27

// Pinii senzorilor
#define S01 13
#define S11 12
#define S21 10
#define S31 9
#define output1 11

#define S02 7
#define S12 6
#define S22 4
#define S32 3
#define output2 5

// Creăm un obiect pentru controlul display-ului LCD
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS); 

// valori ale variabilelor pentru senzorii de culoare
int red = 0;
int green = 0;
int blue = 0;

int redMin = 54;
int redMax = 247;
int blueMin = 58;
int blueMax = 318;
int greenMin = 69;
int greenMax = 353;

int redValue;
int greenValue;
int blueValue;

int red2 = 0;
int green2 = 0;
int blue2 = 0;

int redMin2 = 10;
int redMax2 = 60;
int blueMin2 = 12;
int blueMax2 = 76;
int greenMin2 = 7;
int greenMax2 = 84;

int redValue2;
int greenValue2;
int blueValue2;

int offsetSensor = 50;

// variabile pentru contorizarea timpului si a codului introdus
unsigned long previousMillis = 0; // Variabilă pentru stocarea timpului anterior
unsigned long interval = 1000; // Intervalul de timp în milisecunde
int tone_buzzer_timer = 2000;
enum Type {
  CODE,
  TIME
};
int minutes, seconds = 0;
char code[4] = {'\0', '\0', '\0', '\0'};

// Controlul tastaturii
const byte ROWS = 4;
const byte COLS = 4;
byte rowPins[ROWS] = {30, 32, 34, 36};
byte colPins[COLS] = {22, 24, 26, 28};

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, 4, 3);
