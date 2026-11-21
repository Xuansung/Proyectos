CC = cc
CFLAGS ?= -O2 -std=c11 -Wall -Wextra -pedantic
NVCC ?= nvcc
NVCCFLAGS ?= -O2

simular: simular.c auxiliares.c auxiliares.h
	$(CC) $(CFLAGS) simular.c auxiliares.c -lm -o simular

.PHONY: cuda
cuda: simular_cuda

simular_cuda: simular_cuda.cu auxiliares.c auxiliares.h
	$(NVCC) $(NVCCFLAGS) simular_cuda.cu auxiliares.c -o simular_cuda


