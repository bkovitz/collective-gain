from typing import NamedTuple, List, Tuple
import numpy as np

import cv2
import os

from simulation import Experiment, Simulation, World


class Visualize:
    def __init__(self, image_size: int, output_directory: str="viz") -> None:
        self.image_size = image_size
        self.output_directory = output_directory

    def world(self, iteration: int, giver_radius: int, world: World) -> None:
        img = np.full(
            (self.image_size, self.image_size, 3),
            (128,128,128),
            dtype=np.uint8
        )
        for row in world:
            x, y, is_alive, is_giver, is_carrier = row
            if not is_alive:
                continue
            px = int(x * self.image_size)
            py = int(y * self.image_size)
            color = (0, 0, 0) # black noncarrier
            if is_carrier:
                color = (255, 0, 0) # blue carrier
            if is_giver:
                color = (0, 0, 139) # red giver
            img = cv2.circle(img, (px, py), 1, color, -1)
            if is_giver:
                img = cv2.circle(
                    img,
                    (px, py),
                    giver_radius,
                    (255, 105, 180),
                    1
                ) # circle around giver (pink)
        filename = os.path.join(
            self.output_directory,
            f"{iteration:04d}.png"
        )
        cv2.imwrite(filename, img)

    def simulation(self, the_simulation: Simulation) -> None:
        the_experiment: Experiment = the_simulation.experiment
        the_radius: float = float(the_experiment.giving_radius)
        the_history: List[World] = the_simulation.history
        giver_radius: int = int(the_radius*self.image_size)
        
        for iteration, world in enumerate(the_history):
            self.world(
                iteration=iteration,
                giver_radius=giver_radius,
                world=world
            )
