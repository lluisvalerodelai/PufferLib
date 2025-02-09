import numpy as np
from cy_flappy_bird import CyFlappy
import pufferlib

NOOP = 0
JUMP = 1

observations = np.zeros((1, 4), dtype=np.int32)
actions = np.zeros(1, dtype=np.int32)
rewards = np.zeros(1, dtype=np.int32)
terminals = np.zeros(1, dtype=bool)

env = CyFlappy(rewards, actions, terminals)

env.reset()

for _ in range(100):

    print(rewards)
    print(terminals)
    print(actions)

    action = 0
    env.step(action)
    env.render()

env.close()
