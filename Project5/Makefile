CC = g++
CFLAGS = -O3 -Wall 

PROG = CS433_Project5 
HDRS = 
SRCS = main.cpp

OBJS = $(SRCS:.cpp=.o)

$(PROG): $(SRCS) $(HDRS) format check
	$(CC) $(SRCS) -o $(PROG) $(CFLAGS)

clean:
	rm -f core $(PROG) $(OBJS) *.orig

format:
	astyle -YfDHUbps4yj -k3 $(SRCS)

check: 
	cppcheck $(CHKFLAGS) $(SRCS)
