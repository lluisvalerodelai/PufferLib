#include "flappy_bird.h"
#include "raylib.h"


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
