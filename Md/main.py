from typing import NamedTuple, List
import numpy as np

class Experiment(NamedTuple):
    seed: int
    initial_value: np.int64
    min_step: np.int64
    max_step: np.int64

class IterationState(NamedTuple):
    value: np.int64

class Simulation:
    def __init__(self, theExperiment: Experiment) -> None:
        self.theExperiment = theExperiment
        self.rng = np.random.default_rng(theExperiment.seed)
        self.history: List[IterationState] = [IterationState(theExperiment.initial_value)]

    def step(self) -> None:
        last_value = self.history[-1].value
        step = self.rng.integers(
            self.theExperiment.min_step,
            self.theExperiment.max_step + 1,
            dtype=np.int64
        )
        new_value = last_value + step
        self.history.append(IterationState(new_value))

    def run(self, steps: int) -> None:
        for _ in range(steps):
            self.step()

e: Experiment = Experiment(seed=31337, initial_value=0, min_step=0-5, max_step=5)
s: Simulation = Simulation(e)

s.run(10)
print(s.history)
