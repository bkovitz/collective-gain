from typing import List, Tuple
import numpy as np

from simulation import Experiment, Simulation
from visualize import Visualize

e: Experiment = Experiment(
    seed=31337,
    generations=200,
    capacity=1000,
    initial_organism_count=1000,
    initial_carrier_count=0,
    giving_radius=np.float64(0.093),
    giver_development_probability=np.float64(0.10),
    child_placement_radius=np.float64(0.093),
    max_number_of_children=10,
    base_survival_probability=np.float64(0.105),
    giver_survival_bonus=np.float64(0.10)
)

s: Simulation = Simulation(e)
s.run()

visualize: Visualize = Visualize(image_size=256, output_directory="only-noncarriers")
visualize.simulation(s)
