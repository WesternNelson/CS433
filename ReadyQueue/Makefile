###################################
# Makefile
# Assignment 1
# CS433 - Spring 2012
# Josh Pantarotto and Jim Counts
###################################
CC = g++
CFLAGS = -O3 -Wall
PROG = assignment1
HDRS = *.h
SRCS = *.cpp
OBJ = $(SRCS:.cpp=.o)
BAK = *.orig

$(PROG): $(SRCS) $(HDRS) format check
	$(CC) $(SRCS) -o $(PROG) $(CFLAGS)

format:
	astyle -YfDHUbps4yj -k3 $(SRCS) $(HDRS)

check: 
	cppcheck $(CHKFLAGS) $(SRCS) $(HDRS)

# cleanup
clean:
	rm -f core $(PROG) $(OBJS) $(BAK)

# DO NOT DELETE