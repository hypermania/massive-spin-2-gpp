# massive-spin-2-gpp
Code for solving mode equations for a massive spin-2 field on an FRW background.

This Mathematica/C++ codebase was used to generate the numerical results in the paper ["Cosmological gravitational particle production of massive spin-2 particles"](https://arxiv.org/abs/2302.04390). 

For tensor/vector/scalar sectors of the "nonminimal" theory and tensor/vector sectors of the "minimal" theory, mode equations were solved by running GPP_compute.nb and results were saved to */Dat*.

For scalar sector of the "minimal" theory, mode equations were solved by running the C++ program under */MinimalScalar*. Simply run `make` to compile the project using the GNU Make utility.
