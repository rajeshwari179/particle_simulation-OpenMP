# Particle Simulation with OpenMP

This project parallelizes a toy particle simulation using OpenMP, a parallel computing API for shared-memory multiprocessing. Such simulations find applications in various fields, including mechanics, biology, and astronomy. In this simulation, particles interact by repelling one another within a certain cutoff distance.

## Files

- **CMakeLists.txt**: Build system configuration for compiling the code.
- **main.cpp**: Driver program for executing the code.
- **common.h**: Header file containing shared declarations.
- **job-openmp**: Sample job script to run the OpenMP executable.
- **job-serial**: Sample job script to run the serial executable.
- **serial.cpp**: Contains an O(n) serial algorithm within the `simulate_one_step` function.
- **openmp.cpp**: Implementation of the OpenMP simulation algorithm.

## Instructions

### Building the Code

1. Ensure you have CMake installed on your system.
2. Clone the repository.
3. Navigate to the project directory.
4. Create a build directory: `mkdir build && cd build`.
5. Generate build files: `cmake ..`.
6. Compile the code: `make`.

### Running the Code

#### Serial Version

To run the serial version of the simulation, execute the following command:
```bash
./serial_simulation
