#ifndef _GUI_H_
	#define _GUI_H_
#include "FEvento.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>

#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"

#include "lib_grafo.hpp"
#include "FEvento.h"

using namespace std;

//estados
enum {
	INICIAL = 0,
	GERA_GRAFO,
	GERA_CONEXOES,
	ACOES,
	SETA_VERTICE
};
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

class App : public FEvento {
	public:
		App();
		bool NaExecucao();

	private:
		bool rodando;
		SDL_Surface * planoExibicao;
		Grafo * mGrafo;

	public:
		int argc;
		char * argv;
		bool NoInicio();
		void NoEvento(SDL_Event *);
			void OnExit();
			void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
		void NoLaco();
		void NaRenderizacao();
		void NaLimpeza();
};

#endif
