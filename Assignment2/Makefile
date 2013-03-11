###################################
# Makefile
# Assignment 2
# CS433 - Spring 2012
# Josh Pantarotto and Jim Counts
###################################
CC = g++
CFLAGS = -O3 -Wall
PROG = assignment2
HDRS = mysh.h
SRCS = main.cpp mysh.cpp
OBJ = $(SRCS:.cpp=.o)

$(PROG): $(SRCS) $(HDRS) format check
	$(CC) $(SRCS) -o $(PROG) $(CFLAGS)

format:
	astyle -YfDHUbps4yj -k3 $(SRCS) $(HDRS)

check: 
	cppcheck $(CHKFLAGS) $(SRCS) $(HDRS)

# cleanup
clean:
	rm -f core $(PROG) $(OBJS) *.orig

# DO NOT DELETE