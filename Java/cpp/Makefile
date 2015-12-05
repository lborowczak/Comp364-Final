CC = g++
CFLAGS  = -fopenmp -g -Wall -std=c++11
C_OPTIM_FLAGS = -std=c++11 -O3 -march=native -mtune=native

default: Barnes-Hut

Barnes-Hut: NBody_BH.cpp NBody_BH.hpp Body.o Oct.o BHTree.o
	$(CC) $(CFLAGS) -o Barnes-Hut.out NBody_BH.cpp Body.o Oct.o BHTree.o

Oct.o: Oct.cpp Oct.hpp
	$(CC) $(CFLAGS) -c Oct.cpp

Body.o: Body.cpp Body.hpp Oct.hpp
	$(CC) $(CFLAGS) -c Body.cpp

BHTree.o: BHTree.cpp BHTree.hpp Oct.hpp Body.hpp
	$(CC) $(CFLAGS) -c BHTree.cpp

clean:
	$(RM) Barnes-Hut.out *.o *~
