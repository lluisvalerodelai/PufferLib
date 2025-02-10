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
        int *state
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
        Flappy_env* envs
        Flappy_client* client
        int num_envs

    def __init__(self, int[:, :] observations, int[:] actions, int[:] rewards, unsigned char[:] terminals, int num_envs):
        self.envs = <Flappy_env*>calloc(num_envs, sizeof(Flappy_env))
        self.num_envs = num_envs
        self.client = NULL

        cdef int i
        for i in range(num_envs):
            allocate(&self.envs[i])

            self.envs[i].state = &observations[i, 0] 
            self.envs[i].actions = &actions[i]
            self.envs[i].rewards = &rewards[i]
            self.envs[i].terminals = &terminals[i]

    def reset(self):
        cdef int i
        for i in range(self.num_envs):
            c_reset(&self.envs[i])


    def step(self):
        cdef int i
        for i in range(self.num_envs):
            c_step(&self.envs[i])

    def render(self):
        if self.client == NULL:
            self.client = make_client()
        c_render(self.client, &self.envs[0])

    def close(self):
        if self.client != NULL:
            free_client(self.client)
            self.client = NULL

        free_env(self.envs)
