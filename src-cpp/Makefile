# Makefile for C++ version of pgvector extension
# This Makefile is designed to be used from the parent directory

EXTENSION = vector
EXTVERSION = 0.8.1

MODULE_big = vector_cpp

# C++ source files (relative to src-cpp directory)
OBJS = bitutils.o bitvec.o halfutils.o halfvec.o hnsw.o hnswbuild.o hnswinsert.o hnswscan.o hnswutils.o hnswvacuum.o ivfbuild.o ivfflat.o ivfinsert.o ivfkmeans.o ivfscan.o ivfutils.o ivfvacuum.o sparsevec.o vector.o

# C++ specific flags
PG_CXXFLAGS = $(PG_CFLAGS) -std=c++17 -fpermissive

# To compile for portability, run: make OPTFLAGS=""
OPTFLAGS = -march=native

# Mac ARM doesn't always support -march=native
ifeq ($(shell uname -s), Darwin)
ifeq ($(shell uname -p), arm)
# no difference with -march=armv8.5-a
OPTFLAGS =
endif
endif

# PowerPC doesn't support -march=native
ifneq ($(filter ppc64%, $(shell uname -m)), )
OPTFLAGS =
endif

# For auto-vectorization:
PG_CFLAGS += $(OPTFLAGS) -ftree-vectorize -fassociative-math -fno-signed-zeros -fno-trapping-math
PG_CXXFLAGS += $(OPTFLAGS) -ftree-vectorize -fassociative-math -fno-signed-zeros -fno-trapping-math

# Override the default suffix rules to use C++ compiler for .cpp files
.cpp.o:
$(CXX) $(PG_CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

PG_CONFIG ?= pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
