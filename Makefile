CC=g++
OBJ=FEvento.o FSuperficie.o gui.o

EXEC=grafo

CPPFLAGS=-std=c++11 -Wall -g `sdl-config --cflags`
#LDFLAGS=`sdl-config --libs` libSDL.a libSDL_draw.a libSDL_ttf.a libSDL_draw.a -lEstruturas
LDFLAGS=`sdl-config --libs` -lm -lSDL -lSDL_image -lSDL_gfx

gui: $(OBJ) gui.o
	$(CC) -o $(EXEC) $(OBJ) gui.o $(CPPFLAGS) $(LDFLAGS)
gui.o: gui.h gui.cpp
	rm -rf gui.o
	$(CC) $(CPPFLAGS) -c gui.cpp
FEvento.o: FEvento.h FEvento.cpp
	rm -rf FEvento.o
	$(CC) $(CPPFLAGS) -c FEvento.cpp
FSuperficie.o: FSuperficie.h FSuperficie.cpp
	rm -rf FSuperficie.o
	$(CC) $(CPPFLAGS) -c FSuperficie.cpp


pacotes:
	sudo apt-get install libsdl-image1.2-dev libsdl-image1.2 libsdl1.2-dev libsdl1.2debian libsdl-gfx1.2-4 libsdl-gfx1.2-dev

clean:
	rm -rf *.o
	rm -rf grafo
