#!/bin/bash

echo "A quick test script comparing GPU and CPU speeds on Ann."
echo "pgc++ compiler version:"
pgc++ --version
echo ""
echo "g++ compiler version:"
g++ --version

echo "Compiling using pgc++, 3D ACC parallelization"
rm -f main
pgc++ -std=c++11 -O4 -fast -acc -Minfo=accel main.cpp -o main -DCONFIG_ACC_3D=true
echo "Running..."
exec 3>&1 4>&2
PGIACC3D=$( { time ./main 1>&3 2>&4; } 2>&1 )
exec 3>&- 4>&-
echo "Timing information: $PGIACC3D"

echo "Compiling using pgc++, 2D ACC parallelization"
rm -f main
pgc++ -std=c++11 -O4 -fast -acc -Minfo=accel main.cpp -o main -DCONFIG_ACC_2D=true
echo "Running..."
exec 3>&1 4>&2
PGIACC2D=$( { time ./main 1>&3 2>&4; } 2>&1 )
exec 3>&- 4>&-
echo "Timing information: $PGIACC2D"

echo "Compiling using pgc++, 3D OpenMP parallelization"
rm -f main
pgc++ -std=c++11 -O4 -fast -mp main.cpp -o main -DCONFIG_OMP_3D=true
echo "Running..."
exec 3>&1 4>&2
PGIOMP3D=$( { time ./main 1>&3 2>&4; } 2>&1 )
exec 3>&- 4>&-
echo "Timing information: $PGIOMP3D"

echo "Compiling using pgc++, 2D OpenMP parallelization"
rm -f main
pgc++ -std=c++11 -O4 -fast -mp main.cpp -o main -DCONFIG_OMP_2D=true
echo "Running..."
exec 3>&1 4>&2
PGIOMP2D=$( { time ./main 1>&3 2>&4; } 2>&1 )
exec 3>&- 4>&-
echo "Timing information: $PGIOMP2D"

echo "Compiling using pgc++, no parallelization"
rm -f main
pgc++ -std=c++11 -O4 -fast main.cpp -o main
echo "Running..."
exec 3>&1 4>&2
PGINOP=$( { time ./main 1>&3 2>&4; } 2>&1 )
exec 3>&- 4>&-
echo "Timing information: $PGINOP"


echo "Compiling using g++, 3D OpenMP parallelization"
rm -f main
g++ -std=c++11 -O3 -ffast-math -fopenmp main.cpp -o main -DCONFIG_OMP_3D=true
echo "Running..."
exec 3>&1 4>&2
GCCOMP3D=$( { time ./main 1>&3 2>&4; } 2>&1 )
exec 3>&- 4>&-
echo "Timing information: $GCCOMP3D"

echo "Compiling using g++, 2D OpenMP parallelization"
rm -f main
g++ -std=c++11 -O3 -ffast-math -fopenmp main.cpp -o main -DCONFIG_OMP_2D=true
echo "Running..."
exec 3>&1 4>&2
GCCOMP2D=$( { time ./main 1>&3 2>&4; } 2>&1 )
exec 3>&- 4>&-
echo "Timing information: $GCCOMP2D"

echo "Compiling using g++, no parallelization"
rm -f main
g++ -std=c++11 -O3 -ffast-math -fopenmp main.cpp -o main
echo "Running..."
exec 3>&1 4>&2
GCCNOP=$( { time ./main 1>&3 2>&4; } 2>&1 )
exec 3>&- 4>&-
echo "Timing information: $GCCNOP"

rm -f main

echo ""
echo "---------------------------"
echo "Timing information summary:"
echo "---------------------------"
echo ""

echo "PGI OpenACC 3D parallelization times: $PGIACC3D"
echo "PGI OpenACC 2D parallelization times: $PGIACC2D"
echo "PGI OpenMP 3D parallelization times: $PGIOMP3D"
echo "PGI OpenMP 2D parallelization times: $PGIOMP2D"
echo "PGI no parallelization times: $PGINOP"
echo "GCC OpenMP 3D parallelization times: $GCCOMP3D"
echo "GCC OpenMP 2D parallelization times: $GCCOMP2D"
echo "GCC no parallelization times: $GCCNOP"
