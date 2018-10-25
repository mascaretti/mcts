CXX = mpicxx
CXXFLAGS = -std=c++11 -Wall -g

SRCS = speedup_oxo.cpp speedup_nim.cpp serial_oxo.cpp serial_nim.cpp ../../src/action.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = ../../src/monte_carlo_search_tree.hpp ../../src/node.hpp ../../src/oxo.hpp ../../src/nim.hpp ../../src/action.hpp ../../src/game.hpp
EXEC = speedup_oxo speedup_nim serial_oxo serial_nim

.PHONY = all clean distclean

all: $(EXEC)

% : %.o ../../src/action.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# test_tree : test_tree.o ../src/action.o
# 	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJS) : $(SRCS) $(HEADERS)

clean:
	-rm -f $(OBJS)

distclean:
	-rm -f $(EXEC) $(OBJS)
