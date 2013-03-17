#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"

//#include "lib_grafo.hpp"
#include "gui.h"

App::App() {
	planoExibicao = NULL;
	rodando = true;
}

bool App::NaExecucao() {
	if (NoInicio() == false)
		return -1;
	
	SDL_Event evento;
	
	while(rodando) {
		while(SDL_PollEvent(&evento))
			NoEvento(&evento);
			
		NoLaco();
		NaRenderizacao();
	}
	NaLimpeza();
	return 0;
}

bool App::NoInicio() {
	try {
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
			throw 1;
		if ((planoExibicao = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
			throw 2;

	} catch (int e) {
		switch(e) {
			case 1:
				cout << "FracFly::NoInic: Erro SDL_Init ";
				break;
			case 2:
				cout << "FracFly::NoInic: Erro SDL_VideoMode ";
				break;
		}
		cout << SDL_GetError();
		return false;
	}
	return true;
}
void App::NoEvento(SDL_Event * evento) {
	FEvento::NoEvento(evento);
}

void App::OnExit() {
	rodando = false;
}


void App::NoLaco() {
	
}
void App::NaRenderizacao() {
	SDL_FillRect(planoExibicao, NULL, SDL_MapRGB(planoExibicao->format, 0, 0, 0));
	
	lineRGBA(planoExibicao, 
               20, 10,
               70, 90, 
               255, 0, 0, 255);

	SDL_UpdateRect(planoExibicao, 0, 0, 640, 480);

}

void App::NaLimpeza() {
	SDL_Quit();
}

int mainGui(int argc, char* argv[]) {

	App grafoGui;
	
	return grafoGui.NaExecucao();
}
