#include "flappy_bird.h"
#include "raylib.h"

int main(void) {

  bool done = false;
  InitWindow(WIDTH, HEIGHT, "Flappy bird");
  SetTargetFPS(60);
  unsigned int score = 0;
  char str[32];

  Flappy_client *client = generate_client();

  while (!WindowShouldClose()) {

    if (GetKeyPressed() == KEY_SPACE) {
      client->bird.vertical_velocity = -10;
    }
    client->bird.y_pos += client->bird.vertical_velocity;

    BeginDrawing();

    char* score_str = uint_to_str(score);
    DrawText(score_str, 0, 0, 50, WHITE);
    free(score_str);

    DrawCircle(client->bird.x_pos, client->bird.y_pos, client->bird.radius, RED);
    renderPipes(client->pipes, 0);

    ClearBackground(SKYBLUE);
    EndDrawing();

    if (client->bird.vertical_velocity < 20) {
      client->bird.vertical_velocity += gravity;
    }

    if (client->pipes[0].gapX < client->bird.x_pos) {
      renderPipes(client->pipes, 1);
    }

    if (client->pipes[1].gapX + client->pipes[1].gapWidth < client->bird.x_pos) {
      renderPipes(client->pipes, 2);
      client->pipes[0] = client->pipes[1];
      client->pipes[1] = client->pipes[2];
      client->pipes[2] = new_pipe();
    }

    if (checkCollisions(client->bird, client->pipes[1])) {
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
