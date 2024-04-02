#!/bin/bash

sbatch --array=1-100 -J BIG/M75_N300_U1_1000  job-big.sbatch
sbatch --array=1-100 -J BIG/M100_N400_U1_1000 job-big.sbatch
