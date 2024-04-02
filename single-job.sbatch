#!/bin/bash
#SBATCH --job-name=uniformsched
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=16
#SBATCH --mem=200G
#SBATCH --time=48:00:00
#SBATCH --output=single-job.out
#SBATCH --error=single-job.err
#SBATCH --partition=base
#SBATCH --mail-user=stu209381@mail.uni-kiel.de
#SBATCH --mail-type=FAIL

./code/bin/uniformsched /4.dat 10