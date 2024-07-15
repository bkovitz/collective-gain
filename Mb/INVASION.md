# Notes on non-carrier invasion

Here I experiment with 1 non-carrier invading a population of 19 carriers.

## Basic experiment

The basic experiment uses the following parameters.
```
--initial_g=0.00 \
--num_gens=800 \
--xsize=30 \
--ysize=30 \
--base_fecundity=1 \
--seed=${i} \
--acorn_stdev=2 \
--base_carrying_capacity=0.09 \
--giving_radius=2 \
--laying_radius=2 \
--cc_delta=0.40 \
--mutation_stdev=0.00 \
--num_initial_carriers=19 \
--carrier_initial_g=0.05 \
--num_initial_organisms=20
```

**Observations**

* In 2 out of 50 attempts, the invading noncarriers prevail.
* In 48 out of 50 attempts, the resident carriers prevail.
* When the invading noncarriers prevail, it takes 133 generations to reach
  a homogeneous population of noncarriers.
* When the carriers prevail, on average it takes 32.7 generations to reach
  a homogeneous population of carriers.

## Ideal invaders

The observations show that small carrier populations are fairly resilient,
but not completely immune to invading noncarriers. But perhaps it's just
really difficult for invaders to succeed?

I want to estabilish a baseline point of comparison: would an invader with
no real genetic differences be successful? Alas, that's not easy to test
with the current Mb setup, since carriers always have a lower base reproductive
number that noncarriers.

So instead, I investigate the following question: how successful would a
single invader be if the givers had no advantage, i.e. if the givers didn't do
anything.

I use the following parameters, setting `cc_delta` to zero.
```
--initial_g=0.00 \
--num_gens=800 \
--xsize=30 \
--ysize=30 \
--base_fecundity=1 \
--seed=${i} \
--acorn_stdev=2 \
--base_carrying_capacity=0.09 \
--giving_radius=2 \
--laying_radius=2 \
--cc_delta=0.00 \
--mutation_stdev=0.00 \
--num_initial_carriers=19 \
--carrier_initial_g=0.05 \
--num_initial_organisms=20
```

**Observations**

* In 17 out of 50 attempts, the invading noncarriers prevail.
* In 33 out of 50 attempts, the resident carriers prevail.
* Invading noncarriers prevail on average in 147.12 generations.
* Resident carriers prevail on average in 12.6 generations.

Based on this, it does appear to be fairly difficult for invaders to prevail,
even if they're obviously superior to the residents. I wager this is due to
early generation randomness: they're simply very likely to die out early on
due to random effects. Based on this, I can't rule out that the invaders are
superior even in the basic experiment. Also, more evidence that models with
randomness obscure the dynamics.

## Balanced experiment w/ basic noncarriers

I wonder now, what happens if I do the basic experiment again, but this time
I start with the same number of initial carriers and noncarriers.

I increae the initial population to 40, to make random extinctions less likely.

```
--initial_g=0.00 \
--num_gens=800 \
--xsize=30 \
--ysize=30 \
--base_fecundity=1 \
--seed=${i} \
--acorn_stdev=2 \
--base_carrying_capacity=0.09 \
--giving_radius=2 \
--laying_radius=2 \
--cc_delta=0.40 \
--mutation_stdev=0.00 \
--num_initial_carriers=20 \
--carrier_initial_g=0.05 \
--num_initial_organisms=40
```

**Observations**

* In 7 out of 50 attempts, the noncarriers prevail.
* In 43 out of 50 attempts, the carriers prevail.
* Noncarriers prevail on average in 104 generations.
* Carriers prevail on average in 179 generations.

Okay, it looks like carriers prevail fairly reliably, so they indeed have
higher reproductive number. The discrepancy in generations suggests also
that noncarrier prevail is due to early "freak" events.

## Balanced experiment w/ ideal noncarriers

The same as before, except with useless givers, ideal noncarriers. This
time we should see noncarriers triumph.

```
--initial_g=0.00 \
--num_gens=800 \
--xsize=30 \
--ysize=30 \
--base_fecundity=1 \
--seed=${i} \
--acorn_stdev=2 \
--base_carrying_capacity=0.09 \
--giving_radius=2 \
--laying_radius=2 \
--cc_delta=0.00 \
--mutation_stdev=0.00 \
--num_initial_carriers=20 \
--carrier_initial_g=0.05 \
--num_initial_organisms=40
```

**Observations**

* In 49 out of 50 attempts, the noncarriers prevail.
* In 1 out of 50 attemps, the carriers prevail.
* Noncarriers prevail on average in 94.8 generations.
* Carriers prevail in 70 generations (one case, exact).

Okay, the vastly superior noncarriers take over easily, as we expect. Again,
the weaker carrier's prevail happens too fast, suggesting "freak" events as
the cause.


## Balanced parameter sweep

It's time to determine which giving radii are large enough to cause the
carriers to lose their kin selected advantage.

We know that 15 is sufficient, so we'll try 8 first, then subdivide the
range as necessary. I'll tabulate the number of attempts won by the carriers
below, but won't compute the average number of generations.

```
--initial_g=0.00 \
--num_gens=3200 \
--xsize=30 \
--ysize=30 \
--base_fecundity=1 \
--seed=${i} \
--acorn_stdev=2 \
--base_carrying_capacity=0.09 \
--giving_radius=VARIABLE \
--laying_radius=2 \
--cc_delta=0.40 \
--mutation_stdev=0.00 \
--num_initial_carriers=20 \
--carrier_initial_g=0.05 \
--num_initial_organisms=40
```

**Results**

The results are interesting.

* For a giving radius of 2, either the carriers or noncarriers
  always prevail after 400ish generations, with the vast majority
  of wins going to the carriers.
* For a giving radius of 3, again the carriers or noncarriers prevail.
  The vast majority of wins go to the carriers (even vaster than
  previously, not sure if that's meaningful or a fluke).
* Radius 5 or above, it all goes to the noncarriers. The higher the radius,
  the faster the convergence.
* For a giving radius of 4, we see failure to converge: even after
  a whopping 3200 generations, 29 attempts have positive carrier ratios,
  although notably the carriers never seem to prevail. This could be
  slow convergence, close to the equilibrium where the reproductive numbers
  become perfectly balanced, and therefore the allele ratios in the population
  remain constant. Running it even for 6400 steps only causes 6 of
  the remaining 29 attempts to converge, but the noncarriers do prevail.


```
giving_radius        carriers_win/50        noncarriers_win/50
2                    43                     7
3                    49                     1
4                    0                      21
5                    0                      50
6                    0                      50
...
```
