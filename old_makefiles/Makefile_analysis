CXX = mpicxx

WARNFLAGS = -Wall
STANDARD = -std=c++11
CXXFLAGS = $(WARNFLAGS) $(STANDARD)

DEBUG = yes
OPTFLAGS =

ifeq ($(DEBUG), yes)
  OPTFLAGS += -g
else
  CPPFLAGS += -DNDEBUG
endif

SRCS = statistics.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = ../../src/monte_carlo_search_tree.hpp ../../src/node.hpp ../../src/oxo.hpp ../../src/nim.hpp ../../src/action.hpp ../../src/game.hpp
EXEC = statistics

NP = 4

.DEFAULT_GOAL = all

statistics.o: $(HEADERS)

.PHONY = all clean distclean run

all: $(EXEC)

% : %.o ../../src/action.o
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(CPPFLAGS) $^ -o $@

clean:
	-rm -f $(OBJS)

distclean:
	-rm -f $(EXEC) $(OBJS)

run:
	mpiexec -np $(NP) $(EXEC)
