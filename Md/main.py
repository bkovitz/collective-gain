from typing import NamedTuple, List
import numpy as np

class Experiment(NamedTuple):
    seed: int
    generations: int
    initial_organism_count: int
    initial_carrier_count: int

    giving_radius: np.float64
    giver_development_probability: np.float64

    child_placement_radius: np.float64
    number_of_children: int
    base_survival_probability: np.float64
    giver_survival_bonus: np.float64 


# the main array containing all individuals in the world
World = np.ndarray # shape=(N,5), dtype=np.float64
XCOORD: int = 0
YCOORD: int = 1
CARRIER: int = 2
ALIVE: int = 3
GIVER: int = 4


def initialize_world(the_experiment: Experiment, the_rng: np.random.Generator) -> World:
    total_population = the_experiment.initial_organism_count
    the_world: World = np.zeros((total_population, 5), dtype=np.float64)
    the_world[:,[XCOORD,YCOORD]] = the_rng.random((total_population, 2))
    the_world[0:the_experiment.initial_carrier_count,CARRIER] = 1
    return the_world

class Simulation:
    def __init__(self, the_experiment: Experiment) -> None:
        self.experiment = the_experiment
        self.rng = np.random.default_rng(the_experiment.seed)
        self.history: List[World] = [initialize_world(the_experiment, self.rng)]

    def step(self) -> None:
        self.history.append(self.history[-1])

    def run(self) -> None:
        for _ in range(self.experiment.generations):
            self.step()

e: Experiment = Experiment(
    seed=31337,
    generations=1,
    initial_organism_count=10,
    initial_carrier_count=1,
    giving_radius=np.float64(0.00),
    giver_development_probability=np.float64(0.00),
    child_placement_radius=np.float64(1.00),
    number_of_children=3,
    base_survival_probability=np.float64(0.1),
    giver_survival_bonus=np.float64(0.1)
  )
s: Simulation = Simulation(e)

s.run()
print(s.history[0])
