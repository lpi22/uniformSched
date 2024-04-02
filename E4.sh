#!/bin/bash

sbatch --array=1-100 -J E4/M2_N10_U1_20    job.sbatch
sbatch --array=1-100 -J E4/M2_N10_U1_100   job.sbatch
sbatch --array=1-100 -J E4/M2_N10_U20_50   job.sbatch
sbatch --array=1-100 -J E4/M2_N10_U50_100  job.sbatch
sbatch --array=1-100 -J E4/M2_N10_U100_200 job.sbatch
sbatch --array=1-100 -J E4/M2_N10_U100_800 job.sbatch
sbatch --array=1-100 -J E4/M3_N9_U1_20     job.sbatch
sbatch --array=1-100 -J E4/M3_N9_U1_100    job.sbatch
sbatch --array=1-100 -J E4/M3_N9_U20_50    job.sbatch
sbatch --array=1-100 -J E4/M3_N9_U50_100   job.sbatch
sbatch --array=1-100 -J E4/M3_N9_U100_200  job.sbatch
sbatch --array=1-100 -J E4/M3_N9_U100_800  job.sbatch
