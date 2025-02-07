#include "flappy_bird.h"
#include "../../puffernet.h"
#include "raylib.h"

int main(void) {

  Flappy_env env;
  allocate(&env); //set all the default/0 values for the env
  
  Flappy_client *client = make_client(); //client is for rendering, heandles the initwindow stuff so that later we can pass it to python and get it to render in python

  c_reset(&env); 
  while (!WindowShouldClose()) {

    if (env.done) {
      break;
    }

    //the bird jumps when the key is pressed, not in down position
    //otherwise the bird flies upwards
    if (GetKeyPressed() == KEY_SPACE) {
      env.action = JUMP;
    } else {
      env.action = NOOP;
    }
  
    c_step(&env);
    c_render(client, &env);
  }
  

}
