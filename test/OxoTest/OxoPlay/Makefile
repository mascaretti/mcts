CXX = clang++
CC = clang++
CXXFLAGS = -Wall -std=c++14
LDLFLAGS = -Wall -std=c++14
SRCS = main.cpp ../../../src/action.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = ../../src/action.hpp ../../../src/game.hpp ../../../src/oxo.hpp
EXEC = main.out
.PHONY = all clean distclean
all: $(EXEC)

clean:
	-rm -f $(OBJS)
	
distclean:
	-rm -f $(EXEC) $(OBJS)



$(EXEC) : $(OBJS)
$(OBJS) : $(SRCS) $(HEADERS)