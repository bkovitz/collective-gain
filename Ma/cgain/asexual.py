
import uniplot as up
import sys

########################
### MODEL PARAMETERS ###
########################

# maximum number of individuals that can fit in a habitat
habitat_base_capacity: float = 10.0

# maximum number of givers that can fit in a habitat
habitat_max_capacity: float = 100.0

# the ideal growth rate of a population consisting of 100% takers
base_growth_rate: float = 0.25

# the ideal growth rate of a population consisting of 100% givers
# (the number of children that a taker would have if placed in a population of 100% givers)
ideal_giver_growth_rate: float = 0.25

# death rate (same units as base growth rate, not affected by capacity!)
death_rate: float = 0.1

# prob. of gene-flipping mutation
mutation_prob: float = 0.05

# prob. that a giver-gene-carrier develops into an actual giver
giver_develop_prob: float = 0.2

# amount of carrying capacity increase produced by each giver
giver_capacity_increase: float = 0.0

# timestep delta in simulation
delta_t: float = 0.01

# number of generations to run
num_generations: int = 400

#####################
### INITIAL STATE ###
#####################

giver_pop: float = 0.0
taker_carrier_pop: float = 5.0
taker_noncarrier_pop: float = 5.0

def experiment0():
    '''The very first experiment. Result: givers get exploited but don't disappear.'''
    global habitat_base_capacity, habitat_max_capacity, base_growth_rate, ideal_giver_growth_rate, death_rate, mutation_prob, giver_develop_prob, \
           giver_capacity_increase, delta_t, giver_pop, taker_carrier_pop, taker_noncarrier_pop, num_generations
    habitat_base_capacity = 10.0
    habitat_max_capacity = 100.0
    base_growth_rate = 0.25
    ideal_giver_growth_rate = 0.25
    death_rate = 0.1
    mutation_prob = 0.05
    giver_develop_prob = 0.2
    giver_capacity_increase = 0.0
    delta_t = 0.01
    giver_pop = 0.0
    taker_carrier_pop = 5.0
    taker_noncarrier_pop = 5.0
    print("experiment0")

def experiment1():  # Only carriers; no mutation
    global habitat_base_capacity, habitat_max_capacity, base_growth_rate, ideal_giver_growth_rate, death_rate, mutation_prob, giver_develop_prob, \
           giver_capacity_increase, delta_t, giver_pop, taker_carrier_pop, taker_noncarrier_pop, num_generations
    habitat_base_capacity = 400.0
    habitat_max_capacity = 400.0
    base_growth_rate = 1.00
    ideal_giver_growth_rate = 81.00    # like 9x9 square of "spreading fecundity" in C++ experiment1
    death_rate = 1.0 - (20.0 / 400.0)  # keep population of 20 stable without givers
    # We verified experimentally in the C++ versions that the ratio of givers to the whole population over all generations was 0.047.
    mutation_prob = 0.00
    giver_develop_prob = 0.047
    giver_capacity_increase = 0.0
    delta_t = 0.01
    giver_pop = 0.0
    taker_carrier_pop = 20.0
    taker_noncarrier_pop = 0.0
    print("experiment1")

def experiment2():  # No mutation. Do the carriers die out? Yes in this one. Population fades after ~12,000 generations.
    global habitat_base_capacity, habitat_max_capacity, base_growth_rate, ideal_giver_growth_rate, death_rate, mutation_prob, giver_develop_prob, \
           giver_capacity_increase, delta_t, giver_pop, taker_carrier_pop, taker_noncarrier_pop, num_generations
    habitat_base_capacity = 400.0
    habitat_max_capacity = 400.0
    base_growth_rate = 1.00
    ideal_giver_growth_rate = 81.00    # like 9x9 square of "spreading fecundity" in C++ experiment1
    death_rate = 1.0 - (20.0 / 400.0)  # keep population of 20 stable without givers
    # We verified experimentally in the C++ versions that the ratio of givers to the whole population over all generations was 0.047.
    mutation_prob = 0.00
    giver_develop_prob = 0.1
    giver_capacity_increase = 10.0
    delta_t = 0.01
    giver_pop = 0.0
    taker_carrier_pop = 80.0
    taker_noncarrier_pop = 20.0
    num_generations = 20000
    print("experiment2")

def experiment3():  # No mutation. Do the carriers die out? Yes.
    global habitat_base_capacity, habitat_max_capacity, base_growth_rate, ideal_giver_growth_rate, death_rate, mutation_prob, giver_develop_prob, \
           giver_capacity_increase, delta_t, giver_pop, taker_carrier_pop, taker_noncarrier_pop, num_generations
    habitat_base_capacity = 400.0
    habitat_max_capacity = 400.0
    base_growth_rate = 1.00
    ideal_giver_growth_rate = 200.0
    death_rate = 1.0 - (20.0 / 400.0)  # keep population of 20 stable without givers
    # We verified experimentally in the C++ versions that the ratio of givers to the whole population over all generations was 0.047.
    mutation_prob = 0.00
    giver_develop_prob = 0.01
    giver_capacity_increase = 100.0
    delta_t = 0.01
    giver_pop = 0.0
    taker_carrier_pop = 1.0
    taker_noncarrier_pop = 20.0
    num_generations = 20000
    print("experiment3")

def experiment4():  # No mutation. Givers increase capacity but not growth rate. All carriers develop into givers. Do the carriers die out? Yes.
                    # It's at least an interesting curve, if nothing else.
    global habitat_base_capacity, habitat_max_capacity, base_growth_rate, ideal_giver_growth_rate, death_rate, mutation_prob, giver_develop_prob, \
           giver_capacity_increase, delta_t, giver_pop, taker_carrier_pop, taker_noncarrier_pop, num_generations
    habitat_base_capacity = 40.0
    habitat_max_capacity = 400.0
    base_growth_rate = 1.00
    ideal_giver_growth_rate = 1.0
    death_rate = 1.0 - (20.0 / habitat_base_capacity)  # keep population of 20 stable without givers
    # We verified experimentally in the C++ versions that the ratio of givers to the whole population over all generations was 0.047.
    mutation_prob = 0.00
    giver_develop_prob = 1.0
    giver_capacity_increase = 100.0
    delta_t = 0.01
    giver_pop = 0.0
    taker_carrier_pop = 1.0
    taker_noncarrier_pop = 19.0
    num_generations = 4000
    print("experiment4")

def experiment5():  # No mutation. Givers increase capacity but not growth rate. Do the carriers die out? Yes.
    global habitat_base_capacity, habitat_max_capacity, base_growth_rate, ideal_giver_growth_rate, death_rate, mutation_prob, giver_develop_prob, \
           giver_capacity_increase, delta_t, giver_pop, taker_carrier_pop, taker_noncarrier_pop, num_generations
    habitat_base_capacity = 40.0
    habitat_max_capacity = 400.0
    base_growth_rate = 1.00
    ideal_giver_growth_rate = 1.0
    death_rate = 1.0 - (20.0 / habitat_base_capacity)  # keep population of 20 stable without givers
    # We verified experimentally in the C++ versions that the ratio of givers to the whole population over all generations was 0.047.
    mutation_prob = 0.00
    giver_develop_prob = 0.05
    giver_capacity_increase = 10000.0
    delta_t = 0.01
    giver_pop = 0.0
    taker_carrier_pop = 1.0
    taker_noncarrier_pop = 19.0
    num_generations = 20000
    print("experiment5")

# change this to run a different experiment
experiment5()

################
### DYNAMICS ###
################

history: list[tuple] = []

for step in range(num_generations + 1):
    # print summary
    reproductive_pop: float = taker_carrier_pop + taker_noncarrier_pop
    total_pop: float = giver_pop + reproductive_pop
    #print(step,giver_pop,taker_carrier_pop,taker_noncarrier_pop,total_pop)
    history.append((giver_pop,taker_carrier_pop,taker_noncarrier_pop,total_pop))

    # death and decay
    delta_giver_death: float = giver_pop*death_rate*delta_t
    delta_taker_carrier_death: float = taker_carrier_pop*death_rate*delta_t
    delta_taker_noncarrier_death: float = taker_noncarrier_pop*death_rate*delta_t

    # asexual reproduction
    growth_rate: float = (giver_pop/total_pop)*ideal_giver_growth_rate + (reproductive_pop/total_pop)*base_growth_rate
    habitat_capacity: float = min(habitat_base_capacity + giver_capacity_increase*giver_pop, habitat_max_capacity)
    delta_growth: float = (1 - total_pop/habitat_capacity)*growth_rate*reproductive_pop*delta_t
    carrier_parent_prob: float = taker_carrier_pop/reproductive_pop
    noncarrier_parent_prob: float = taker_noncarrier_pop/reproductive_pop
    prob_giver: float = carrier_parent_prob*(1-mutation_prob)*giver_develop_prob + noncarrier_parent_prob*mutation_prob*giver_develop_prob
    prob_taker_carrier: float = carrier_parent_prob*(1-mutation_prob)*(1-giver_develop_prob) + noncarrier_parent_prob*mutation_prob*(1-giver_develop_prob)
    prob_taker_noncarrier: float = carrier_parent_prob*mutation_prob + noncarrier_parent_prob*(1-mutation_prob)

    # updates
    giver_pop = giver_pop + prob_giver*delta_growth - delta_giver_death
    taker_carrier_pop = taker_carrier_pop + prob_taker_carrier*delta_growth - delta_taker_carrier_death
    taker_noncarrier_pop = taker_noncarrier_pop + prob_taker_noncarrier*delta_growth - delta_taker_noncarrier_death

    #if taker_noncarrier_pop > 20.0:
    print("delta_giver_death", delta_giver_death)
    print("delta_taker_carrier_death", delta_taker_carrier_death)
    print("delta_taker_noncarrier_death", delta_taker_noncarrier_death)
    print("growth_rate", growth_rate)
    print("habitat_capacity", habitat_capacity)
    print("delta_growth", delta_growth)
    print("carrier_parent_prob", carrier_parent_prob)
    print("noncarrier_parent_prob", noncarrier_parent_prob)
    print("prob_giver", prob_giver)
    print("prob_taker_carrier", prob_taker_carrier)
    print("prob_taker_noncarrier", prob_taker_noncarrier)
    print()
        #sys.exit(2)



##########
# REPORT #
##########

print(step, giver_pop, taker_carrier_pop, taker_noncarrier_pop, total_pop, taker_noncarrier_pop/total_pop)

carrier_plot1 = [entry[0]/(entry[0]+entry[1]+0.00001) for entry in history]
carrier_plot2 = [entry[1]/(entry[0]+entry[1]+0.00001) for entry in history]
up.plot((carrier_plot1,carrier_plot2), title="Giver/Total, Carrier/Total")

taker_plot1 = [entry[0] for entry in history]
taker_plot2 = [entry[1] for entry in history]
taker_plot3 = [entry[2] for entry in history]
up.plot((taker_plot1,taker_plot2,taker_plot3), title="Givers (blue), Carriers (magenta), Noncarriers (green)")

cratio_plot = [(entry[2])/(entry[0]+entry[1]+entry[2]) for entry in history]
up.plot(cratio_plot, title="Noncarriers / Total pop.")

nc_plot = [entry[2] for entry in history]
up.plot(nc_plot, title="Noncarriers")

print("END")
