#include "raylib.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 500
#define HEIGHT 800
#define PIPE_SEPARATION 200
#define NOOP 0
#define JUMP 1

#define num_pipes_screen 3

int gravity = 1;

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

typedef struct {
  int state[4];
  int action;
  int reward;
  bool done;
} Flappy_env;

typedef struct {
  Bird *bird;
  PipePair pipes[num_pipes_screen];

} Flappy_client;

Flappy_env *generateEnv() {
  Flappy_env *env = (Flappy_env *)calloc(1, sizeof(Flappy_env));
  return env;
}

Bird *generateBird() {
  Bird *bird = (Bird *)calloc(1, sizeof(Bird));
  bird->y_pos = 0;
  bird->x_pos = 200;
  bird->radius = 15;

  return bird;
}

PipePair new_pipe() {
  PipePair *pipes = (PipePair *)calloc(1, sizeof(PipePair));

  pipes->velocity = 1;
  pipes->gapY = 100 + (rand() % 300);
  pipes->gapX = WIDTH;
  pipes->gapSize = 125 + ((rand() % 75));
  pipes->gapWidth = 100;

  return *pipes;
}

// setPipeList sets the initial 2 pipes, afterwards new pipes are generated one
// by one
void setPipeList(Flappy_client *client) {
  client->pipes[0] =
      *(PipePair *)calloc(1, sizeof(PipePair)); // we dont render pipe 0
  for (int i = 1; i < num_pipes_screen; i++) {
    client->pipes[i] = new_pipe();
  }
}

Flappy_client *generate_client() {
  Flappy_client *client = (Flappy_client *)calloc(1, sizeof(Flappy_client));

  client->bird = generateBird();
  setPipeList(client);

  return client;
}

void renderPipes(PipePair pipes[num_pipes_screen], int pipeNum) {

  DrawRectangle(pipes[pipeNum].gapX, 0, pipes[pipeNum].gapWidth,
                pipes[pipeNum].gapY, GREEN);
  DrawRectangle(pipes[pipeNum].gapX,
                pipes[pipeNum].gapY + pipes[pipeNum].gapSize,
                pipes[pipeNum].gapWidth,
                HEIGHT - pipes[pipeNum].gapY + pipes[pipeNum].gapSize, GREEN);

  pipes[pipeNum].gapX -= pipes[pipeNum].velocity;
}

bool checkCollisions(Bird *bird, PipePair *pipes) {

  if (bird->y_pos > HEIGHT)
    return true;

  // check collision with the top pipe
  bool collisionY = bird->y_pos - bird->radius < pipes->gapY;
  bool collisionX = bird->x_pos + bird->radius > pipes->gapX;

  if (collisionX && collisionY)
    return true;

  // check collision for the bottom poipe
  collisionY = bird->y_pos + bird->radius > pipes->gapY + pipes->gapSize;

  if (collisionY && collisionX)
    return true;

  return false;
}

// for rendering the score on the screen
char *uint_to_str(unsigned int num) {
  // Max length for an unsigned int string (assuming 32-bit) + null terminator
  char *str = (char *)malloc(11);

  sprintf(str, "%u", num);
  return str; // Caller must free this memory
}

void c_step(Flappy_env *env, Flappy_client *client) {

  // update velocities
  if (env->action == JUMP) {
    client->bird->vertical_velocity = -10;
  }

  client->bird->y_pos += client->bird->vertical_velocity;

  if (client->bird->vertical_velocity < 20) {
    client->bird->vertical_velocity += gravity;
  }

  // update the pipe render order if needed
  if (client->pipes[1].gapX + client->pipes[1].gapWidth < client->bird->x_pos) {
    client->pipes[0] = client->pipes[1];
    client->pipes[1] = client->pipes[2];
    client->pipes[2] = new_pipe();
  }

  // check if env is done
  if (checkCollisions(client->bird, &client->pipes[1])) {
    env->done = true;
  } else {
    env->reward += 1;
  }
}

void c_render(Flappy_env *env, Flappy_client *client) {
  BeginDrawing();

  DrawCircle(client->bird->x_pos, client->bird->y_pos, client->bird->radius,
             RED);

  renderPipes(client->pipes, 0);

  if (client->pipes[0].gapX < client->bird->x_pos) {
    renderPipes(client->pipes, 1);
  }

  if (client->pipes[1].gapX + client->pipes[1].gapWidth < client->bird->x_pos) {
    renderPipes(client->pipes, 2);
  }

  char *score_str = uint_to_str(env->reward);
  DrawText(score_str, 0, 0, 50, WHITE);
  free(score_str);

  ClearBackground(SKYBLUE);
  EndDrawing();
}

void free_env(Flappy_env *env) { free(env); }

void free_client(Flappy_client *client) { free(client); }
