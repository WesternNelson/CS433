###################################
# Programming Assignment 3
# Discrete Event Simulation
# Jim Counts and Joshua Pantarotto
# 3/31/2012
# 
###################################
CC = g++
CFLAGS = -O3 -Wall 
LIB = -lm
LDFLAGS = -L.
PROG = Assignment3
OBJ = main.o random.o
SRC = main.cpp random.c
HDRS = *.h

all: $(PROG) format check

$(PROG): $(OBJ) 
	$(CC) -c $(CFLAGS) $(SRC)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJ) $(LDFLAGS) $(LIB)


# cleanup
clean:
	/bin/rm -f *.o $(PROG) *.orig


format:
	astyle -YfDHUbps4yj -k3 $(SRC) $(HDRS)

check: 
	cppcheck $(CHKFLAGS) $(SRC) $(HDRS)

# DO NOT DELETE
