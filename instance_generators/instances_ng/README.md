# Network Game
We report the instance file for the network formation game. We remark all the matrices are saved with [Armadillo](https://arma.sourceforge.net) and can be equivalently read with [Pyarma](https://pyarma.sourceforge.io).


## Instance format

For each instance with _n'_ players and _m_ nodes, we save 2 files:
- _G_n_n'-grid.adj_ is the file (Armadillo) containing the adjacency matrix of the graph
- _SSTT_n_n'-grid.adj_ contains the matrix with sources and destinations for each player. Specifically, the first and second column of row _i_ are the source and sink node id for player _i_