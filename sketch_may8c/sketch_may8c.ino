#include </Users/dumitru.bianca/Desktop/PM/CSGO-Inspired-Airsoft-Bomb/sketch_may8c/utils.h>

// Functii specifice pentru game_mode_2
void readSensor1() {
  red = getRedPW(1);
  redValue = map(red, redMin, redMax, 255, 0); // min la 255 si max la 0 (pwm invers la frequency)

  blue = getBluePW(1);
  blueValue = map(blue, blueMin, blueMax, 255, 0);

  green = getGreenPW(1);
  greenValue = map(green, greenMin, greenMax, 255, 0);
}

void readSensor2() {
  red2 = getRedPW(2);
  redValue2 = map(red2, redMin2, redMax2, 255, 0); // min la 255 si max la 0 (pwm invers la frequency)

  blue2 = getBluePW(2);
  blueValue2 = map(blue2, blueMin2, blueMax2, 255, 0);

  green2 = getGreenPW(2);
  greenValue2 = map(green2, greenMin2, greenMax2, 255, 0);
}

int verifyKey1() {
  // Verificare cheie introdusa corect
  if (redValue > blueValue && redValue > greenValue
      && (redValue - blueValue) >= offsetSensor && (redValue - greenValue) >= offsetSensor) {
    return 1;
  }
  // verificare cheie introdusa incorect
  if (blueValue > redValue && blueValue > greenValue
      && (blueValue - redValue) >= offsetSensor && (blueValue - greenValue) >= offsetSensor) {
    return 2;
  }
  return 0;
}

int verifyKey2() {
  // Verificare cheie introdusa corect
  if (blueValue2 > redValue2 && blueValue2 > greenValue2
      && (blueValue2 - redValue2) >= offsetSensor && (blueValue2 - greenValue2) >= offsetSensor) {
    return 1;
  }
  // Verificare cheie introdusa incorect
  if (redValue2 > blueValue2 && redValue2 > greenValue2
      && (redValue2 - blueValue2) >= offsetSensor && (redValue2 - greenValue2) >= offsetSensor) {
    return 2;
  }
  return 0;
}
int getRedPW(int type) {
  int pw;
  if (type == 1) {
    digitalWrite(S21, LOW);
    digitalWrite(S31, LOW);
    pw = pulseIn(output1, LOW);
  } else {
    digitalWrite(S22, LOW);
    digitalWrite(S32, LOW);
    pw = pulseIn(output2, LOW);
  } 
  return pw;
}

int getGreenPW(int type) {
  int pw;
  if (type == 1) {
    digitalWrite(S21, HIGH);
    digitalWrite(S31, HIGH);
    pw = pulseIn(output1, LOW);
  } else {
    digitalWrite(S22, HIGH);
    digitalWrite(S32, HIGH);
    pw = pulseIn(output2, LOW);
  }
  return pw;
}

int getBluePW(int type) {
  int pw;
  if (type == 1) {
    digitalWrite(S21, LOW);
    digitalWrite(S31, HIGH);
    pw = pulseIn(output1, LOW);
  } else {
    digitalWrite(S22, LOW);
    digitalWrite(S32, HIGH);
    pw = pulseIn(output2, LOW);
  }
  return pw;
}


// Functii specifice pentru ambele moduri
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
void displaySetTimerStart() {
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

void displayIntroduceKeys(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("!INTRODUCE KEYS!");
  lcd.setCursor(0, 1);
  lcd.print("TIMER:");
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
  for (int i = 0; i < 1000; i++) {
  tone(buzzer, 2000);
  delay(50);
  noTone(buzzer);

  tone(buzzer, 2500);
  delay(50);
  noTone(buzzer);

  tone(buzzer, 3000);
  delay(100);
  noTone(buzzer);

  delay(200);
  }
}

void displayArmingBomb() {
  displayStartBomb();
  while (1) {
    char key = customKeypad.getKey();
    if (key == 'A') {
      beep(500);
      blink_led_green();
      break;
    }
  }

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("BOMB ARMED!");
  for (int i = 200; i <= 1200; i += 100) {
    tone(buzzer, i); 
    delay(50); 
  }
  delay(100);

  // Oprește tonul pentru o scurtă pauză
  noTone(buzzer);
  lcd.setCursor(4, 1);
  print_timer_start();
  digitalWrite(led_pin2, LOW);
}

void error_code_sound() {
  for (int i = 1000; i <= 2000; i += 500) {
    tone(buzzer, i);
    delay(50);
    noTone(buzzer);
    delay(50);
}

void displayBombDefused() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("BOMB DEFUSED");
  lcd.setCursor(0, 1);
  lcd.print("!!!!!CS WIN!!!!!");
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
  noTone(buzzer);
  digitalWrite(led_pin2, HIGH);
}

void decrementTime() {
  unsigned long currentMillis = millis(); 
  if (currentMillis - previousMillis >= interval) {
    // Actualizați timpul anterior
    previousMillis = currentMillis;

    if (seconds > 0) {
      seconds--;
    } else {
      if (minutes > 0) {
        minutes--;
        seconds = 59;
      } else {
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
    beep(2000);
  }
}


void game_mode_1() {
  displaySetTimerStart();
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

  displayArmingBomb();

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
  displaySetTimerStart();
  displayStartBomb();
  displayArmingBomb();
  displayIntroduceKeys();
  while (true) {
    readSensor1();
    readSensor2();
    decrementTime();
    if (minutes == 0 && seconds == 0) {
      displayBombExplode();
      break;
    }
    if (verifyKey1() == 1 && verifyKey2() == 1) {
      displayBombDefused();
      break;
    }
    if (verifyKey1() == 2 || verifyKey2() == 2) {
      displayBombExplode();
      break;
    }
  }
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(S01, OUTPUT);
  pinMode(S11, OUTPUT);
  pinMode(S21, OUTPUT);
  pinMode(S31, OUTPUT);
  pinMode(output1, INPUT);
  pinMode(S02, OUTPUT);
  pinMode(S12, OUTPUT);
  pinMode(S22, OUTPUT);
  pinMode(S32, OUTPUT);
  pinMode(output2, INPUT);

  digitalWrite(led_pin2, HIGH);
  digitalWrite(S01, HIGH);
  digitalWrite(S11, LOW);
  digitalWrite(S02, HIGH);
  digitalWrite(S12, HIGH);
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
}