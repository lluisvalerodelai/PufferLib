#include "flappy_bird.h"
#include "../../puffernet.h"
#include "raylib.h"

int main(void) {

  // Weights* weights = load_weights("resources/pong_weights.bin", 133764);
  // LinearLSTM* net = make_linearlstm(weights, 1, 8, 3);

  Flappy_client *client = generate_client();
  Flappy_env *env = generateEnv();

  InitWindow(WIDTH, HEIGHT, "Flappy bird");
  SetTargetFPS(80);

  // c_reset(env)

  while (!WindowShouldClose()) {

    if (GetKeyPressed() == KEY_SPACE) {
      env->action = JUMP;
    } else {
      env->action = NOOP;
    }

    c_step(env, client);
    c_render(env, client);

    if (env->done) {
      break;
    }
  }

  CloseWindow();
  free(env);
  free(client);

  return 0;
}
