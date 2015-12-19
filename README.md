# C++ Parallelization Tests

This repo contains some basic c++ code, set up to run in parallel on either multiple CPU cores or a GPU. The idea is to not only parallelize the code, but do so using some modern c++11 features, wrapping up some OpenACC directives in a class.

Some timing information suggests parallelization is good overall:
PGI OpenACC 3D parallelization:
  0m4.900s
PGI OpenACC 2D parallelization:
  0m6.937s
PGI OpenMP 3D parallelization:
  0m5.697s
PGI OpenMP 2D parallelization:
  0m7.287s
PGI no parallelization:
  1m12.434s
GCC OpenMP 3D parallelization:
  0m5.341s
GCC OpenMP 2D parallelization:
  0m7.409s
GCC no parallelization:
  1m58.419s

But for hardware and a code similar to this on ann(.kenyon.edu), GPU speedups may not be too significant compared to just running on many cpus.
