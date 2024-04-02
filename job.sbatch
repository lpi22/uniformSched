#!/bin/bash
#SBATCH --job-name=uniformsched
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=16
#SBATCH --mem=200G
#SBATCH --time=48:00:00
#SBATCH --output=logs/%A_%a.out
#SBATCH --error=err/%A_%a.err
#SBATCH --partition=base
#SBATCH --mail-user=stu209381@mail.uni-kiel.de
#SBATCH --mail-type=FAIL
#SBATCH --array=1-100

TID=`printf %03d $SLURM_ARRAY_TASK_ID`

./code/bin/uniformsched "/${SLURM_JOB_NAME}_$TID.dat"