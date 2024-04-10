# particle_simulation-OpenMP
Parallelizing a toy particle simulation (similar simulations are used in mechanics, biology, and astronomy). In our simulation, particles interact by
repelling one another.
The particles repel one another, but only when closer than a cutoff distance.
##CMakeLists.txt:
The build system that manages compiling your code.
##main.cpp
A driver program that runs your code.
##common.h
A header file with shared declarations
##job-openmp
A sample job script to run the OpenMP executable
##job-serial
A sample job script to run the serial executable
##serial.cpp
An O(n) serial algorithm within the simulate_one_step function.
##openmp.cpp
 OpenMP simulation algorithm.
