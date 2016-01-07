# C++ Parallelization Tests

This repository contains some basic c++ code, set up to run in parallel on either multiple CPU cores or a GPU. The idea is to not only parallelize the code, but do so using some modern c++11 features, wrapping OpenACC directives in class methods.

The code creates a large "3D" array (lattice), and uses an iterative method to solve an algebraic problem for each point in the lattice. Values calculated and stored at points on the lattice have no dependency on other lattice points, so parallelization can be done in several ways. A comparison can then be made between execution speeds for different parallelization schemes.

## Running the code

### Initial setup

#### Running on the HPCC at Case Western (hpclogin.cwru.edu)

The code should work on the hpctest node at CWRU. In order to run this code, first log in to hpctest:
```{r, engine='bash', hpctest_login}
$ ssh hpctest
```
And then request a node with a GPU:
```{r, engine='bash', hpctest_gpureq}
$ srun --gres=gpu:1 -c 12 -N 1 -p gpufermi --pty /bin/bash
```
Finally, the following modules should be loaded so the code can compile:
```{r, engine='bash', loadgcc}
$ module load gcc/4.9.3
```
```{r, engine='bash', loadpgi}
$ module load pgi/15.10
```
```{r, engine='bash', loadcuda}
$ module load cuda/7.0.28
```

#### Running on Ann (ann.kenyon.edu)

Ann is currently configured so that this code will run as-is.

### Downloading and compiling

The code can be cloned from github:
```{r, engine='bash', clone}
$ git clone https://github.com/cwru-pat/cpp_parallelization.git && cd cpp_parallelization
```
(If you intend to develop on the HPCC at CWRU, a newer version of git may be helpful too; eg. `$ module load git/2.4.8`)

The code can then be compiled using either the PGI compiler or GCC. Compile statements should look similar to:
```{r, engine='bash', pgicompile}
$ pgc++ -std=c++11 -O4 -fast -acc -Minfo=accel main.cpp -o main -DCONFIG_ACC_3D=true
```
Where the `CONFIG_ACC_3D` variable will enable one of a few parallelizations options, as described [in the code](https://github.com/cwru-pat/cpp_parallelization/blob/master/main.cpp#L26).
For GCC, a compile statement might look like
```{r, engine='bash', gcccompile}
g++ -std=c++11 -O3 -ffast-math -fopenmp main.cpp -o main -DCONFIG_OMP_3D=true
```

Once compiled, the executable `main` can then be run and timed, eg. using a command like `$ time ./main`.

The script `$ ./run_tests.sh` will compile the code using a variety of options and will print some timing information.

## Results

Some timing information from Ann suggests parallelization is highly beneficial. Ann has a Tesla K40m graphics card, and 32 Intel(R) Xeon(R) CPU E5-2650 v2 @ 2.60GHz CPUs. Code was compiled using `pgc++ 15.10-0 64-bit target on x86-64 Linux -tp sandybridge` and `g++ (Ubuntu 4.8.5-2ubuntu1~14.04.1) 4.8.5`. Timing results follow:

- PGI OpenACC 3D parallelization:
  - 0m4.900s
- PGI OpenACC 2D parallelization:
  - 0m6.937s
- PGI OpenMP 3D parallelization:
  - 0m5.697s
- PGI OpenMP 2D parallelization:
  - 0m7.287s
- PGI no parallelization:
  - 1m12.434s
- GCC OpenMP 3D parallelization:
  - 0m5.341s
- GCC OpenMP 2D parallelization:
  - 0m7.409s
- GCC no parallelization:
  - 1m58.419s

Nodes on the CWRU HPCC also strongly benefit from parallelization, to a larger extent than Ann for this code. GPU nodes on the cluster have Tesla M2090 cards, and 12 Intel(R) Xeon(R) CPU X5650  @ 2.67GHz CPUs.  Code was compiled using `pgc++ 15.10-0 64-bit target on x86-64 Linux -tp nehalem` and `g++ (GCC) 4.9.3` (and `num_threads(12)` specified). Timing results follow:

- PGI OpenACC 3D parallelization:
  - 0m1.044s
- PGI OpenACC 2D parallelization:
  - 0m1.492s
- PGI OpenMP 3D parallelization:
  - 0m14.097s
- PGI OpenMP 2D parallelization:
  - 0m14.539s
- PGI no parallelization:
  - 2m42.114s
- GCC OpenMP 3D parallelization:
  - 0m14.065s
- GCC OpenMP 2D parallelization:
  - 0m14.383s
- GCC no parallelization:
  - 2m40.087s

For hardware and a code similar to this on ann(.kenyon.edu), GPU speedups may not be too significant compared to just running on many cpus. For nodes such as those on the HPCC at CWRU with fewer cores and less cache, speedups may be significantly larger.
