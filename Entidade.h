/**
 * Grupo
 * André Silva
 * Eliézer Ribeiro
 * 
 */
#ifndef ENTIDADE_H_
#define ENTIDADE_H_

#include "lib_grafo.hpp"

#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"

class Entidade {
public:
	Entidade();
	virtual ~Entidade();
	static vector<Entidade*> listaEntidades;
	int x;
	int Dx;
	int y;
	int Dy;
	int raio;

	SDL_Color cor;

	SDL_Surface * superficie;

	virtual void NoLaco();
	virtual void NaRenderizacao(SDL_Surface * planoExibicao);
	virtual void NaLimpeza();

};


class EntidadeVertice : public Entidade {
public:
	EntidadeVertice();
	EntidadeVertice(Vertice * cVertice, int raio);
	Vertice * circulo;
	int raio;

	void NoLaco();
	void NaRenderizacao(SDL_Surface * planoExibicao);
};
class EntidadeAresta : public Entidade  {
public:
	EntidadeAresta();
	EntidadeAresta(Aresta * cAresta, int x2, int y2);
	Aresta * reta;
	int dist;
	int x2;
	int y2;

	void NoLaco();
	void NaRenderizacao(SDL_Surface * planoExibicao);
};


#endif /* ENTIDADE_H_ */
