#!/bin/bash
module load mpi/openmpi-x86_64
mpirun --mca btl_tcp_if_include eth0 ./rank