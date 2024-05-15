#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Adresa I2C a display-ului LCD
#define LCD_ADDRESS 0x27

// Numărul de coloane și rânduri ale display-ului LCD
#define LCD_COLUMNS 16
#define LCD_ROWS 2
#define led_pin1 23 // red
#define led_pin2 25 // green
#define buzzer 27
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

//TO DO: de verificat valorile sub 10!!
// Definirea pinilor pentru linii și coloane
const byte ROWS = 4;
const byte COLS = 4;
byte rowPins[ROWS] = {30, 32, 34, 36}; // Linii conectate la pinii digitali 6, 7, 8, 9
byte colPins[COLS] = {22, 24, 26, 28}; // Coloane conectate la pinii digitali 2, 3, 4, 5
unsigned long previousMillis = 0; // Variabilă pentru stocarea timpului anterior
unsigned long interval = 1000; // Intervalul de timp în milisecunde
int tone_buzzer_timer = 2000;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

enum Type {
  CODE,
  TIME
};

int minutes, seconds = 0;
char code[4] = {'\0', '\0', '\0', '\0'};
// Crearea obiectului Keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Creăm un obiect pentru controlul display-ului LCD
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS); 

void beep(int value) {
  tone(buzzer, value);
  delay(100);
  noTone(buzzer);
}

void blink_led_green() {
  digitalWrite(led_pin2, LOW);
  delay(100);
  digitalWrite(led_pin2, HIGH);
}

void blink_led_green_code() {
  digitalWrite(led_pin2, HIGH);
  delay(100);
  digitalWrite(led_pin2, LOW);
}

void blink_led_red() {
  digitalWrite(led_pin1, HIGH);
  delay(100);
  digitalWrite(led_pin1, LOW);
}

void turn_off_green() {
  digitalWrite(led_pin2, LOW);
}


void displayInitialScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CHOOSE GAME MODE");
  lcd.setCursor(0, 1);
  lcd.print("PRESS 1 OR 2");
}

void displayErrorMessage() {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("ERROR!");
  blink_led_green();
  tone(buzzer, 100);
  delay(100);
  noTone(buzzer);
}

bool displayRetypeMessage(Type t) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Retype?");
  lcd.setCursor(8, 0);
  if (t == TIME) {
    char minStr[3];
    char secStr[3];
    
    sprintf(minStr, "%02d", minutes);
    sprintf(secStr, "%02d", seconds);
    lcd.print(minStr);
    lcd.print(":");
    lcd.print(secStr);
  } else if (t == CODE) {
    lcd.print(code);
  }
  lcd.setCursor(0, 1);
  lcd.print("1-yes or 2-no");
  while (1) {
    char key = customKeypad.getKey();
    if (key) {
      beep(500);
      blink_led_green();
      if (key == '1') {
        return true;
      }
      if (key == '2') {
        return false;
      }
    }
  }
}

void displayIntroduceCode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TYPE THE CODE:");
  lcd.setCursor(3, 1);
}

void displaySetTimer() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SET TIMER");
  lcd.setCursor(7, 1);
  lcd.print(":");
  lcd.setCursor(4, 1);
}

void print_timer_start() {
  char minStr[3];
  char secStr[3];
  
  sprintf(minStr, "%02d", minutes);
  sprintf(secStr, "%02d", seconds);

  lcd.print(minStr[0]);
  lcd.setCursor(5, 1);
  lcd.print(minStr[1]);
  lcd.setCursor(7, 1);
  lcd.print(":");
  lcd.setCursor(9, 1);
  lcd.print(secStr[0]);
  lcd.setCursor(10, 1);
  lcd.print(secStr[1]);
}

void displayStartBomb() {
  lcd.clear();
  lcd.noBlink();
  lcd.setCursor(0, 0);
  lcd.print("PRESS A TO START");
  lcd.setCursor(4, 1);
  print_timer_start();
}

void displayBombExplode() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("BOMB EXPLODED");
  lcd.setCursor(0, 1);
  lcd.print("!TERRORISTS WIN!");
  for (int i = 0; i < 20; i++) {
   tone(buzzer, 2000); // Ton de bază
  delay(50); // Pauză mică
  noTone(buzzer); // Oprim tonul

  tone(buzzer, 2500); // Ton de înaltă frecvență
  delay(50); // Pauză mică
  noTone(buzzer); // Oprim tonul

  tone(buzzer, 3000); // Ton și mai înalt de înaltă frecvență
  delay(100); // Pauză mică
  noTone(buzzer); // Oprim tonul

  delay(200); // Pauză între explozii
  }
}

void displayBombArmed() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("BOMB ARMED!");
  // // Sunetul de armare a bombei
  for (int i = 200; i <= 1200; i += 100) {
    tone(buzzer, i); // Emite un ton la frecvența i Hz
    delay(50); // Pauză mică între tonuri pentru a crea efectul de crescendo
  }

  // Pauză între cicluri
  delay(100);

  // Oprește tonul pentru o scurtă pauză
  noTone(buzzer);
  lcd.setCursor(4, 1);
  print_timer_start();
  digitalWrite(led_pin2, LOW);
}

void error_code_sound() {
  // Secvența de tonuri pentru semnalizarea unei erori de introducere a codului
  for (int i = 1000; i <= 2000; i += 500) {
    tone(buzzer, i); // Emite un ton la frecvența i Hz
    delay(50); // Pauză între tonuri
    noTone(buzzer); // Oprește tonul
    delay(50); // Pauză între tonuri pentru a crea efectul auditiv
  }
}

void displayBombDefused() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("BOMB DEFUSED");
  lcd.setCursor(0, 1);
  lcd.print("!!!!!CS WIN!!!!!");
  // Secvența de tonuri crescătoare
  for (int i = 100; i <= 1000; i += 50) {
    tone(buzzer, i);
    delay(10);
  }
  noTone(buzzer);
  delay(100);

  for (int i = 1000; i >= 100; i -= 50) {
    tone(buzzer, i); // Emite un ton la frecvența i Hz
    delay(10);
  }
  // Oprește tonul pentru o pauză mai lungă
  noTone(buzzer);
  digitalWrite(led_pin2, HIGH);
}

void decrementTime() {
  unsigned long currentMillis = millis(); // Obțineți timpul actual în milisecunde

  // Verificați dacă trebuie să scădeți timpul
  if (currentMillis - previousMillis >= interval) {
    // Actualizați timpul anterior
    previousMillis = currentMillis;

    // Scadeți timpul
    if (seconds > 0) {
      seconds--;
    } else {
      if (minutes > 0) {
        minutes--;
        seconds = 59;
      } else {
        // Dacă timerul a expirat, opriți beep-ul și ieșiți din buclă
        return;
      }
    }

    // Afisati timpul pe LCD
    lcd.setCursor(8, 1);
    lcd.print("|");
    lcd.setCursor(9, 1);
    if (minutes < 10) {
      lcd.print(0);
      lcd.print(minutes);
    } else {
      lcd.print(minutes);
    }
    lcd.setCursor(12, 1);
    lcd.print(":");
    lcd.setCursor(14, 1);
    if (seconds < 10) {
      lcd.print(0);
      lcd.print(seconds);
    } else {
      lcd.print(seconds);
    }
    blink_led_red();
    //beep(2000);
  }
}



void game_mode_1() {
  Serial.println("1!");
  displaySetTimer();
  bool setTime = true;
  int pos = 0;
  bool retype = false;
  while (setTime) {
    lcd.blink();

    char key = customKeypad.getKey();
    
    if (key && key >= '0' && key <= '9') {
      beep(500);
      blink_led_green();
      if (pos == 0) {
        int ch = key - '0';
        minutes = 10 * ch;
        lcd.print(key);
        lcd.setCursor(5, 1);
      } else if (pos == 1) {
        minutes += key - '0';
        lcd.print(key);
        lcd.setCursor(9, 1);
      } else if (pos == 2) {
        seconds = 10 * (key - '0');
        lcd.print(key);
        lcd.setCursor(10, 1);
      } else if (pos == 3) {
        seconds += (key - '0');
        lcd.print(key);
        delay(500);
        if (displayRetypeMessage(TIME)) {
          minutes = 0;
          seconds = 0;
          retype = true;
          displaySetTimer();
        } else {
          setTime = false;
          break;
        }
      }
      if (retype) {
        pos = 0;
        retype = false;
      } else {
        pos++;
      }
    }
  }
  Serial.println(minutes);
  Serial.println(seconds);

  displayIntroduceCode();
  int nr_digits = 0;
  int code_pos = 3;
  while (nr_digits < 4) {
    lcd.blink();
    char key = customKeypad.getKey();

    if (key) {
      beep(500);
      blink_led_green();
      code[nr_digits] = key;
      lcd.print(key);
      if (nr_digits == 3) {
        delay(500);
        if (displayRetypeMessage(CODE)) {
          for (int i = 0; i < 4; i++) {
            code[i] = '\0';
          }
          nr_digits = 0;
          code_pos = 3;
          displayIntroduceCode();
        }
      } else {
        code_pos += 2;
        lcd.setCursor(code_pos, 1);
      }
      nr_digits++;
    }
  }

  Serial.println(code);
  displayStartBomb();

  while (1) {
    char key = customKeypad.getKey();
    if (key == 'A') {
      beep(500);
      blink_led_green();
      break;
    }
  }
  displayBombArmed();
  turn_off_green();
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CODE: ");
  lcd.setCursor(0, 1);
  lcd.print("TIMER:");
  int pos_code = 0;
  char input_code[4] = {'\0', '\0', '\0', '\0'};
  bool correct_code = true;
  while (true) {
    // Verificăm și gestionăm decrementarea timpului
    decrementTime();
    if (minutes == 0 && seconds == 0) {
      displayBombExplode();
    }
    char key = customKeypad.getKey();
    // Verificăm dacă s-au introdus date de la tastatură
    if (key) {
      beep(1000);
      blink_led_green_code();
      lcd.setCursor(6 + pos_code, 0);
      lcd.print("*");
      // Citim codul introdus
      input_code[pos_code] = key;
      if (pos_code == 3) {
        for (int i = 0 ; i < 4; i++) {
          if (input_code[i] != code[i]) {
            correct_code = false;
          }
        }
          // Dacă codul este corect, oprim timerul
        if (correct_code) {
          displayBombDefused();
          break; // Ieșim din buclă pentru a termina jocul
        } else {
          error_code_sound();
          // Dacă codul este incorect, scădem timpul mai rapid
          if (interval > 300) {
            interval -= 300;
          }
          if (interval == 300) {
              interval = 150;
          }
          for (int i = 0; i < 4; i ++) {
            input_code[i] = '\0';
            lcd.setCursor(i + 6, 0);
            lcd.print(' ');
          }
          pos_code = 0;
        }
        correct_code = true;
      } else {
        pos_code++;
      }
    }
  }
}

void game_mode_2() {

}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  // pinMode(S01, OUTPUT);
  // pinMode(S11, OUTPUT);
  // pinMode(S21, OUTPUT);
  // pinMode(S31, OUTPUT);
  // pinMode(output1, INPUT);
  // pinMode(S02, OUTPUT);
  // pinMode(S12, OUTPUT);
  // pinMode(S22, OUTPUT);
  // pinMode(S32, OUTPUT);
  // pinMode(output2, INPUT);

  digitalWrite(led_pin2, HIGH);
  // digitalWrite(S01, HIGH);
  // digitalWrite(S11, HIGH);
  // digitalWrite(S02, HIGH);
  // digitalWrite(S12, HIGH);
  displayInitialScreen();
}

void loop() {
  char key = customKeypad.getKey();
  if (key) {
    beep(500);
    blink_led_green();
    if (key == '1') {
      game_mode_1();
    } else if (key == '2') {
      game_mode_2();
    } else {
      displayErrorMessage();
      lcd.setCursor(0, 1);
      lcd.print("PRESS 1 OR 2");
      delay(2000);
      displayInitialScreen();
    }
  }
  // int frequency;
  // // red filter
  // digitalWrite(S21, LOW);
  // digitalWrite(S31, LOW);

  // //reading output
  // frequency = pulseIn(output1, LOW);
  // // frequency = map(frequency, 25, 70, 255, 0);
  // //print value
  // Serial.print("R=");
  // Serial.print(frequency);
  // Serial.print(" ");

  // //green
  // digitalWrite(S21, HIGH);
  // digitalWrite(S31, HIGH);

  // //reading output
  // frequency = pulseIn(output1, LOW);
  // //print value
  // Serial.print("G=");
  // Serial.print(frequency);
  // Serial.print(" ");

  // //blue
  // digitalWrite(S21, LOW);
  // digitalWrite(S31, HIGH);

  // //reading output
  // frequency = pulseIn(output1, LOW);
  // //print value
  // Serial.print("B=");
  // Serial.print(frequency);
  // Serial.println(" ");
  // Serial.println();
  // delay(100);

  // int frequency2;
  // // red filter
  // digitalWrite(S22, LOW);
  // digitalWrite(S32, LOW);

  // //reading output
  // frequency = pulseIn(output2, LOW);
  // // frequency = map(frequency, 25, 70, 255, 0);
  // //print value
  // Serial.print("R2=");
  // Serial.print(frequency2);
  // Serial.print(" ");

  // //green
  // digitalWrite(S22, HIGH);
  // digitalWrite(S32, HIGH);

  // //reading output
  // frequency = pulseIn(output2, LOW);
  // //print value
  // Serial.print("G2=");
  // Serial.print(frequency2);
  // Serial.print(" ");

  // //blue
  // digitalWrite(S22, LOW);
  // digitalWrite(S32, HIGH);

  // //reading output
  // frequency = pulseIn(output2, LOW);
  // //print value
  // Serial.print("B2=");
  // Serial.print(frequency2);
  // Serial.println(" ");
  // Serial.println();
  // delay(100);
}