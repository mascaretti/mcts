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

SRCS = speedup_oxo.cpp speedup_nim.cpp serial_oxo.cpp serial_nim.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = ../../src/monte_carlo_search_tree.hpp ../../src/node.hpp ../../src/oxo.hpp ../../src/nim.hpp ../../src/action.hpp ../../src/game.hpp
EXEC = speedup_oxo speedup_nim serial_oxo serial_nim
DEFAULT_EXEC = speedup_oxo

NP = 4

.DEFAULT_GOAL = all

speedup_oxo.o speedup_nim.o serial_oxo.o serial_nim.o: ../../src/game.hpp
speedup_oxo.o speedup_nim.o serial_oxo.o serial_nim.o: ../../src/action.hpp
speedup_nim.o serial_nim.o: ../../src/nim.hpp
speedup_oxo.o serial_oxo.o: ../../src/oxo.hpp
speedup_oxo.o speedup_nim.o serial_oxo.o serial_nim.o: ../../src/node.hpp
speedup_oxo.o speedup_nim.o serial_oxo.o serial_nim.o: ../../src/monte_carlo_search_tree.hpp

.PHONY = all clean distclean run

all: $(EXEC)

% : %.o ../../src/action.o
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(CPPFLAGS) $^ -o $@

clean:
	-rm -f $(OBJS)

distclean:
	-rm -f $(EXEC) $(OBJS)

run:
	mpiexec -np $(NP) $(DEFAULT_EXEC)
