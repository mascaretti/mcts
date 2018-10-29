CXX = g++
# CC = g++

CXXFLAGS = -Wall -std=c++11

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = ../../../src/action.hpp ../../../src/nim.hpp ../../../src/game.hpp
EXEC = main

.DEFAULT_GOAL = all

$(OBJS) : $(HEADERS)

.PHONY = all clean distclean

all: $(EXEC)

$(EXEC) : $(OBJS) ../../../src/action.o
	$(CXX) $(CXXFLAGS) $(LDLFLAGS) $^ -o $@

clean:
	-rm -f $(OBJS)

distclean:
	-rm -f $(EXEC) $(OBJS)
