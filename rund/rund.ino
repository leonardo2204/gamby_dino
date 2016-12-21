
#include<Gamby.h>
#include <avr/pgmspace.h>
#include "assets.h"

extern const long font[];
// general use string buffer
char text[16];

int cloudX;
int cactusX;
int positionY;
int cloudVelocityX;
int cactusVelX;
int velocityY;

const int gravity = 3;

int score;

boolean onGround;
boolean isAlive;

long timeToDraw;
long dinoSpeed;

boolean playing;

GambyGraphicsMode gamby;

void intro()
{
  gamby.drawSprite(10, 6, dino_top);
  gamby.drawSprite(10, 24, dino_leg_1);
  gamby.update();
  randomSeed(millis());
  gamby.setPos(0, 7);
  gamby.print("Press any button to begin!");
}

void setup()
{
  gamby.font = font;
  intro();

  waitForAnyButton();

  Serial.begin(9600);

  startGame();
}

void startGame() {
  dinoSpeed = 200;

  cloudX = 80;
  cactusX = 80;
  positionY = 40;
  cloudVelocityX = 1;
  cactusVelX = 4;
  velocityY = 0;

  score = 0;

  isAlive = true;
  onGround = true;

  timeToDraw = millis() + dinoSpeed;
  drawScreen();

  gamby.drawPattern = PATTERN_BLACK;

  playing = true;
}

void drawDino() {
  // dino
  gamby.drawSprite(8, positionY, dino_top);

  if (onGround) {
    if (cloudX % 2 == 0) {
      gamby.drawSprite(8, positionY + 18, dino_leg_1);
    } else {
      gamby.drawSprite(8, positionY + 18, dino_leg_2);
    }
  } else {
    gamby.drawSprite(8, positionY + 18, dino_leg_0);
  }

  gamby.update();
}

void drawCactus() {
  gamby.drawSprite(cactusX, 40, cactus_1);
  gamby.update();
}

void startJump() {
  if (onGround)
  {
    velocityY = -14.0;
    onGround = false;
  }
}

void drawCloud() {
  gamby.drawSprite(cloudX, 9, cloud_1);
  gamby.update();
}

void waitForAnyButton() {
  gamby.readInputs();
  byte initialInputs = gamby.inputs;
  while (gamby.inputs == initialInputs) {
    delay(100);
    gamby.readInputs();
  }
}

void drawScreen() {
  gamby.clearScreen();
  gamby.setPos(82, 0);
  gamby.print(score);
}

void loop() {
  if (playing) {

    if (millis() > timeToDraw) {
      gamby.readInputs();

      velocityY += gravity;
      positionY += velocityY;
      cloudX -= cloudVelocityX;
      cactusX -= cactusVelX;

      if (positionY > 40)
      {
        positionY = 40;
        velocityY = 0.0;
        onGround = true;
      }

      if (cloudX < - 13) {
        cloudX = 80;
      }

      if (cactusX < - 13) {
        cactusX = 80;
      }

      drawCloud();
      drawCactus();
      drawDino();

      gamby.line(0, 62, 95, 62);
      gamby.update();

      // jump!
      if (onGround && gamby.inputs & BUTTON_1) {
        startJump();
      }

      ++score;
      drawScreen();
      timeToDraw = millis() + dinoSpeed;
      delay(16);
    }
  }
}
