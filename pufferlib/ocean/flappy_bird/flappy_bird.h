#include "raylib.h"
#include <raylib.h>
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
  int radius;
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
  bird->radius = 15;

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

// setPipeList sets the initial 2 pipes, afterwards new pipes are generated one
// by one
void setPipeList(PipePair *pipes[num_pipes_screen]) {
  pipes[0] = (PipePair *)calloc(1, sizeof(PipePair));
  for (int i = 1; i < num_pipes_screen; i++) {
    pipes[i] = new_pipe();
  }
}

void renderPipes(PipePair *pipes[num_pipes_screen], int pipeNum) {

  DrawRectangle(pipes[pipeNum]->gapX, 0, pipes[pipeNum]->gapWidth,
                pipes[pipeNum]->gapY, GREEN);
  DrawRectangle(pipes[pipeNum]->gapX,
                pipes[pipeNum]->gapY + pipes[pipeNum]->gapSize,
                pipes[pipeNum]->gapWidth,
                HEIGHT - pipes[pipeNum]->gapY + pipes[pipeNum]->gapSize, GREEN);

  pipes[pipeNum]->gapX -= pipes[pipeNum]->velocity;
}

bool checkCollisions(Bird *bird, PipePair *pipes) {

  if (bird->y_pos > HEIGHT)
    return true;


  //check collision with the top pipe
  bool collisionY = bird->y_pos - bird->radius < pipes->gapY;
  bool collisionX = bird->x_pos + bird->radius > pipes->gapX;

  if (collisionX && collisionY)
    return true;

  //check collision for the bottom poipe
  collisionY = bird->y_pos + bird->radius > pipes->gapY + pipes->gapSize;

  if (collisionY && collisionX) return true;

  return false;
}

char* uint_to_str(unsigned int num) {
    // Max length for an unsigned int string (assuming 32-bit) + null terminator
    char* str = (char *) malloc(11); 
    if (!str) return NULL;  // Check for allocation failure

    sprintf(str, "%u", num);
    return str;  // Caller must free this memory
}
