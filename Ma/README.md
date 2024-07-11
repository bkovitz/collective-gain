# Zoltan's Collective Gain Model (GoldStd)

## Installation

1. The models use `poetry` for dependency management and packaging.
   Follow the instructions on the [Poetry website](https://python-poetry.org)
   to install `poetry` on your system.
2. Install dependencies by issuing `poetry install` in the model root dir.
3. Run the model using `poetry run python3 cgain/asexual.py`.

## Scenario Description

TODO: *explain the polyphenism assumption, givers and takers, givers don't
reproduce, etc.*

## One-habitat models

### Population model

TODO: *migrate this to use Unicode variable names for consistency*

The model consists of the following three real-valued quantities.

```
G: the number of givers in the current population
Tc: the number of takers who carry the giver gene in the current population
Tn: the number of takers who do not carry the giver gene in the current population
```

We let `P = G + Tc + Tn` denote the total population of the habitat, and set
`T = Tc + Tn` as the total reproductive population.

The number of givers determines the growth rate of the total population:
if there are no givers, the population grows at some base rate `b0`; as the ratio
of givers approaches `100%`, the base growth `b` linearly converges to some
higher rate parameter `b1`. In some configurations we set `b0=b1`, i.e. in
these cases the givers do not affect the growth rate.

The number of givers also determines the maximal carrying capacity of a habitat,
which we denote with `k`. A parameter `l` determines how much a single giver
increases the base capacity `K`.

```
b = (1 - G/P)b0 + (G/P)b1
k = K + lG
```

The evolution of the total population `P` is determined by the base growth and
the maximal carry capacity parameter `k` of the habitat in a logistic manner.

```
dP = (1 - P/k)bT dt
```

The evolution of the total population in turn determines the evolution of the
quantities `G`, `Tc` and `Tn` via one of three reproduction models:

1. asexual reproduction (binary fission);
2. haploid (monoploid) sexual reproduction; or
3. diploid sexual reproduction.

### Reproduction models

#### Asexual reproduction

In this model, our organisms reproduce asexually via binary fission.
Each child inherits the whole genome of its unique parent, and thus the child
becomes an exact clone of the parent, unless a low-probability mutation event
also occurs. The mutation probability is governed by a parameter `m`, and a
mutation event turns a giver gene carrier into a non-carrier and vice versa.

Finally, each carrier probabilistically develops into either a giver or a
taker. The probability of developing into a taker is determined by the
parameter `D`.

The probabilistic decision tree below summarizes this process and its
possible outcomes for a freshly created individual.

```
o Determine the identity of the parent uniformly randomly.
+-- (prob. Tc/T): parent carries the giver gene
|   o Did a mutation happen?
|   +-- (prob. m): yes; the child does not carry the giver gene and is Tn.
|   +-- (prob. 1-m): no; the child carries the giver gene.
|       o Did the child develop into a giver?
|       +-- (prob. D): yes; the child is G.
|       +-- (prob. 1-D): no; the child is Tc.
+-- (prob. Tn/T): parent does not carry the giver gene
    o Did a mutation happen?
    +-- (prob. m): yes; the child carries the giver gene.
    |   o Did the child develop into a giver?
    |   +-- (prob. D): yes; the child is G.
    |   +-- (prob. 1-D): no; the child is Tc.
    +-- (prob. 1-m): no; the child does not carry the giver gene and is Tn.
```

Based on this probabilistic decision tree, we deduce that a freshly created
individual
* belongs to `G` with probability `(Tc/T)(1-m)D + (Tn/T)mD`,
* belongs to `Tc` with probability `(Tc/T)(1-m)(1-D) + (Tn/T)m(1-D)`,
* belongs to `Tn` with probability `(Tc/T)m + (Tn/T)(1-m)`.

We can therefore create a deterministic model of this probabilistic process
simply by bucketing the newly born individuals (`dP`) into the three classes
proportionally to these probabilities. We get the equations below.

```
dG = (Tc/T)(1-m)D + (Tn/T)mD
dTc = (Tc/T)(1-m)(1-D) + (Tn/T)m(1-D)
dTn = (Tc/T)m + (Tn/T)(1-m)
```

## Multi-habitat models

TODO:WRITE

## Stochastic models

TODO:WRITE
