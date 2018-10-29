CXX = g++
# CXX = clang

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

.DEFAULT_GOAL = all

.PHONY: all clean

all: obj/action.o

obj/action.o: src/action.cpp include/action.hpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) obj/action.o
