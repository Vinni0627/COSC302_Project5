CXX=		g++
CXXFLAGS=	-g -Wall -std=gnu++11
SHELL=		bash

all:		worddice

worddice:	worddice.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^


clean:
	rm -f solution

test:		worddice Dice1.txt Words1.txt
	./worddice Dice1.txt Words1.txt