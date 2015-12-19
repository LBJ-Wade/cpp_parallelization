#include <iostream>
#include <cmath>
#include "periodicArray.h"

#define N 512

int main(void) {

  int i, j, k, s;

  std::cout << "Initializing array...\n";
  cosmo::periodicArray<int, double> pa;
  pa.init(N, N, N); // for ACC, data should now exist on device
  double * const _pa = pa._array;

  std::cout << "Running loop...\n";
  for(s=0; s<100; s++) {
    #if CONFIG_OMP_3D
      #pragma omp parallel for default(shared) private(i, j, k) num_threads(32)
    #endif
    #if CONFIG_ACC_3D
      #pragma acc parallel loop independent present(pa, _pa)
    #endif
    for(i=0; i<N; i++) {
      #if CONFIG_OMP_2D
        #pragma omp parallel for default(shared) private(j, k) num_threads(32)
      #endif
      #if CONFIG_ACC_2D
        #pragma acc parallel loop independent present(pa, _pa)
      #endif
      #if CONFIG_ACC_3D
        #pragma acc loop independent
      #endif
      for(j=0; j<N; j++) {
        #if CONFIG_ACC_3D
          #pragma acc loop independent
        #endif
        #if CONFIG_ACC_2D
          #pragma acc loop independent
        #endif
        for(k=0; k<N; ++k) {
          int idx = pa.idx(i, j, k);
          // This should just iteratively solve the following 
          // equation for x (when "0" gives a convergent initial guess):
          // x = sqrt(x) + i*j*k
          _pa[idx] = std::sqrt(_pa[idx]) + (double) i*j*k;
          // directly calling pa(i,j,k) is not yet 
          // supported by PGI compilers
        }
      }
    }
  }

  std::cout << "Returning data to host...\n";
  pa.updateHost();
  std::cout << "An array value is: \n"
            << " pa(1, 5, 7) = " << pa(1, 5, 7) << "\n"
            << " analytic expectation is: " << 1.0/2*(1 + 2*(1*5*7) + sqrt(1 + 4*(1*5*7))) << "\n";

  return 0;
}
