# Machine Learning phases of matter

## Abstract
The aim of this project is to reobtain the results presented in [Machine learning phases of matter (Juan Carrasquilla, Roger G. Melko)](https://arxiv.org/abs/1605.01735), that is show that a standard feed-forward neural network (FFNN) can be trained to detect order parameter from raw configurations sampled with Monte Carlo.



## Data Generation - Monte Carlo 
The first step is to obtain the Ising model configurations via a Monte Carlo simulation.

## Data augmentation
The square lattice Ising model is invariant under 90° rotations and $Z_2$ symmetry. Therefore from a single configuration I can obtain other 7 valid configuration by rotating the image by 90°, 180°, 270° and inverting all the spins.
