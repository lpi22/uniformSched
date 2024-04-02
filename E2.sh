#!/bin/bash

sbatch --array=1-100 -J E2/M2_N10_U100_800   job.sbatch
sbatch --array=1-100 -J E2/M2_N30_U100_800   job.sbatch
sbatch --array=1-100 -J E2/M2_N50_U100_800   job.sbatch
sbatch --array=1-100 -J E2/M2_N100_U100_800  job.sbatch
sbatch --array=1-100 -J E2/M3_N10_U100_800   job.sbatch
sbatch --array=1-100 -J E2/M3_N30_U100_800   job.sbatch
sbatch --array=1-100 -J E2/M3_N50_U100_800   job.sbatch
sbatch --array=1-100 -J E2/M3_N100_U100_800  job.sbatch
sbatch --array=1-100 -J E2/M4_N30_U100_800   job.sbatch
sbatch --array=1-100 -J E2/M4_N50_U100_800   job.sbatch
sbatch --array=1-100 -J E2/M4_N100_U100_800  job.sbatch
sbatch --array=1-100 -J E2/M6_N30_U100_800   job.sbatch
sbatch --array=1-100 -J E2/M6_N50_U100_800   job.sbatch
sbatch --array=1-100 -J E2/M6_N100_U100_800  job.sbatch
sbatch --array=1-100 -J E2/M8_N30_U100_800   job.sbatch
sbatch --array=1-100 -J E2/M8_N50_U100_800   job.sbatch
sbatch --array=1-100 -J E2/M8_N100_U100_800  job.sbatch
sbatch --array=1-100 -J E2/M10_N30_U100_800  job.sbatch
sbatch --array=1-100 -J E2/M10_N50_U100_800  job.sbatch
sbatch --array=1-100 -J E2/M10_N100_U100_800 job.sbatch
