cdef extern from "flappy_bird.h":

    ctypedef struct Flappy_env:
        int *state[4];
        Bird *bird;
        PipePair *pipes[3];
        int action;
        int reward;
        bool done;

    
    ctypedef struct Flappy_client:
      int WIDTH;
      int HEIGHT;

cdef class CyFlappyBird:
    cdef:
        Flappy_client *client
        Flappy_env *env

    def __init__(self):
        self.env = Flappy_env()
        self.client = generate_client()

    def step(self, int action):
        self.env.action = action
        return c_step(self.env, self.client)

    def render(self):
        c_render(self.env, self.client)
