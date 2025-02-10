from flappy_bird import Flappy
import numpy as np

env = Flappy(2)

env.reset()
for i in range(100):

    actions = np.array([0, 0]) 
    if i % 50 == 0:
        actions[0] = 1

    # print(env.step(actions))
    observations, rewards, done, terminals, info = env.step(actions)
    print(f"Observations: {observations}")
    print(f"Rewards: {rewards}")
    print(f"Done: {done}")
    print(f"Terminals: {terminals}")
    print(info)

    env.render()


