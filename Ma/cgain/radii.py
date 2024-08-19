# radii.py -- Experiment to see what are the probabilities that noncarriers
# appear within the giving radius of givers (or carriers)

# We're trying to estimate PN and PC.

from dataclasses import dataclass, field
from typing import Set, Tuple
from random import randint
import random

random.seed(1)

# These are "square" radii: the distance from the center of a square to its
# side.
laying_radius: int = 2

giving_radius: int = 1

carrier_initial_g: float = 0.05

# number of iterations to run
num_iterations: int = 10000

@dataclass(frozen=True)
class Organism:
    location: Tuple[int, int]

@dataclass
class World:
    carriers: Set[Organism] = field(default_factory=set)
    givers: Set[Organism] = field(default_factory=set)
    noncarriers: Set[Organism] = field(default_factory=set)

#    def __str__(self):
#        diameter = 3 * laying_radius
#        for x in range(-diameter, +diameter + 1):
#            for y in range(-diameter, +diameter + 1):
#                if Organism((x, y)) in self.givers:
                    


def random_location_near(loc: Tuple[int, int], radius: int) -> Tuple[int, int]:
    return (loc[0] + randint(-radius, +radius),
            loc[1] + randint(-radius, +radius))
    
def benefits_from(giver: Organism, recipient: Organism) -> bool:
    return (
        abs(giver.location[0] - recipient.location[0]) <= giving_radius
        and
        abs(giver.location[1] - recipient.location[1]) <= giving_radius
    )

PCs = []
PNs = []

for i in range(num_iterations):
    world = World()
    carrier1 = Organism((0, 0))
    noncarrier1 = Organism(random_location_near((0, 0), 2 * laying_radius))

#    # make babies until we get a giver
#    while True:
#        baby = Organism(
#            random_location_near(noncarrier1.location, laying_radius)
#        )
#        world.noncarriers.add(baby)
#
#        baby = Organism(random_location_near(carrier1.location, laying_radius))
#        if random.uniform(0.0, 1.0) < carrier_initial_g:
#            world.givers.add(baby)
#            break
#        else:
#            world.carriers.add(baby)
    
    # make babies
    world.givers.add(   # make sure we have a giver
        Organism(random_location_near(carrier1.location, laying_radius))
    )
                        # the carrier's other babies
    for _ in range(1, round(1.0 / carrier_initial_g)):
        world.carriers.add(
            Organism(random_location_near(carrier1.location, laying_radius))
        )
                        # the noncarrier's babies
    for _ in range(0, round(1.0 / carrier_initial_g)):
        world.noncarriers.add(
            Organism(random_location_near(noncarrier1.location, laying_radius))
        )

    carrier_beneficiaries: float = sum(
        1.0 for c in world.carriers
            for g in world.givers if benefits_from(g, c)
    )
    noncarrier_beneficiaries: float = sum(
        1.0 for n in world.noncarriers
            for g in world.givers if benefits_from(g, n)
    )
    if len(world.carriers) == 0:
        continue
    PC = carrier_beneficiaries / len(world.carriers)
    PN = noncarrier_beneficiaries / len(world.noncarriers)

    #print('PC', PC)
    #print('PN', PN)
    PCs.append(PC)
    PNs.append(PN)

print('giving_radius:', giving_radius)
print('# children:', round(1.0 / carrier_initial_g))
avgPC = sum(PCs) / len(PCs)
print('PC avg:', avgPC)
avgPN = sum(PNs) / len(PNs)
print('PN avg:', avgPN)
RHS = (1 - carrier_initial_g) * avgPC
print('(1 - carrier_initial_g) PC:', RHS)
print(avgPN > RHS)
