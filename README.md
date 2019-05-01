# EnergyRelay
Simulation code for the paper ["Energy-efficient outage-constrained power allocation based on statistical channel knowledge for dual-hop cognitive relay networks"](https://onlinelibrary.wiley.com/doi/abs/10.1002/dac.2965).

## Abstract
This paper investigates the power allocation problem in decode‐and‐forward cognitive dual‐hop systems over Rayleigh fading channels. In order to optimize the performance of the secondary network in terms of power consumption, an outage‐constrained power allocation scheme is proposed. The secondary nodes adjust their transmit power subject to an average interference constraint at the primary receiver and an outage probability constraint for the secondary receivers while having only statistical channel knowledge with respect to the primary nodes. We compare this approach with a power allocation scheme based on instantaneous channel state information under a peak interference constraint. Analytical and numerical results show that the proposed approach, without requiring the constant interchange of channel state information, can achieve a similar performance in terms of outage probability as that of power allocation based on instantaneous channel knowledge. Moreover, the transmit power allocated by the proposed approach is considerably smaller than the power allocated by the method based on instantaneous channel knowledge in more than 50% of the time.

## Getting Started
In order to run the simulations you need Matlab 2015a or higher and a C compiler compatible with the installed Matlab version. From the command line type:
```bash
git https://github.com/raikel/EnergyRelay
```
Open Matlab and add the source directory `src` (and all its subfolders) to the Matlab search path. In the Matlab workspace, open the directory `src\lib\mex` and type in the command window:
```bash
compile
```
This will compile all the source `mex` files. To run the simulation with default parameters values, type in the Matlab command window:
```Matlab
stats = netsim()
```

## Cite
Please cite this work as
> Bordón, R., Sánchez, S. M., Mafra, S. B., Garcia Fernandez, E. M., Souza, R. D., & Rebelatto, J. L. (2017). Energy‐efficient outage‐constrained power allocation based on statistical channel knowledge for dual‐hop cognitive relay networks. International Journal of Communication Systems, 30(3), e2965.
