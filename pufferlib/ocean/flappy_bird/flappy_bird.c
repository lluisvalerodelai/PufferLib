#include "flappy_bird.h"
#include "../../puffernet.h"
#include "raylib.h"

int main(void) {

  Flappy_env env;
  allocate(&env); // set all the default/0 values for the env

  Flappy_client *client = make_client();


  c_reset(&env);
  while (!WindowShouldClose()) {

    if (env.terminals[0] == 1) {
      break;
    }

    // the bird jumps when the key is pressed, not in down position
    // otherwise the bird flies upwards
    if (GetKeyPressed() == KEY_SPACE) {
      env.actions[0] = JUMP;
    } else {
      env.actions[0] = NOOP;
    }

    c_step(&env);
    c_render(client, &env);
    print_flappy_env(&env);
  }
}
