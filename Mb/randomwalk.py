import numpy as np

world = np.ones(400) * 0.01
for gen in range(10000):
    print(np.mean(world))
    world += np.random.normal(loc=0.0, scale=0.01, size=400)
    world = np.maximum(world, np.zeros(400))
    world = np.minimum(world, np.ones(400))
    #print(world[:3])
