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

SRCS = ai_vs_random.cpp ai_vs_ai.cpp ai_vs_human.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = ../src/monte_carlo_search_tree.hpp ../src/node.hpp ../src/oxo.hpp ../src/nim.hpp ../src/action.hpp ../src/game.hpp
EXEC = ai_vs_random ai_vs_ai ai_vs_human
DEFAULT_EXEC = ai_vs_random

NP = 4

.DEFAULT_GOAL = all

ai_vs_random.o: $(HEADERS)
ai_vs_ai.o: $(HEADERS)
ai_vs_human.o: $(HEADERS)

.PHONY = all clean distclean run

all: $(EXEC)

% : %.o ../src/action.o
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(CPPFLAGS) $^ -o $@

clean:
	-rm -f $(OBJS)

distclean:
	-rm -f $(EXEC) $(OBJS)

run:
	mpiexec -np $(NP) $(DEFAULT_EXEC)
