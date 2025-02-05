#include "flappy_bird.h"
#include "raylib.h"

int main(void) {

  bool done = false;
  InitWindow(WIDTH, HEIGHT, "Flappy bird");
  SetTargetFPS(60);
  unsigned int score = 0;
  char str[32];

  Bird *bird = generateBird();
  PipePair *pipes[num_pipes_screen];
  setPipeList(pipes);

  while (!WindowShouldClose()) {

    if (GetKeyPressed() == KEY_SPACE) {
      bird->vertical_velocity = -10;
    }
    bird->y_pos += bird->vertical_velocity;

    BeginDrawing();

    char* score_str = uint_to_str(score);
    DrawText(score_str, 0, 0, 50, WHITE);
    free(score_str);

    DrawCircle(bird->x_pos, bird->y_pos, bird->radius, RED);
    renderPipes(pipes, 0);

    ClearBackground(SKYBLUE);
    EndDrawing();

    if (bird->vertical_velocity < 20) {
      bird->vertical_velocity += gravity;
    }

    if (pipes[0]->gapX < bird->x_pos) {
      renderPipes(pipes, 1);
    }

    if (pipes[1]->gapX + pipes[1]->gapWidth < bird->x_pos) {
      renderPipes(pipes, 2);
      pipes[0] = pipes[1];
      pipes[1] = pipes[2];
      pipes[2] = new_pipe();
    }

    if (checkCollisions(bird, pipes[1])) {
      done = true;
      break;
    } else {
      score += 1;
    }
  }

  CloseWindow();
  // free everything
  
  return 0;
}
