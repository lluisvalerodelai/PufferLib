import gymnasium
import random
from cy_flappy_bird import CyFlappy
import pufferlib

NOOP = 0
JUMP = 1

env = CyFlappy()

env.reset()

for _ in range(100):
    action = 0
    env.step(action)
    env.render()
