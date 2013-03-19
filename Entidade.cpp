/*
 * Entidade.cpp
 *
 *  Created on: 18/03/2013
 *      Author: andref
 */

#include "Entidade.h"

std::vector<Entidade*> Entidade::listaEntidades;

Entidade::Entidade() {
	// TODO Auto-generated constructor stub
	superficie = NULL;
	this->x = 0;
	this->y = 0;
	cor = {0,0,0};
}

Entidade::~Entidade() {
	// TODO Auto-generated destructor stub
}

void Entidade::NaRenderizacao(SDL_Surface * planoExibicao) {

}
void Entidade::NoLaco() {

}
void Entidade::NaLimpeza() {
	if(superficie)
		SDL_FreeSurface(superficie);

}



EntidadeVertice::EntidadeVertice() {
	circulo = new Vertice();
	raio = 0;
}
EntidadeVertice::EntidadeVertice(Vertice * cVertice,int raio) {
	circulo = new Vertice(*cVertice);
	this->raio = raio;
}
void EntidadeVertice::NaRenderizacao(SDL_Surface * planoExibicao) {
	filledCircleRGBA(planoExibicao, this->x, this->y, this->raio, this->cor.r, this->cor.g, this->cor.b, 200);

}

void EntidadeVertice::NoLaco() {
	cout << "this->x: " << this->x << " this->y: " << this->y << endl;
}
