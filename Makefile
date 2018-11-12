CXX = mpicxx

WARNFLAGS = -Wall
STANDARD = -std=c++11
CXXFLAGS = $(WARNFLAGS) $(STANDARD)

INCLUDE_PATH = $(PWD)/include
CPPFLAGS += -I$(INCLUDE_PATH)

DEBUG = yes
OPTFLAGS =

ifeq ($(DEBUG), yes)
  OPTFLAGS += -g
else
  CPPFLAGS += -DNDEBUG
endif

HEADERS = $(INCLUDE_PATH)/monte_carlo_search_tree.hpp $(INCLUDE_PATH)/node.hpp
HEADERS += $(INCLUDE_PATH)/oxo.hpp $(INCLUDE_PATH)/nim.hpp $(INCLUDE_PATH)/oxo_action.hpp $(INCLUDE_PATH)/nim_action.hpp $(INCLUDE_PATH)/game.hpp

SRCS = examples/ai_vs_random.cpp examples/ai_vs_ai.cpp examples/ai_vs_human.cpp
SRCS += test/analysis/statistics.cpp
SRCS += test/speedup_test/speedup_oxo.cpp test/speedup_test/speedup_nim.cpp test/speedup_test/serial_oxo.cpp test/speedup_test/serial_nim.cpp
SRCS += test/struct_test/test_nim_tree.cpp test/struct_test/test_oxo_tree.cpp test/struct_test/test_node.cpp
SRCS += test/nim_test/nim_play.cpp test/nim_test/random_nim_match.cpp
SRCS += test/oxo_test/oxo_play.cpp test/oxo_test/random_oxo_match.cpp

OBJS = $(SRCS:.cpp=.o)

EXEC = $(SRCS:.cpp= )

.DEFAULT_GOAL = all

examples/ai_vs_ai.o: $(HEADERS)
examples/ai_vs_human.o: $(HEADERS)
examples/ai_vs_random.o: $(HEADERS)

test/analysis/statistics.o: $(HEADERS)

test/nim_test/nim_play.o test/nim_test/random_nim_match.o: include/nim_action.hpp
test/nim_test/nim_play.o test/nim_test/random_nim_match.o: include/game.hpp
test/nim_test/nim_play.o test/nim_test/random_nim_match.o: include/nim.hpp

test/oxo_test/oxo_play.o test/oxo_test/random_oxo_match.o: include/oxo_action.hpp
test/oxo_test/oxo_play.o test/oxo_test/random_oxo_match.o: include/game.hpp
test/oxo_test/oxo_play.o test/oxo_test/random_oxo_match.o: include/oxo.hpp

test/speedup_test/speedup_oxo.o test/speedup_test/speedup_nim.o test/speedup_test/serial_oxo.o test/speedup_test/serial_nim.o: include/game.hpp
test/speedup_test/speedup_nim.o test/speedup_test/serial_nim.o: include/nim_action.hpp
test/speedup_test/speedup_oxo.o test/speedup_test/serial_oxo.o: include/oxo_action.hpp
test/speedup_test/speedup_nim.o test/speedup_test/serial_nim.o: include/nim.hpp
test/speedup_test/speedup_oxo.o test/speedup_test/serial_oxo.o: include/oxo.hpp
test/speedup_test/speedup_oxo.o test/speedup_test/speedup_nim.o test/speedup_test/serial_oxo.o test/speedup_test/serial_nim.o: include/node.hpp
test/speedup_test/speedup_oxo.o test/speedup_test/speedup_nim.o test/speedup_test/serial_oxo.o test/speedup_test/serial_nim.o: include/monte_carlo_search_tree.hpp

test/struct_test/test_node.o test/struct_test/test_oxo_tree.o test/struct_test/test_nim_tree.o: include/game.hpp
test/struct_test/test_node.o test/struct_test/test_nim_tree.o: include/nim_action.hpp
test/struct_test/test_node.o test/struct_test/test_oxo_tree.o: include/oxo_action.hpp
test/struct_test/test_node.o test/struct_test/test_oxo_tree.o test/struct_test/test_nim_tree.o: include/node.hpp
test/struct_test/test_node.o test/struct_test/test_oxo_tree.o: include/oxo.hpp
test/struct_test/test_oxo_tree.o test/struct_test/test_nim_tree.o: include/monte_carlo_search_tree.hpp
test/struct_test/test_nim_tree.o: include/nim.hpp

.PHONY: all clean

all: obj/nim_action.o obj/oxo_action.o $(EXEC)

% : %.o obj/nim_action.o obj/oxo_action.o
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $^ -o $@

obj/%.o: src/%.cpp include/%.hpp
	if [ ! -d "obj" ]; then mkdir obj; fi
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(EXEC)

distclean:
	$(RM) $(EXEC)
	$(RM) $(OBJS)
	$(RM) obj/*.o
