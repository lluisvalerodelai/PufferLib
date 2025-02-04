#include <raylib.h>
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 500
#define HEIGHT 800
#define PIPE_SEPARATION 200

int gravity = 1;
int num_pipes_screen = 3;

typedef struct {
  int y_pos;
  int x_pos;
  int vertical_velocity;
  int gravity;
} Bird;

typedef struct {
  int velocity;
  int gapY;
  int gapX;
  int gapSize;
  int gapWidth;
} PipePair;

Bird *generateBird() {
  Bird *bird = (Bird *)calloc(1, sizeof(Bird));
  bird->y_pos = 200;
  bird->x_pos = 200;

  return bird;
}

PipePair *new_pipe() {
    PipePair *pipes = (PipePair *)calloc(1, sizeof(PipePair));

    pipes->velocity = 1;
    pipes->gapY = 100 + (rand() % 300);
    pipes->gapX = WIDTH;
    pipes->gapSize = 125 + ((rand() % 75));
    pipes->gapWidth = 100;

  return pipes;
}

void setPipeList(PipePair *pipes[num_pipes_screen]) {
  for (int i = 0; i < num_pipes_screen; i++) {
    pipes[i] = new_pipe();
  }
}

void renderPipes(PipePair *pipes[num_pipes_screen], int pipeNum) {

  DrawRectangle(pipes[pipeNum]->gapX, pipeNum, pipes[pipeNum]->gapWidth, pipes[pipeNum]->gapY, GREEN);
  DrawRectangle(pipes[pipeNum]->gapX, pipes[pipeNum]->gapY + pipes[pipeNum]->gapSize, pipes[pipeNum]->gapWidth,
                HEIGHT - pipes[pipeNum]->gapY + pipes[pipeNum]->gapSize, GREEN);

  pipes[pipeNum]->gapX -= pipes[pipeNum]->velocity;
}
