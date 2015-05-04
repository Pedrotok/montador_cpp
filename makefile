all: MONTAGEM

init.o: init.cpp header/init.hpp
	g++ -c init.cpp

mont.o: mont.cpp header/init.hpp
	g++ -c mont.cpp

MONTAGEM: mont.o init.o
	g++ -g mont.o init.o -o mont