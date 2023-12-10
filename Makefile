all:
	g++ src/board.cpp -Wall -Ofast -o obj/board.o
	./obj/board.o

clean:
	rm obj/*.o
