TARGET = ProducerConsumer 

CC = gcc
CXX = g++
RM = rm -f

CFLAGS = -g -O2 -Wall -std=gnu++11 
CPPFLAGS = -g -ggdb -O2 -Wall -std=gnu++11
LDFLAGS = -g -ggdb -O2 -L/usr/lib64 -s
LDLIBS = -lc -lm -lcrypt -ldl -pthread


SRCS = ProducerConsumer.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: depend clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LDLIBS)

.cpp.o:
	$(CXX) $(CPPFLAGS) -c $<  -o $@

clean:
	$(RM) $(TARGET) $(OBJS)

