#include "raylib.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

int PIPE_SEPARATION = 200;
int NOOP = 0;
int JUMP = 1;
int WIDTH = 500;
int HEIGHT = 800;

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
  int *state[4];
  Bird *bird;
  PipePair *pipes[3];
  int action;
  int reward;
  bool done;
} Flappy_env;

typedef struct {
  int WIDTH;
  int HEIGHT;

} Flappy_client;

Bird *generateBird() {
  Bird *bird = (Bird *)calloc(1, sizeof(Bird));
  bird->y_pos = 0;
  bird->x_pos = 200;
  bird->radius = 15;

  return bird;
}

// setPipeList sets the initial 2 pipes, afterwards new pipes are generated one
// by one
void setPipeList(PipePair *pipes) {
  pipes->velocity = 1;
  pipes->gapY = 100 + (rand() % 300);
  pipes->gapX = WIDTH;
  pipes->gapSize = 125 + ((rand() % 75));
  pipes->gapWidth = 100;
}

void allocate(Flappy_env *env) {
  env->bird = generateBird();

  env->pipes[0] = (PipePair *)calloc(1, sizeof(PipePair));
  // we dont set the first pipe because we wouldnt render pipe "0"
  for (int i = 1; i < 3; i++) {
    env->pipes[i] = (PipePair *)calloc(1, sizeof(PipePair));
    setPipeList(env->pipes[i]);
  }
}

Flappy_client *make_client() {
  Flappy_client *client = (Flappy_client *)calloc(1, sizeof(Flappy_client));
  client->WIDTH = 500;
  client->HEIGHT = 800;

  InitWindow(client->WIDTH, client->HEIGHT, "PufferLib Flappy Bird");
  SetTargetFPS(80);
  return client;
}

void c_reset(Flappy_env *env) {
  // reset the bird, resetting radius and x_pos is redundant?
  env->bird->y_pos = 0;
  env->bird->x_pos = 200;
  env->bird->radius = 15;

  // reset the **values** of the environment, but not the pointers
  env->action = NOOP;
  env->done = false;
  env->reward = 0;
}

bool checkCollisions(Bird *bird, PipePair *pipes) {

  if (bird->y_pos > HEIGHT) {
    return true;
  }

  // check collision with the top pipe
  bool collisionY = bird->y_pos - bird->radius < pipes->gapY;
  bool collisionX = bird->x_pos + bird->radius > pipes->gapX;

  // printf("collision X: %b \n collision Y: %b \n", collisionX, collisionY);

  if (collisionX && collisionY)
    return true;

  // check collision for the bottom poipe
  collisionY = bird->y_pos + bird->radius > pipes->gapY + pipes->gapSize;

  if (collisionY && collisionX)
    return true;

  return false;
}

// for rendering the score on the screen
char *uint_to_str(int num) {
  // Max length for an unsigned int string (assuming 32-bit) + null terminator
  char *str = (char *)malloc(11);

  sprintf(str, "%u", num);
  return str; // Caller must free this memory
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

void c_step(Flappy_env *env) {

  // update velocities
  if (env->action == JUMP) {
    env->bird->vertical_velocity = -10;
  }

  env->bird->y_pos += env->bird->vertical_velocity;

  if (env->bird->vertical_velocity < 20) {
    env->bird->vertical_velocity += gravity;
  }

  // update the pipe render order if needed
  if (env->pipes[1]->gapX + env->pipes[1]->gapWidth < env->bird->x_pos) {
    free(env->pipes[0]);
    env->pipes[0] = env->pipes[1];
    env->pipes[1] = env->pipes[2];
    env->pipes[2] = new_pipe();
  }

  // check if env is done
  if (checkCollisions(env->bird, env->pipes[1])) {
    env->done = true;
  } else {
    env->reward += 1;
  }
}

void renderPipes(PipePair *pipes) {

  DrawRectangle(pipes->gapX, 0, pipes->gapWidth, pipes->gapY, GREEN);
  DrawRectangle(pipes->gapX, pipes->gapY + pipes->gapSize, pipes->gapWidth,
                HEIGHT - pipes->gapY + pipes->gapSize, GREEN);

  pipes->gapX -= pipes->velocity;
}

void c_render(Flappy_client *client, Flappy_env *env) {

  if (IsKeyDown(KEY_ESCAPE)) {
    exit(0);
  }

  BeginDrawing();

  DrawCircle(env->bird->x_pos, env->bird->y_pos, env->bird->radius, RED);

  renderPipes(env->pipes[0]);

  if (env->pipes[0]->gapX < env->bird->x_pos) {
    renderPipes(env->pipes[1]);
  }

  if (env->pipes[1]->gapX + env->pipes[1]->gapWidth < env->bird->x_pos) {
    renderPipes(env->pipes[2]);
  }

  char *score_str = uint_to_str(env->reward);
  DrawText(score_str, 0, 0, 50, WHITE);
  free(score_str);

  ClearBackground(SKYBLUE);
  EndDrawing();
}

void free_env(Flappy_env *env) {
  free(env->bird);
  for (int i = 0; i < 3; i++) {
    free(env->pipes[i]);
  }

  free(env);
}

void free_client(Flappy_client *client) { free(client); }
