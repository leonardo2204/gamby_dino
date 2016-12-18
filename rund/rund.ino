
#include<Gamby.h>
#include <avr/pgmspace.h>
#include "assets.h"

extern const long font[];

// frame counter
unsigned int frame = 0;

// general use string buffer
char text[16];

// distance ran
int d, delta;

int cloud_1_y;

int d_jump, d_jump_t;

int d_run;
int d_tumble_t;

int ox;

long timeToDraw;
long lastDrawnTime;
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
  d = 0;
  delta = 0;

  cloud_1_y = 2;

  d_jump = 0;
  d_jump_t = 0;

  d_tumble_t = 0;
  d_run = 0;

  ox = 130;

  drawScreen();

  dinoSpeed = 200;

  timeToDraw = millis() + dinoSpeed;

  playing = true;
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
  gamby.setPos(62, 0);
  gamby.print("Score:");
  gamby.update();
}

void loop() {
  if (playing) {

    if (millis() > timeToDraw) {
      gamby.readInputs();

      ++frame;
      if (frame > 16000) frame = 0;

      drawScreen();
      if (!d_run && (gamby.inputs & BUTTON_1)) {
        d_run = 1;
      }

      if (d_tumble_t && gamby.inputs & BUTTON_1) {
        setup();
        return;
      }

      ++d;

      // increase distance whilst running
      if (d_run && (++delta > 4)) {
        delta = 0;
      }

      // obstacles
      if (d_run) {
        ox -= (frame % 2) * (d / 100) + 2;
        if (ox < -15) ox += 140 + random(0, 60);
      }

      // jump!
      if (!d_jump_t && gamby.inputs & BUTTON_1) {
        d_jump_t = 1;
        d_jump = 5;

        //tocar som
      } else if (d_jump_t) {
        //if (d_jump_t == 3) arduboy.tunes.tone(880, 80);

        ++d_jump_t;

        if (d_jump_t < 6) {
          d_jump += 6;
        } else if (d_jump_t < 9) {
          d_jump += 2;
        } else if (d_jump_t < 13) {
          d_jump += 1;
        } else if (d_jump_t == 16 || d_jump_t == 18) {
          d_jump += 1;
        } else if (d_jump_t == 20 || d_jump_t == 22) {
          d_jump -= 1;
        } else if (d_jump_t > 38) {
          d_jump = 0;
          d_jump_t = 0;
        } else if (d_jump_t > 32) {
          d_jump -= 6;
        } else if (d_jump_t > 29) {
          d_jump -= 2;
        } else if (d_jump_t > 25) {
          d_jump -= 1;
        }
      }

      // hit detect
      if (!d_tumble_t && ox > -10 && ox < 16 && d_jump_t < 14) {
        d_tumble_t = 1;
      }

      if (d_tumble_t) {
        if (d_tumble_t == 1) {
          //arduboy.tunes.tone(246, 80);
        } else if (d_tumble_t == 6) {
          //arduboy.tunes.tone(174, 200);
        }
        ++d_tumble_t;
        if (d_jump > -4) {
          d_jump -= 1;
          ox -= 1;
        } else {
          d_run = 0;
        }
      }

      // parallax clouds
      gamby.drawSprite(76 - (d % 76), cloud_1_y, cloud_1);

      // dino
      int dy = 40 - d_jump;
      gamby.drawSprite(0, dy, dino_top);

      if (d_run && !d_jump) {
        if ((frame % 8) / 4) {
          gamby.drawSprite(0, dy + 18, dino_leg_1);
        } else {
          gamby.drawSprite(0, dy + 18, dino_leg_2);
        }
      }else{
        gamby.drawSprite(0, dy + 18, dino_leg_0);
      }

      gamby.update();

      if (d % 76 + 15 == 0) {
        cloud_1_y = random(1, 50);
      }

      timeToDraw = millis() + dinoSpeed;
      lastDrawnTime = millis();
    }
  }
}
