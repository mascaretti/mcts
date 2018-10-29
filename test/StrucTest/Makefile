CXX = mpicxx
CXXFLAGS = -std=c++11 -Wall -g

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

SRCS = test_node.cpp test_oxo_tree.cpp test_nim_tree.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = ../../src/monte_carlo_search_tree.hpp ../../src/node.hpp ../../src/oxo.hpp ../../src/nim.hpp ../../src/action.hpp ../../src/game.hpp
EXEC = test_node test_oxo_tree test_nim_tree
DEFAULT_EXEC = test_oxo_tree

NP = 4

.DEFAULT_GOAL = all

test_node.o test_oxo_tree.o test_nim_tree.o: ../../src/game.hpp
test_node.o test_oxo_tree.o test_nim_tree.o: ../../src/action.hpp
test_node.o test_oxo_tree.o test_nim_tree.o: ../../src/node.hpp
test_node.o test_oxo_tree.o: ../../src/oxo.hpp
test_oxo_tree.o test_nim_tree.o: ../../src/monte_carlo_search_tree.hpp
test_nim_tree.o: ../../src/nim.hpp

.PHONY = all clean distclean run_parallel

all: $(EXEC)

% : %.o ../../src/action.o
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(CPPFLAGS) $^ -o $@

clean:
	-rm -f $(OBJS)

distclean:
	-rm -f $(EXEC) $(OBJS)

run:
	mpiexec -np $(NP) $(DEFAULT_EXEC)
