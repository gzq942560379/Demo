#!/bin/bash

source ./env.sh

ompi_info --all | grep btl_openib_have_cuda_gdr

ompi_info --parsable --all | grep mpi_built_with_cuda_support:value

./MPIGPUDirect