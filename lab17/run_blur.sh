#!/bin/bash
#SBATCH --job-name=blur_lab17
#SBATCH --output=output.txt
#SBATCH --error=error.txt
#SBATCH --time=00:05:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8

make

./blur.exe 1 4000
./blur.exe 2 4000
./blur.exe 4 4000
./blur.exe 8 4000