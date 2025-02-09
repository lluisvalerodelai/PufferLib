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
        int state[4]
        Bird* bird
        PipePair* pipes[3]
        int *actions
        int *rewards
        unsigned char *terminals

    ctypedef struct Flappy_client:
        int WIDTH
        int HEIGHT

    Bird* generateBird()
    void setPipeList(PipePair* pipes)
    void allocate(Flappy_env* env)
    Flappy_client* make_client()
    void c_reset(Flappy_env* env)
    unsigned char checkCollisions(Bird* bird, PipePair* pipes)
    char* uint_to_str(int num)
    PipePair* new_pipe()
    void c_step(Flappy_env* env)
    void renderPipes(PipePair* pipes)
    void c_render(Flappy_client* client, Flappy_env* env)
    void free_env(Flappy_env* env)
    void free_client(Flappy_client* client)

cdef class CyFlappy:
    cdef:
        Flappy_env* env
        Flappy_client* client

    def __init__(self, int [:] rewards, int[:] actions, unsigned char[:] terminals):
        self.env = <Flappy_env*>calloc(1, sizeof(Flappy_env))
        self.client = NULL
        allocate(self.env)

        self.env.actions = &actions[0]
        self.env.rewards = &rewards[0]
        self.env.terminals = &terminals[0]

    def reset(self):
        c_reset(self.env)

    def step(self):
        c_step(self.env)

    def render(self):
        if self.client == NULL:
            self.client = make_client()
        c_render(self.client, self.env)

    def close(self):
        if self.client != NULL:
            free_client(self.client)
            self.client = NULL

        free_env(self.env)
