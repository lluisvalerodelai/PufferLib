cimport numpy as cnp
from libc.stdlib cimport calloc, free

cdef extern from "flappy_bird.h":
    ctypedef struct Bird:
        int y_pos
        int x_pos
        int vertical_velocity
        int gravity
        int radius

    ctypedef struct PipePair:
        int velocity
        int gapY
        int gapX
        int gapSize
        int gapWidth

    ctypedef struct Flappy_env:
        int* state[4]
        Bird* bird
        PipePair* pipes[3]
        int action
        int reward
        bint done

    ctypedef struct Flappy_client:
        int WIDTH
        int HEIGHT

    Bird* generateBird()
    void setPipeList(PipePair* pipes)
    void allocate(Flappy_env* env)
    Flappy_client* make_client()
    void c_reset(Flappy_env* env)
    bint checkCollisions(Bird* bird, PipePair* pipes)
    char* uint_to_str(int num)
    PipePair* new_pipe()
    void c_step(Flappy_env* env)
    void renderPipes(PipePair* pipes)
    void c_render(Flappy_client* client, Flappy_env* env)
    void free_env(Flappy_env* env)
    void free_client(Flappy_client* client)

# Cython Class for Flappy Bird Environment
cdef class CyFlappy:
    cdef:
        Flappy_env* env
        Flappy_client* client
        int action

    def __init__(self):
        self.env = <Flappy_env*>calloc(1, sizeof(Flappy_env))
        self.client = NULL
        self.action = 0
        allocate(self.env)

    def reset(self):
        c_reset(self.env)

    def step(self, int action):
        self.action = action
        self.env.action = action  # Assign the action to environment
        c_step(self.env)

    def render(self):
        if self.client == NULL:
            self.client = make_client()  # Initialize client window
        c_render(self.client, self.env)

    def close(self):
        if self.client != NULL:
            free_client(self.client)
            self.client = NULL
        free_env(self.env)

    def get_reward(self):
        return self.env.reward

    def is_done(self):
        return self.env.done

