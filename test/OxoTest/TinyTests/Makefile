CXX = clang++
CC = clang++
CXXFLAGS = -Wall -std=c++14
LDLFLAGS = -Wall -std=c++14
SRCS = random_moves.cpp ../../../src/action.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = $(wildcard *.hpp)
EXEC = random_moves
.PHONY = all clean distclean

all: $(EXEC)

clean:
	-rm -f $(OBJS)
	
distclean:
	-rm -f $(EXEC) $(OBJS)



$(EXEC) : $(OBJS)
$(OBJS) : $(SRCS) $(HEADERS)