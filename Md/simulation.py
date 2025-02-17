from typing import NamedTuple, List, Tuple
import numpy as np

World = np.ndarray # shape=(N,5), dtype=np.float64
XCOORD: int = 0
YCOORD: int = 1
IS_ALIVE: int = 2
IS_GIVER: int = 3
IS_CARRIER: int = 4
World_fields = 5

class Experiment(NamedTuple):
    seed: int
    generations: int
    capacity: int
    initial_organism_count: int
    initial_carrier_count: int

    giving_radius: np.float64
    giver_development_probability: np.float64

    child_placement_radius: np.float64
    max_number_of_children: int
    base_survival_probability: np.float64
    giver_survival_bonus: np.float64

def initialize_world(
    the_experiment: Experiment,
    the_rng: np.random.Generator
) -> World:
    the_world: World = np.zeros(
        shape=(the_experiment.initial_organism_count, World_fields),
        dtype=np.float64
    )
    the_world[:,[XCOORD,YCOORD]] = the_rng.uniform(
        low=0,
        high=1,
        size=(the_experiment.initial_organism_count, 2)
    )
    the_world[0:the_experiment.initial_carrier_count,IS_CARRIER] = 1
    the_rng.shuffle(the_world, axis=0)
    return the_world

class Simulation:
    def __init__(self, the_experiment: Experiment) -> None:
        self.experiment = the_experiment
        self.rng = np.random.default_rng(the_experiment.seed)
        the_world: World = initialize_world(
            the_experiment,
            self.rng
        )
        self.history: List[World] = [the_world]

    def displacements(self, count: int, radius: np.float64) -> np.ndarray:
        angles = self.rng.uniform(0, 2 * np.pi, count)
        vectors = \
            np.column_stack((np.cos(angles), np.sin(angles))) * \
            np.tile(self.rng.uniform(0, radius, count), (1,1)).transpose()
        return vectors

    def step(self) -> None:
        the_world: World = self.history[-1]
        the_givers: World = the_world[the_world[:,IS_GIVER] == 1]
        the_takers: World = the_world[the_world[:,IS_GIVER] != 1]

        the_givers_locations: np.ndarray = \
            the_givers[:,[XCOORD,YCOORD]].reshape(1,the_givers.shape[0],2)
        the_takers_locations: np.ndarray = \
            the_takers[:,[XCOORD,YCOORD]].reshape(the_takers.shape[0],1,2)

        distances_to_givers: np.ndarray = np.linalg.norm(
            the_takers_locations - the_givers_locations,
            axis=2
        )

        nearby_givers: np.ndarray = \
            distances_to_givers <= self.experiment.giving_radius

        the_survival_probabilities: np.ndarray =      \
            nearby_givers.sum(axis=1) *               \
            self.experiment.giver_survival_bonus +    \
            self.experiment.base_survival_probability

        the_children: World = the_takers
        the_children[:,IS_ALIVE] = the_survival_probabilities
        the_children = the_children.repeat(
            self.experiment.max_number_of_children,
            axis=0
        )
        the_children[:,[XCOORD,YCOORD]] += self.displacements(
            count=the_children.shape[0],
            radius=self.experiment.child_placement_radius
        )
        the_children[:,[XCOORD,YCOORD]] %= 1

        the_children[:,IS_GIVER] = self.rng.binomial(
            n=1,
            p=self.experiment.giver_development_probability,
            size=the_children.shape[0]
        ) * the_children[:,IS_CARRIER]

        new_world: World =                               \
            the_children[the_children[:,IS_ALIVE] >      \
            self.rng.uniform(0,1,the_children.shape[0])]
        new_world[:,IS_ALIVE] = 1

        self.rng.shuffle(new_world, axis=0)
        while new_world.shape[0] > self.experiment.capacity:
            cutoff: np.float64 = \
                np.float64(self.experiment.capacity) / \
                np.float64(new_world.shape[0])
            new_world = new_world[
                self.rng.uniform(0,1,new_world.shape[0]) < cutoff
            ]
        self.history.append(new_world)

    def run(self) -> None:
        for iteration in range(self.experiment.generations):
            ww: World = self.history[-1]
            self.step()
