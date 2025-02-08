import gymnasium
from cy_flappy_bird import CyFlappy
import pufferlib

NOOP = 0
JUMP = 1

class FlappyBird(pufferlib.PufferEnv):
    def __init__(self, buf=None):
        self.single_observation_space = gymnasium.spaces.Box(low=-1000, high=1000, shape=(4,))
        self.single_action_space = gymnasium.spaces.Discrete(2) 
        self.env = CyFlappy()
        super().__init__(buf)

    def reset(self):
        self.observatio
