cdef extern from "flappy_bird.h":
    int WIDTH
    int HEIGHT
    int PIPE_SEPARATION
    int NOOP
    int JUMP
    int gravity
    int num_pipes_screen

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
        int action
        int reward
        bint done
    
    ctypedef struct Flappy_client:
        Bird *bird
        PipePair pipes[3] #change to num_pipes_screen

    Flappy_env *generateEnv()
    Bird *generateBird()
    PipePair new_pipe()
    void setPipeList(Flappy_client *client)
    Flappy_client *generate_client()
    void renderPipes(PipePair *pipes[3], int pipeNum) #also change to num_pipes_screen
    bint checkCollisions(Bird *bird, PipePair*pipes)
    char *uint_to_str(unsigned int num)
    void c_step(Flappy_env *env, Flappy_client *client)
    void c_render(Flappy_env *env, Flappy_client *client) 
    void free_env(Flappy_env *env)
    void free_client(Flappy_client *client)

cdef class CyFlappyBird:
    cdef:
        Flappy_client *client
        Flappy_env *env

    def __init__(self):
        self.env = generateEnv()
        self.client = generate_client()

    def step(self, int action):
        self.env.action = action
        return c_step(self.env, self.client)

    def render(self):
        c_render(self.env, self.client)
