
import uniplot as up

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

#####################
### INITIAL STATE ###
#####################

giver_pop: float = 0.0
taker_carrier_pop: float = 5.0
taker_noncarrier_pop: float = 5.0

################
### DYNAMICS ###
################

history: list[tuple] = []

for step in range(25000):
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


##########
# REPORT #
##########

print(step,giver_pop,taker_carrier_pop,taker_noncarrier_pop,total_pop,taker_noncarrier_pop/total_pop)

carrier_plot1 = [entry[0]/(entry[0]+entry[1]+0.00001) for entry in history]
carrier_plot2 = [entry[1]/(entry[0]+entry[1]+0.00001) for entry in history]
up.plot((carrier_plot1,carrier_plot2), title="Giver/Total, Carrier/Total")

taker_plot1 = [entry[0] for entry in history]
taker_plot2 = [entry[1] for entry in history]
taker_plot3 = [entry[2] for entry in history]
up.plot((taker_plot1,taker_plot2,taker_plot3), title="Givers, Carriers, Noncarriers")

cratio_plot = [(entry[2])/(entry[0]+entry[1]+entry[2]) for entry in history]
up.plot(cratio_plot, title="Noncarriers / Total pop.")

nc_plot = [entry[2] for entry in history]
up.plot(nc_plot, title="Noncarriers")

print("END")
