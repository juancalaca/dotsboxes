CXXFLAGS = -Wall -Werror

all: dotsedge.o dotsbox.o dotsboard.o main.o
	g++ dotsedge.o dotsbox.o dotsboard.o main.o -o main
	
dotsedge.o: DotsEdge.cc DotsEdge.h
	g++ $(CXXFLAGS) -c DotsEdge.cc -o dotsedge.o

dotsbox.o: DotsBox.cc DotsBox.h
	g++ $(CXXFLAGS) -c DotsBox.cc -o dotsbox.o

dotsboard.o: DotsBox.h DotsBoard.cc DotsBoard.h
	g++ $(CXXFLAGS) -c DotsBoard.cc -o dotsboard.o

main.o: DotsEdge.h DotsEdge.cc DotsBoard.cc DotsBox.cc DotsBoard.h DotsBox.h main.cc
	g++ $(CXXFLAGS) -c main.cc -o main.o

default: all

clean:
	rm *.o main