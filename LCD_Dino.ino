
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonPin = 2;

// Custom dinosaur character
byte dino[8] = {
  B00111,
  B00101,
  B00111,
  B10110,
  B11111,
  B01110,
  B01010,
  B01010
};

// Custom cactus character
byte cactus[8] = {
  B00100,
  B00101,
  B10101,
  B10101,
  B11101,
  B00111,
  B00100,
  B00100
};

int obstaclePos = 15;
int score = 0;

bool jumping = false;
bool gameOver = false;

unsigned long jumpStart = 0;
unsigned long lastMove = 0;

const unsigned long jumpDuration = 600;
const unsigned long moveInterval = 300;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, dino);
  lcd.createChar(1, cactus);

  lcd.setCursor(2, 0);
  lcd.print("DINO GAME");
  lcd.setCursor(1, 1);
  lcd.print("Press to start");

  while (digitalRead(buttonPin) == HIGH) {
    // Wait for button press
  }

  delay(200);
  lcd.clear();
  lastMove = millis();
}

void loop() {
  if (gameOver) {
    if (digitalRead(buttonPin) == LOW) {
      delay(200);

      obstaclePos = 15;
      score = 0;
      jumping = false;
      gameOver = false;

      lcd.clear();
      lastMove = millis();
    }
    return;
  }

  // Button press makes the dinosaur jump
  if (digitalRead(buttonPin) == LOW && !jumping) {
    jumping = true;
    jumpStart = millis();
  }

  // Finish the jump after a short time
  if (jumping && millis() - jumpStart >= jumpDuration) {
    jumping = false;
  }

  // Move the cactus
  if (millis() - lastMove >= moveInterval) {
    lastMove = millis();

    lcd.setCursor(obstaclePos, 1);
    lcd.print(" ");

    obstaclePos--;

    if (obstaclePos < 0) {
      obstaclePos = 15;
      score++;
    }
  }

  // Collision: cactus reaches the dinosaur
  if (obstaclePos == 1 && !jumping) {
    gameOver = true;

    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("GAME OVER");
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(score);
    return;
  }

  // Draw the dinosaur
  lcd.setCursor(1, jumping ? 0 : 1);
  lcd.write(byte(0));

  // Erase the dinosaur's previous position
  lcd.setCursor(1, jumping ? 1 : 0);
  lcd.print(" ");

  // Draw the cactus
  if (obstaclePos >= 0 && obstaclePos < 16) {
    lcd.setCursor(obstaclePos, 1);
    lcd.write(byte(1));
  }

  // Display score
  lcd.setCursor(9, 0);
  lcd.print("Score:");
  lcd.print(score);
  lcd.print(" ");
}
