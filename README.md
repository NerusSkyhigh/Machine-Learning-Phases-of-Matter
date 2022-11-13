# Machine Learning phases of matter
## Abstract
This project aims to reobtain the results presented in [Machine learning phases of matter (Juan Carrasquilla, Roger G. Melko)](https://arxiv.org/abs/1605.01735) regarding the two-dimensional square Ising model. The authors show that a standard _feed-forward neural network_ (FFNN) is a suitable tool to detect Ising's model phase transition.  
This is done by first comparing the behavior of a _fully connected neural networr_ (FCNN) to a hand-made Toy model. Then the number of parameters of the FCNN is increased and the accuracy of the model is studied as a function of the lattice size to remove boundary effects.  
In the original article, the FCNN is upgraded to a _convolutional neural network_ to test its accuracy on non-Ising Hamiltonians. In this project, the convolutional network is applied only to the standard Ising model but the results of the article are reported with some considerations.
Finally, as an appendix, the original data is analyzed with the _t-stochastic neighbors embedding_ algorithm to separate the data into clusters.

> :grey_exclamation: The project is subdivided into notebook written to be run on [google colab](https://colab.research.google.com/). A copy is provided in this repository but they may not run in your local environment. It is possible to access them using the link provided.

## Notebooks
### 01 Monte Carlo simulation
[Google colab link](https://colab.research.google.com/drive/1de9nkvQpTHr8AZuaSgxCjmR14VqJ5I4F?usp=sharing)

The first notebook is centered around the Monte Carlo code `MonteCarloSampling.c` written to generate the configurations.
In particular, in the notebook, the behavior of the magnetization and the autocorrelation will be analyzed. Both tests will lead to positive results with a correlation length of $5000$ time steps.


### 02 The Toy model
TBA

## Bibliography
- Article: [Machine learning phases of matter](https://www.nature.com/articles/nphys4035), [arxiv version](https://arxiv.org/abs/1605.01735) available in the repository.
- Pytorch tutorials: [https://pytorch.org/tutorials/](https://pytorch.org/tutorials/)
