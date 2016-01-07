#include <iostream>
#include <cmath>
#include "periodicArray.h"

// Array size is N^3
#define N 512

int main(void) {

  // integer variables for iteration
  int i, j, k, s;

  std::cout << "Initializing periodicArray class...\n";
  // create a periodicArray class instance
  utils::periodicArray<int, double> pa;
  // Call the initialization routine.
  // When using ACC, this routine creates a reference to the class
  // and internal array on the device.
  pa.init(N, N, N);
  // create a variable referencing the internal array of the class.
  double * const _pa = pa._array;

  std::cout << "Running loop...\n";
  // Run 100 iterations
  for(s=0; s<100; s++)
  {
    // parallelization statements depend on variables set at compilation time.
    // Variables used are:
    //  - CONFIG_OMP_3D
    //    parallelize all 3 loops using OpenMP directives
    //  - CONFIG_OMP_2D
    //    parallelize only the inner 2 loops using OpenMP directives
    //  - CONFIG_ACC_3D
    //    parallelize all 3 loops using OpenACC directives
    //  - CONFIG_ACC_2D
    //    parallelize only the inner 2 loops using OpenACC directives
    // Parallelizing only the inner 2 loops may be desirable in some cases,
    // such as for nontrivial lattice integration routines.
    #if CONFIG_OMP_3D
      #pragma omp parallel for default(shared) private(i, j, k) num_threads(32)
    #endif
    #if CONFIG_ACC_3D
      #pragma acc parallel loop independent present(pa, _pa)
    #endif
    for(i=0; i<N; i++)
    {
      #if CONFIG_OMP_2D
        #pragma omp parallel for default(shared) private(j, k) num_threads(32)
      #endif
      #if CONFIG_ACC_2D
        #pragma acc parallel loop independent present(pa, _pa)
      #endif
      #if CONFIG_ACC_3D
        #pragma acc loop independent
      #endif
      for(j=0; j<N; j++)
      {
        #if CONFIG_ACC_3D
          #pragma acc loop independent
        #endif
        #if CONFIG_ACC_2D
          #pragma acc loop independent
        #endif
        for(k=0; k<N; ++k) {
          // As a demonstration, this should iteratively solve the following 
          // equation for x (when "0" gives a convergent initial guess):
          // x = sqrt(x) + i*j*k
          int idx = pa.idx(i, j, k);
          _pa[idx] = std::sqrt(_pa[idx]) + (double) i*j*k;
          // Directly calling pa(i,j,k) is not yet supported by PGI compilers,
          // but should be in the future. If the PGI compiler was installed
          // with the "unified memory evaluation package" installed, adding
          // the "-ta=tesla:managed" flag may help this.
          // See also: http://stackoverflow.com/questions/32190696/using-openacc-to-parallelize-nested-loops
        } // end k loop
      } // end j loop
    } // end i loop
  }

  std::cout << "Returning data to host...\n";
  pa.updateHost();
  std::cout << "A calculated value is: \n"
            << " pa(1, 5, 7) = " << pa(1, 5, 7) << "\n"
            << " analytic expectation is: " << 1.0/2*(1 + 2*(1*5*7) + sqrt(1 + 4*(1*5*7))) << "\n";

  return 0;
}
