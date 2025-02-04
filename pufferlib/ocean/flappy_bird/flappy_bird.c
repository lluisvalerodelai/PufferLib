#include "flappy_bird.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

// Flappy_env env = ..;
// allocate(&env)?
// Client* client = make_client for the env make_client(&env)
// c_reset(&env)
// while(!window close)
// if (pressing left shift)
//  if (action)
//   env.action = action
// c_render(client, &env)

#define WIDTH 500
#define HEIGHT 800
#define PIPE_SEPARATION 200

int gravity = 1;
int pipe_speed_rate = 1;

int pipe_height = 250;
int pipe_width = 100;
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



int main(void) {

  InitWindow(WIDTH, HEIGHT, "Flappy bird");
  SetTargetFPS(60);

  Bird *bird = generateBird();
  PipePair *pipes[num_pipes_screen];
  setPipeList(pipes);

  while (!WindowShouldClose()) {

    if (GetKeyPressed() == KEY_SPACE) {
      bird->vertical_velocity = -10;
    }
    bird->y_pos += bird->vertical_velocity;

    BeginDrawing();

    DrawCircle(bird->x_pos, bird->y_pos, 15, RED);
    renderPipes(pipes, 0);

    ClearBackground(SKYBLUE);
    EndDrawing();

    if (bird->vertical_velocity < 20) {
      bird->vertical_velocity += gravity;
    }

    if (pipes[0]->gapX < bird->x_pos) {
      renderPipes(pipes, 1);
    }

    if (pipes[1]->gapX < bird->x_pos) {
      renderPipes(pipes, 2);
      pipes[0] = pipes[1];
      pipes[1] = pipes[2];
      pipes[2] = new_pipe();
    }

  }

  CloseWindow();
  // free everything

  return 0;
}
