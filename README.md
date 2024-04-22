# Divide and Conquer Approach for Scheduling Problems on Uniform Machines

This is the implementation to my masters thesis at Kiel University.

## Description

This project determines for a given scheduling instance the optimal makespan. If a makespan is also given to the program, it checks the feasibility of the instance with respect to the makespan. 

## Setup 

Clone this repository to your desktop and install all dependencies. Make sure you have the gnu-compiler gcc version 8.5.0 20210514 or higher installed.

We used the operating system: Rocky Linux 8.6 (Green Obsidian).

## Usage

### Linux

Once the dependencies are installed, you enter the `bin` folder from the shell and type `make` to compile and link the program. Now type `./uniformsched` with filename of the scheduling instance as a parameter (e.g. `./uniformsched /test.dat`. Add a number $T$ as an additional parameter to determine the feasibility of the instance with respect to the makespan $T$ (e.g. `./uniformsched /test.dat 5`).

The program creates a log file in the output folder and automatically stops when the calculation is finished. 

## Directory Tree

- code
  - bin
  - c
  - include
  - instances_uniform
    - BIG
    - E1
    - E2
    - E3
    - E4
  - obj
  - output
    - E1
    - E2

## Author

Lis Pirotton, 2023

