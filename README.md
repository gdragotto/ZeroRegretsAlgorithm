# ZERO Regrets
_Zero Regrets_ is an algorithm to optimize over the set of pure Nash equilibria in Integer Programming Games. 
This repository contains the instances, the results, and the instructions to request and run the code.

If you work with Zero Regrets, please consider citing our paper:


* [The Zero Regrets Algorithm: Optimizing over Pure Nash Equilibria via Integer Programming](https://arxiv.org/abs/2111.06382):

  ```
  @article{Dragotto_2021_ZERORegrets,
    title        = {{The Zero Regrets Algorithm: Optimizing over Pure Nash Equilibria via Integer Programming}},
    author       = {Dragotto, Gabriele and Scatamacchia, Rosario},
    year         = 2021,
    journal      = {arXiv},
    volume       = {abs/2111.06382},
    url          = {https://arxiv.org/abs/2111.06382},
    eprinttype   = {arXiv}
  }
  ```

## Code

### Original code
The directory *tests/bin* contains the binary files for the solvers. We provide a binary for each instance type (e.g., Knapsack Game or Network Formation Game). The subdirectories contain the binaries compiled for 2 distributions:
- MacOS Ventura 13.4 - compiled with Apple clang 14.0.3 and Gurobi 10.0 on arm64-apple-darwin22.5.0
- Linux RedHat 4.18.0-425.3.1.el8.x86_64 - compiled with gcc 11.1 and Gurobi 10.0

To request a license file, [send us an email](mailto:gdragotto@princeton.edu?cc=gabrieledragotto@gmail.com;rosario.scatamacchia@polito.it&subject=Zero%20Regrets%20-%20License%20Request)

We provide also the running bash scripts in the same folder, and the instance generators (and respective instances) in the folder instance_generators.

#### How to run the code
- Move the binaries for your distribution in the same directory of _installGurobi.sh_
- Create the directory _gurobi_
- Run _./installGurobi.sh_
- Run your favorite bash script with the respective instance folder



### Open-source implementation
We also provide an open-source implementation of Zero Regrets via the package [ZERO](https://github.com/ds4dm/ZERO).
We reference to the package for the detailed documentation.   

## Instances
In each folder, we describe the specific instances' format. Please refer to the readme in each subfolder.

## Results
In the folder _results_, we include the csv files with the computational results reported in the paper.

## Dependencies
We employ the following open-source packages:
- [Armadillo](https://arma.sourceforge.net)
- [iGraph](http://igraph.org)



