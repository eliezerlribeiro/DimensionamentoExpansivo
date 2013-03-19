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
	this->Dx = 1;
	this->raio = 5;
	
	this->y = 0;
	this->Dy = 1;
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


/**
 * Implementacao EntidadeVertice
 */
EntidadeVertice::EntidadeVertice() : Entidade() {
	circulo = new Vertice();
	raio = 0;
}
EntidadeVertice::EntidadeVertice(Vertice * cVertice,int raio) : Entidade() {
	circulo = new Vertice(*cVertice);
	this->raio = raio;
}
void EntidadeVertice::NaRenderizacao(SDL_Surface * planoExibicao) {
	filledCircleRGBA(planoExibicao, this->x, this->y, this->raio, this->cor.r, this->cor.g, this->cor.b, 255);
}

void EntidadeVertice::NoLaco() {
}
/**
 * Implementacao EntidadeAresta
 */
EntidadeAresta::EntidadeAresta() : Entidade() {
	reta = new Aresta();
	this->x2 = 0;
	this->y2 = 0;
	dist = 0;
}
EntidadeAresta::EntidadeAresta(Aresta * cAresta,int x2, int y2) : Entidade() {
	reta = new Aresta(*cAresta);
	this->x2 = x2;
	this->y2 = y2;
	dist = 0;
}
void EntidadeAresta::NaRenderizacao(SDL_Surface * planoExibicao) {
	lineRGBA(planoExibicao, this->x, this->y, this->x2, this->y2, this->cor.r , this->cor.g, this->cor.b, 255);
}

void EntidadeAresta::NoLaco() {

}
