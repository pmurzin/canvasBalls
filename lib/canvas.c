#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emscripten.h>

// number of circles
#define NUM_CIRCLES 100

struct Circle {
  // coordinates and radius
  int x;
  int y;
  int rad;
  // rgb colours
  int cr;
  int cg;
  int cb;
};

struct CircleAnimationData {
  // coordinates and radius
  int x;
  int y;
  int rad;
  // axis velocities
  int xv;
  int yv;
  // axis direction (1 == forward, 0 == backward)
  int xd;
  int yd;
};

struct Circle circles[NUM_CIRCLES];
struct CircleAnimationData animationData[NUM_CIRCLES];

int getRand(max) {
  return (rand() % max);
}

// init circle data and start render - JS
int main() {

  // printf("init circles\n");

  // seed random number generator
  srand(time(NULL));

  // create circles
  for (int i = 0; i < NUM_CIRCLES; i++) {
    int radius = getRand(150);

    int x = getRand(1000) + radius;
    int y = getRand(1000) + radius;

    // fill animation data struct - i
    animationData[i].x = x;
    animationData[i].y = y;
    animationData[i].rad = radius;
    animationData[i].xv = getRand(10);
    animationData[i].yv = getRand(10);
    animationData[i].xd = 1;
    animationData[i].yd = 1;

    // fill circle struct - i
    circles[i].x = x;
    circles[i].y = y;
    circles[i].rad = radius;
    circles[i].cr = getRand(255);
    circles[i].cg = getRand(255);
    circles[i].cb = getRand(255);
  }

  // start JS rendering
  // previously was done by emscripten_run_script("render()");
  // Use helper function EM_ASM for getting dataLength of render JS function
  // 6 properies for Circle struct
  EM_ASM({ render($0, $1); }, NUM_CIRCLES*6, 6);
}

// return animated circles to JS
struct Circle * getCircles(int canvasWidth, int canvasHeight) {
  // update circle data
  for (int i = 0; i < NUM_CIRCLES; i++) {

    // collision RIGHT - set x direction backwards 0
    if ( (animationData[i].x + animationData[i].rad) >= canvasWidth )
      animationData[i].xd = 0;

    // collision LEFT - set x direction forwards 1
    if ( (animationData[i].x - animationData[i].rad) <= 0 )
      animationData[i].xd = 1;

    // collision BOTTOM - set y direction backwards 0
    if ( (animationData[i].y + animationData[i].rad) >= canvasHeight )
      animationData[i].yd = 0;

    // collision TOP - set y direction forwards 1
    if ( (animationData[i].y - animationData[i].rad) <= 0 )
      animationData[i].yd = 1;

    // Move circles in specified direction
    if (animationData[i].xd == 1) {
      animationData[i].x += animationData[i].xv;
    }
    else {
      animationData[i].x -= animationData[i].xv;
    }

    if (animationData[i].yd == 1) {
      animationData[i].y += animationData[i].yv;
    }
    else {
      animationData[i].y -= animationData[i].yv;
    }

    // Update matching circle
    circles[i].x = animationData[i].x;
    circles[i].y = animationData[i].y;
  }

  // Return updated circles
  return circles;
}