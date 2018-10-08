CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRCS = test_node.cpp ../src/oxo.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = $(wildcard *.hpp)
EXEC = main

.PHONY = all clean distclean

all: $(EXEC)

$(EXEC) : $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJS) : $(SRCS) $(HEADERS)

clean:
	-rm -f $(OBJS)

distclean:
	-rm -f $(EXEC) $(OBJS)
