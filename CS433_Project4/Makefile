CC = g++
CFLAGS = -O2 -Wall -lpthread

PROG = CS433_Project4
HDRS = Buffer.h Program.h TestFramework.h Tests.h
SRCS = main.cpp

OBJS = $(SRCS:.cpp=.o)

$(PROG): $(SRCS) $(HDRS) format check
	$(CC) $(SRCS) -o $(PROG) $(CFLAGS)

clean:
	rm -f core $(PROG) $(OBJS) *.orig

format:
	astyle -YfDHUbps4yj -k3 $(SRCS) $(HDRS)

check: 
	cppcheck $(CHKFLAGS) $(SRCS) $(HDRS)
