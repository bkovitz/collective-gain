from typing import NamedTuple, Union, List, Tuple
import numpy as np

from simulation import World, IS_CARRIER, Experiment, Simulation

class Inconclusive(NamedTuple):
    pass
class NoncarriersWin(NamedTuple):
    pass
class CarriersWin(NamedTuple):
    pass

Outcome = Union[Inconclusive, NoncarriersWin, CarriersWin]

def outcome(simulation: Simulation) -> Outcome:
    for iteration, world in enumerate(simulation.history):
        carrier_count: int = world[:,IS_CARRIER].sum()
        noncarrier_count: int = world.shape[0] - carrier_count
        if carrier_count == 0 and noncarrier_count > 0:
            return NoncarriersWin()
        if carrier_count > 0 and noncarrier_count == 0:
            return CarriersWin()
        if carrier_count == 0 and noncarrier_count == 0:
            return Inconclusive()
    return Inconclusive()

def make_experiment(
    child_placement_radius: np.float64,
    giving_radius: np.float64,
    seed: int
) -> Experiment:
    return Experiment(
        seed=seed,
        generations=200,
        capacity=200,
        initial_organism_count=20,
        initial_carrier_count=10,
        giving_radius=giving_radius,
        giver_development_probability=np.float64(0.10),
        child_placement_radius=child_placement_radius,
        max_number_of_children=10,
        base_survival_probability=np.float64(0.105),
        giver_survival_bonus=np.float64(1.00)
    )


param1: np.ndarray = np.linspace(0.01, 0.16, 10, dtype=np.float64)
param2: np.ndarray = np.linspace(0.01, 0.16, 10, dtype=np.float64)

seeds: List[int] = [
    0xfa73c1d1, 0xcf5da55a, 0xb70ad205, 0xdb74f2de,
    0xde7e334e, 0xd50bf241, 0x4c0ee3fa, 0x2dbaa8c1,
    0xedc8d8e9, 0x2cd9c3e3, 0x8f1ec227, 0x2f320a75,
    0x3a13d445, 0x9f7520ec, 0x66187e87, 0x2035fc0f,
    0x651f4b14, 0x82441cb3, 0xce43d650, 0xd6924b7c,
    0x164d40fb, 0xbc604b1c, 0x72483cc8, 0x65e88398,
    0xeac27846, 0x92fe4f85, 0x5fabfc18, 0x22da6496,
    0xd9f0ae68, 0x71b92331, 0xef211301, 0x3f0f8338
]

print(param1)
print(param2)
for p1 in param1:
    for p2 in param2:
        carrier_wins: float = 0
        noncarrier_wins: float = 0
        total: float = 0
        for seed in seeds:
            e: Experiment = make_experiment(p1,p2,seed)
            s: Simulation = Simulation(e)
            s.run()
            the_outcome: Outcome = outcome(s)
            if isinstance(the_outcome, NoncarriersWin):
                noncarrier_wins += 1
            if isinstance(the_outcome, CarriersWin):
                carrier_wins += 1
            total += 1
        seeds = [x + 31 for x in seeds]
        print(carrier_wins,end=",")
    print()

