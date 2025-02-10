import gymnasium
import numpy as np
import time
from cy_flappy_bird import CyFlappy
import pufferlib

class Flappy(pufferlib.PufferEnv):
    def __init__(self, num_envs=1, render_mode=None, buf=None):
        self.single_observation_space = gymnasium.spaces.Box(low=-500, high=1000, shape=(4,), dtype=np.int32)
        
        self.single_action_space = gymnasium.spaces.Discrete(2)
        self.render_mode = render_mode
        self.num_agents = num_envs

        super().__init__(buf)
        self.rewards = np.zeros(num_envs, dtype=np.int32) #PufferEnv defaults rewards as floats
        self.c_envs = CyFlappy(self.observations, self.actions, self.rewards, self.terminals, num_envs)

    def reset(self, seed=None):
        self.c_envs.reset()
        return self.observations, []

    def step(self, actions):
        self.actions[:] = actions
        self.c_envs.step()

        episode_returns = self.rewards[self.terminals]

        info = []
        if len(episode_returns) > 0:
            info = [{
                'reward': np.mean(episode_returns)
            }]

        return (self.observations, self.rewards, self.terminals, self.truncations, info)

    def render(self):
        self.c_envs.render()

    def close(self):
        self.c_envs.close()
