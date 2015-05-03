INC_DIR=-I/usr/include/X11
LIB_DIR=-L/usr/lib
LIBS=-lX11


all: game.o link

lines.o: game.cpp
	g++ -c -Wall game.cpp $(INC_DIR)

link: game.o
	g++ game.o $(LIB_DIR) $(LIBS) -o game 

clean: 
	rm game.o game 

