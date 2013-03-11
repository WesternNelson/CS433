CC = g++
CFLAGS = -O3 -Wall 

PROG = CS433_Project5 
HDRS = 
SRCS = main.cpp

OBJS = $(SRCS:.cpp=.o)

$(PROG) : $(SRCS) $(HDRS)
	$(CC) $(SRCS) -o $(PROG) $(CFLAGS)

clean :
	rm -f core $(PROG) $(OBJS)
