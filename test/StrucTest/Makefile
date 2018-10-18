CXX = mpicxx
CXXFLAGS = -std=c++11 -Wall -g

SRCS = test_node.cpp test_oxo_tree.cpp test_nim_tree.cpp ../src/action.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = ../src/monte_carlo_search_tree.hpp ../src/node.hpp ../src/oxo.hpp ../src/nim.hpp ../src/action.hpp ../src/game.hpp
EXEC = test_node test_oxo_tree test_nim_tree

.PHONY = all clean distclean

all: $(EXEC)

% : %.o ../src/action.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# test_tree : test_tree.o ../src/action.o
# 	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJS) : $(SRCS) $(HEADERS)

clean:
	-rm -f $(OBJS)

distclean:
	-rm -f $(EXEC) $(OBJS)
