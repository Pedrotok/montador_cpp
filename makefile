all: MONTAGEM

init.o: init.cpp header/init.hpp
	g++ -c -Wall init.cpp

mont.o: mont.cpp header/init.hpp
	g++ -c -std=c++11 -Wall mont.cpp

MONTAGEM: mont.o init.o
	g++ -g -Wall mont.o init.o -o mont
