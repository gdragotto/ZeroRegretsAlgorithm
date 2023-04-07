# Quadratic Game
The instances associated with this game are split in convex and non-convex.
We remark all the matrices are saved with [Armadillo](https://arma.sourceforge.net) and can be equivalently read with [Pyarma](https://pyarma.sourceforge.io).

## Convex Instances

We generate the convex instances following the procedure detailed in [Computing All Solutions of Nash Equilibrium Problems with Discrete Strategy Sets](https://epubs.siam.org/doi/10.1137/15M1052445). 
For each instance, we save a series of file with the prefix _Quadratic-n-m-bType-iType_ where:
- _n_ is the number of players
- _m_ is the number of variables per player
- _bType_ is either "A" or "B". If "A", the bounds (upper and lower) are multiplied by 2. Otherwise, the bounds are kept as in the instance generator input
- _iType_ is either "H" or "L" (see the original paper). If "H", the entries of Q are multiplied by 0.1 in the generation process, otherwise they are multiplied by 0.01.

For each instance and for each player _i_, we report the following files:
- _prefix-C-i_ is the matrix representing the bilinear interaction terms
- _prefix-c-i_ is the vector representing the linear terms
- _prefix-Q-i_ is the vector representing the quadratic terms
- _prefix.txt_ contains the instance information. Specifically, on the first line it contains _n_ and _m_ while, on the second line, it contains the upper and lower bounds on the variables. We remark the bounds are equal for all the variables (i.e., the problem is box-constrained)


## Non-convex Instances

The non-convex instances were originally released [here](https://github.com/schwarze-st/nep_pruning). We refer to the paper (Section 5) for the detailed description of the instances. The format is analogous to the convex instances with the exception that the prefix corresponds to the original instance file names.