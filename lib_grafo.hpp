#ifndef _LIB_GRAFO_
	#define _LIB_GRAFO_
	
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>
#include <cmath>
#include <climits>
#include <vector>

using namespace std;


class Excecao {
	public:
		Excecao();

	public:
		bool Tratamento(string msgErro) const;
		bool Tratamento(stringstream msgErro) const;
};



/**
 * Classe Vertice
 **/

class Aresta : public Excecao {
	private:
		int id1;
		int id2;
		int peso;

	public:
		Aresta();
		Aresta(int id1, int id2, int peso);
		Aresta(const Aresta & cAresta);
		
		bool 		SetPeso(int peso);
		bool 		SetConexao(int id1, int id2);
		int 		GetId1() const;
		int 		GetId2() const;
		int 		GetPeso() const;

		bool 		Conexao(int id1,int id2) const;

	public:
		bool operator==(int null);
		bool operator==(const Aresta & id2);
		bool operator!=(int null);
		bool operator!=(const Aresta & id2);
};

class Vertice : public Excecao  {
	private:
		std::string informacao;
		int id;
		int tamLista;
		Aresta * lArestas;
		
	public:
		Vertice();
		Vertice(const Vertice & cVertice);
		Vertice(int id,const std::string, int tamLista);
	
	public:
		bool 		SetId(int id);
		bool 		SetInformacao(const std::string);
		bool 		SetConexao(int id2, int peso);
		int 		GetId() const;
		int 		GetTamLista() const;
		Aresta * 	GetAresta(int id2) const;
		std::string GetInformacao() const;
		bool 		DelConexao(int id2);
		
		bool		Existe() const;
		bool		ExisteAresta(int id2) const;
		bool		Conexao(int id2) const;
		void 		Mostra() const;

	public:
		bool operator==(int null);
		bool operator==(const Vertice & b);
		bool operator!=(int null);
		bool operator!=(const Vertice & b);

};

//Tipos de Heap
enum {
	MINIMO = 0,
	MAXIMO
};

class Heap : public Excecao {
	private:
		Aresta * arrayHeap;
		int tamHeapMax;
		int tamHeap;
		int tipo;
	public:
		Heap();
		Heap(const Heap & cHeap);
		Heap(int tamHeap);
		~Heap();

		bool Push(Aresta * dados);
		Aresta * Pop();
		void ListaHeap();
		Aresta * GetAresta(int id) const;
		void SetTipo(int tipo);
		int GetTipo() const;
		int GetTamHeap() const;
		int GetTamHeapMax() const;
		bool TrocaAresta(int id1, int id2) const;
		
	private:
		bool BalanceUp(int posPai, int posFilho) const;
		bool BalanceDown(int posPai) const;


};

/**
 * Classe Grafo
 **/
class Grafo : public Excecao {
	private:
		int tamGrafo;
		int nrElementos;
		bool direcionado;
		bool debug;
		Vertice * lVertice;
	
	public:
		Grafo();
		Grafo(const Grafo & cGrafo);
		Grafo(int tamGrafo);
		~Grafo();

	public:
		bool Direcionado() const;
		void SetDirecionado(bool direcionado);

		bool 		ExisteVertice(int id) const;
		bool 		SetVertice(int id,std::string);
		bool 		RemoveVertice(int id);
		
		Vertice 	GetVertice(int id) const;
		int			GetTamGrafo() const;
		int			GetElementos() const;
		bool 		Conexao(int,int) const;

		bool 		CriaConexao(int,int,int);
		bool 		RemoveConexao(int id1,int id2);
		
		std::vector<int> Vizinhos(int id);
		bool 		Vizinho(int id1, int id2) const;
		
		std::vector<int> OrdemTopologica();
		std::vector<int> GetConjVertices() const;
		int 		GetGrauEntrada(int id) const;
		int 		GetGrauSaida(int id) const;
		int 		GetGrau(int id) const;
		
	private:
		std::vector<int> conjKruskal;
		int BuscaKruskal(int id);
	public:
		std::vector<Aresta *> Kruskal();
		
	private:
		std::vector<int> conjDijkstra;
		bool ExisteConjDijkstra(int id);
	public:
		std::vector<int> Dijkstra(int id1, int id2);
		
		void JsonVertice(int id);
		void JsonRemove(int id);
		void JsonVizinhos(int id);
		void JsonConexao(int id1, int id2);
		void JsonOrdemTopologica();
		void JsonArvoreMinima();
		void JsonMenorCaminho(int id1, int id2);
};

#endif
