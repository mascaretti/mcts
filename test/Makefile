CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRCS = test_node.cpp ../src/oxo.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = $(wildcard *.hpp)
EXEC = main

.PHONY = all clean distclean

all: $(EXEC)

clean:
	-rm -f $(OBJS)

distclean:
	-rm -f $(EXEC) $(OBJS)

$(EXEC) : $(OBJS)

$(OBJS) : $(SRCS) $(HEADERS)
