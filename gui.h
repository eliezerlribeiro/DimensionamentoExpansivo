#ifndef _GUI_H_
	#define _GUI_H_
#include "FEvento.h"
#include <iostream>
#include <string>

using namespace std;

class App : public FEvento {
	public:
		App();
		bool NaExecucao();

	private:
		bool rodando;
		SDL_Surface * planoExibicao;

	public:
		bool NoInicio();
		void NoEvento(SDL_Event *);
			void OnExit();
		void NoLaco();
		void NaRenderizacao();
		void NaLimpeza();
};

#endif
