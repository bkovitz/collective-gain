from typing import List, Tuple
import numpy as np

from simulation import Experiment, Simulation
from visualize import Visualize

e: Experiment = Experiment(
    seed=31341,
    generations=300,
    capacity=200,
    initial_organism_count=50,
    initial_carrier_count=25,
    giving_radius=np.float64(0.2),
    giver_development_probability=np.float64(0.05),
    child_placement_radius=np.float64(0.1),
    max_number_of_children=10,
    base_survival_probability=np.float64(0.11),
    giver_survival_bonus=np.float64(0.1)
)

s: Simulation = Simulation(e)
s.run()

visualize: Visualize = Visualize(
    image_size=256,
    output_directory="viz_example_invasion"
)
visualize.simulation(s)

# Usage:
#  mkdir viz_example_invasion
#  poetry run python3 example_invasion.py
#  ffmpeg -framerate 10 -i viz_example_invasion/%04d.png -vf "scale=500:-1:flags=lanczos" -loop 0 output.gif
