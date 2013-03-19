CC=g++
OBJ=FEvento.o FSuperficie.o lib_grafo.o Entidade.o gui.o

EXEC=grafo

CPPFLAGS=-std=c++11 -Wall -g `sdl-config --cflags`
#LDFLAGS=`sdl-config --libs` libSDL.a libSDL_draw.a libSDL_ttf.a libSDL_draw.a -lEstruturas
LDFLAGS=`sdl-config --libs` -lm -lSDL -lSDL_image -lSDL_gfx

gui: $(OBJ) gui.o
	$(CC) -o $(EXEC) $(OBJ) $(CPPFLAGS) $(LDFLAGS)
gui.o: gui.h gui.cpp
	rm -rf gui.o
	$(CC) $(CPPFLAGS) -c gui.cpp
lib_grafo.o: lib_grafo.hpp lib_grafo.cpp
	rm -rf lib_grafo.o
	$(CC) $(CPPFLAGS) -c lib_grafo.cpp
FEvento.o: FEvento.h FEvento.cpp
	rm -rf FEvento.o
	$(CC) $(CPPFLAGS) -c FEvento.cpp
FSuperficie.o: FSuperficie.h FSuperficie.cpp
	rm -rf FSuperficie.o
	$(CC) $(CPPFLAGS) -c FSuperficie.cpp
Entidade.o: Entidade.h Entidade.cpp
	rm -rf Entidade.o
	$(CC) $(CPPFLAGS) -c Entidade.cpp


pacotes:
	sudo apt-get install libsdl-image1.2-dev libsdl-image1.2 libsdl1.2-dev libsdl1.2debian libsdl-gfx1.2-4 libsdl-gfx1.2-dev

clean:
	rm -rf *.o
	rm -rf grafo
