all: main
main: main.cpp
	g++ -std=c++1z main.cpp -lpthread
	./a.out
