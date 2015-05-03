INC_DIR=-I/usr/include/X11
LIB_DIR=-L/usr/lib
LIBS=-lX11


all: game.o link

game.o: game.cpp
	g++ -std=c++0x -pthread -c -Wall $(INC_DIR) game.cpp

link: game.o
	g++ -std=c++0x -pthread $(LIBDIR)  game.o  $(LIBS) -o game 

clean: 
	rm game.o game 

