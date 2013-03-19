/**
 * Grupo
 * André Silva
 * Eliézer Ribeiro
 * 
 */
#include "lib_grafo.hpp"

/**
 * Implementacao classe Grafo
 **/
Grafo::Grafo() {
	this->direcionado = false;
	this->tamGrafo = 0;
	this->nrElementos = 0;
	this->debug = false;
	this->lVertice = NULL;
}
Grafo::Grafo(const Grafo & cGrafo) {
	this->direcionado = cGrafo.Direcionado();
	this->tamGrafo = cGrafo.GetTamGrafo();
	this->nrElementos = cGrafo.GetElementos();
	this->debug = false;
	this->lVertice = new Vertice [ this->tamGrafo ];
	for (int i = 0; i < this->tamGrafo; i++) {
		Vertice * nVertice = new Vertice(cGrafo.GetVertice(i));
		this->lVertice[i] = *nVertice;
	}
}
Grafo::Grafo(int tamGrafo) {
	this->direcionado = false;
	this->tamGrafo = tamGrafo;
	this->nrElementos = 0;
	this->debug = false;
	this->lVertice = new Vertice [ tamGrafo ];
}
bool Grafo::Direcionado() const {
	return this->direcionado;
}

int Grafo::GetTamGrafo() const {
	return this->tamGrafo;
}
int Grafo::GetElementos() const {
	return this->nrElementos;
}
void Grafo::SetDirecionado(bool direcionado) {
	this->direcionado = direcionado;
}

bool Grafo::ExisteVertice(int id) const {
	bool retorno = false;
	try {
		if (id >= tamGrafo) {
			throw 0;
		}
		if (this->lVertice[id].Existe())
			retorno = true;
	} catch (...) {
		retorno = false;
	}
	return retorno;
}

bool Grafo::SetVertice(int id, std::string informacao) {
	bool retorno = false;
	try {
		if (id >= tamGrafo)
			throw;
		//verifica se jah esta na lista
		if (!this->ExisteVertice(id)) {
			Vertice * nVertice = new Vertice(id, informacao, tamGrafo);
			this->lVertice[id] = *nVertice;
			this->nrElementos++;
			retorno = true;
		}
	} catch (...) {
		retorno = false;
	}
	return retorno;
}

bool Grafo::RemoveVertice(int id) {
	bool retorno = false;
	string strErro;
	try {
		if (!ExisteVertice(id)) {
			throw 1;
		}
		Vertice * nVertice = new Vertice();
		this->lVertice[id] = *nVertice;
		//remove todas conexoes
		for (int i = 0; i < this->tamGrafo; i++) {
			if (!this->Direcionado())
				this->RemoveConexao(i,id);
		}
		this->nrElementos--;
		retorno = true;
	} catch(...) {
		retorno = false;
	}
	return retorno;
}
Vertice Grafo::GetVertice(int id) const {
	Vertice retorno;
	if (id >= 0 && id <= tamGrafo && this->ExisteVertice(id)) {
		retorno = this->lVertice[id];
	}
	return retorno;
}



bool Grafo::Conexao(int id1, int id2) const {
	bool retorno = false;
	if (this->ExisteVertice(id1) && this->ExisteVertice(id2))
		if (this->lVertice[id1].ExisteAresta(id2))
			retorno = (this->lVertice[id1].GetAresta(id2)->GetPeso() != INT_MIN ? true : false);
	return retorno;
}
bool Grafo::CriaConexao(int id1,int id2, int peso) {
	bool retorno = true;
	try {
		if (!this->ExisteVertice(id1) || !this->ExisteVertice(id2))
			throw 1;
		this->lVertice[id1].SetConexao(id2, peso);
		if (!this->Direcionado()) {
			this->lVertice[id2].SetConexao(id1, peso);
		}
	} catch (int e) {
		if (debug) {
			switch(e) {
				case 1:
					cout << "Vertice nao existe" << endl;
					break;
				case 2:
					cout << "Jah existe conexao" << endl;
					break;
			}
		}
		retorno = false;
	}
	return retorno;
}
bool Grafo::RemoveConexao(int id1,int id2) {
	bool retorno = true;
	try {
		if (!this->ExisteVertice(id1) || !this->ExisteVertice(id2))
			throw 1;
		//se aresta jah eh igual a zero nao tinha conexao
		retorno = this->lVertice[id1].DelConexao(id2);
		if (!this->Direcionado())
			this->lVertice[id2].DelConexao(id1);
	} catch (int e) {
		retorno = false;
	}
	return retorno;
}

bool Grafo::Vizinho(int id1, int id2) const {
	bool retorno = false;
	if (this->ExisteVertice(id1) && this->ExisteVertice(id2)) {
		if (this->lVertice[id1].ExisteAresta(id2)) {
			retorno = (this->Conexao(id1,id2) ? true : false);
		}
	}
	return retorno;
}

std::vector<int> Grafo::Vizinhos(int id) {
	std::vector<int> lVizinhos;
	for(int i = 0; i < this->tamGrafo; i++) {
		if (Vizinho(id,i)) {
			lVizinhos.push_back(i);
		}
	}
	return lVizinhos;

}

std::vector<int> Grafo::OrdemTopologica() {
	std::vector<int> listaOrdenado;
	std::vector<int> lSemArestas;
	Grafo * clone = new Grafo(*this);
	int id;
	bool dag = true;
	lSemArestas = clone->GetConjVertices();
	while ((int) lSemArestas.size() > 0) {
		id = lSemArestas[0];
		lSemArestas.erase(lSemArestas.begin());
		clone->RemoveVertice(id);
		//cout << "lSemArestas[0]: " << lSemArestas[0] << endl;
		//remova um nodo n de S
		//insira n em L
		listaOrdenado.push_back(id);

		if ((int) lSemArestas.size() == 0) {
			lSemArestas.clear();
			lSemArestas = clone->GetConjVertices();
			if ((int) lSemArestas.size() == 0 && clone->nrElementos > 0) {
				dag = false;
				break;
			}
			//for (int i =0; i < (int) lSemArestas.size(); i++)
			//	cout << "lSemArestas[i]: " << lSemArestas[i] << endl;
		}
	}
	if (!dag) {
		listaOrdenado.clear();
	}
	return listaOrdenado;
}

/**
 * Retorna o conjunto de todos os nós sem arestas de entrada
 **/
std::vector<int> Grafo::GetConjVertices() const {
	std::vector<int> retorno;
	retorno.clear();
	for(int i = 0; i < this->tamGrafo; i++) {
		if (this->GetVertice(i).GetId() == -1) continue;
		if (GetGrauEntrada(i) == 0) {
			retorno.push_back(i);
		}
	}
	return retorno;
}

int Grafo::GetGrauSaida(int id) const {
	int retorno = 0;
	for (int i=0; i < this->tamGrafo; i++) {
		if (this->Conexao(id,i)) retorno++;
	}
	return retorno;
}
int Grafo::GetGrauEntrada(int id) const {
	int retorno = 0;
	for (int i=0; i < this->tamGrafo; i++) {
		if (this->Conexao(i,id)) retorno++;
	}
	return retorno;
}
int Grafo::GetGrau(int id) const {
	int retorno = 0;
	retorno+= GetGrauEntrada(id);
	if (retorno >= 0)
		retorno+= GetGrauSaida(id);
	return retorno;
}

void Grafo::JsonVertice(int id) {
	cout << "{\"vertice\":{\"ID\": " << id << ", \"dado\":\"" << ( this->ExisteVertice(id) == true ? this->GetVertice(id).GetInformacao() : "") << "\", \"resposta\":\"" << ( this->ExisteVertice(id) == true ? "sucesso" : "falha") << "\"}}" << endl;
}

void Grafo::JsonRemove(int id) {
	cout << "{\"delete\":{\"ID\":" << id <<"\", resposta\":\"";
	if (this->ExisteVertice(id)) {
		if (this->RemoveVertice(id))
			cout << "sucesso";
		else
			cout << "falha";
	} else {
		cout << "falha";
	}
	cout << "\"}}" << endl;
}

void Grafo::JsonVizinhos(int id) {
	std::vector<int> vLista;
	cout << "{\"vizinhos\":{\"ID\":" << id << ", \"resposta\":\"" << (this->ExisteVertice(id) ? "sucesso" : "falha" ) << "\", \"vizinhos\":[";
	vLista = this->Vizinhos(id);
	for(int j = 0; j < (int) vLista.size(); j++) {
		if (this->ExisteVertice((int)vLista[j])) {
			if (j > 0) cout << ",";
			cout << vLista[j];
		}
	}
	cout << "]}}" << endl;
}

void Grafo::JsonConexao(int id1, int id2) {
	cout << "{\"conexao\":{\"ID1\":" << id1;
	cout << ", \"ID2\":" << id2;
	cout << ", \"resposta\":\"" << (this->ExisteVertice(id1) && this->ExisteVertice(id2) ? "sucesso" : "falha");
	cout << "\", \"conexao\":\"" << (this->ExisteVertice(id1) && this->ExisteVertice(id2) && this->lVertice[id1].GetAresta(id2)->GetPeso() != INT_MIN ? "sim" : "nao");
	cout << "\"}}" << endl;
}

void Grafo::JsonOrdemTopologica() {
	std::vector<int> ordem = this->OrdemTopologica();
	cout << "{\"ordemtop\":[";
	if ((int) ordem.size() > 0) {
	for (int i=0; i < (int) ordem.size(); i++)
		cout << (i > 0 ? "," : "") << ordem[i];
	}
	cout << "]}" << endl;
}
void Grafo::JsonArvoreMinima() {
	std::vector<Aresta *> lArestas = this->Kruskal();
	int custo = 0;
	int id1, id2;
	cout << "{\"arvoreminima\":{\"arestas\":[";
	for(int i = 0; i < (int) lArestas.size(); i++) {
		Aresta * cAresta = lArestas[i];
		if (i > 0) cout << ",";
		id1 = cAresta->GetId1();
		id2 = cAresta->GetId2();
		custo+= cAresta->GetPeso();
		cout << "(" << id1 << "," << id2 << ")";
	}
	cout << "],\"custo\":" << custo << "}}" << endl;
}

void Grafo::JsonMenorCaminho(int id1, int id2) {
	std::vector<int> lVertices = this->Dijkstra(id1,id2);
	int custo = 0;
	int id, idtmp;
	bool inicio = true;
	cout << "{\"menorcaminho\":{\"ID1\":" << id1 << ", \"ID2\":" << id2 << ", \"caminho\":[";
	//0,2,5,4,3
	while(!lVertices.empty()) {
		if (!inicio)
			cout << ",";
		else
			inicio = false;
		id = lVertices.back();
		lVertices.pop_back();
		if ((int)lVertices.size() > 0) {
			idtmp = lVertices.back();
			custo += this->GetVertice(id).GetAresta(idtmp)->GetPeso();
		}
		cout << id;
	}

	cout << "],\"custo\":" << custo << "}}" << endl;
}

std::vector<Aresta *> Grafo::Kruskal() {
	std::vector<Aresta *> retorno;
	//so faz se nao eh direcionado
	if (!this->Direcionado()) {
		//conjunto de vertices e ranks
		std::vector<int> ranks;
		//inicializa conjunto de vertices e ranks
		Heap * nHeap;
		int tamHeapMax = 0;
		conjKruskal.clear();
		ranks.clear();
		for (int i = 0; i < tamGrafo; i++) {
			this->conjKruskal.push_back(i);
			ranks.push_back(1);
			for (int j =0; j < tamGrafo; j++)
				if (this->Conexao(i,j))
					tamHeapMax++;
		}
		nHeap = new Heap(tamHeapMax);
		nHeap->SetTipo(MINIMO);
		for (int i = 0; i < tamGrafo; i++) {
			for (int j =i; j < tamGrafo; j++) {
				if (this->Conexao(i,j)) {
					Aresta * nAresta = new Aresta(*this->GetVertice(i).GetAresta(j));
					nHeap->Push(nAresta);
				}
			}
		}
		
		//nHeap->ListaHeap();
		int id1;
		int id2;
		int id1tmp;
		int id2tmp;

		std::vector<Aresta *> lArestas;

		while(nHeap->GetTamHeap() > 0 ) {
			Aresta * cAresta = nHeap->Pop();
			id1 = cAresta->GetId1();
			id2 = cAresta->GetId2();
			//cout << "id1: " << id1 << " id2:" << id2 << endl;
			
			if ((id1tmp = this->BuscaKruskal(id1)) != (id2tmp = BuscaKruskal(id2))) {
				//cout << "conjuntos diferentes: " << "id1: " << this->BuscaKruskal(id1) << " id2: " << this->BuscaKruskal(id2) << endl;
				retorno.push_back(cAresta);
				if (ranks[id1tmp] != ranks[id2tmp]) {
					int maior = (ranks[id1tmp] > ranks[id2tmp] ? id1tmp : id2tmp);
					int menor = (ranks[id2tmp] > ranks[id1tmp] ? id1tmp : id2tmp);
					conjKruskal[menor] = maior;
					//cout << "ranks: " << " menor: " << ranks[menor] << "maior: " << ranks[maior] << endl;
				} else {
					conjKruskal[id2tmp] = id1tmp;
					ranks[id1tmp] += 1;
					//cout << "aumentou rank" << endl;
				}
			}
		}
	}
	
	return retorno;
}

int Grafo::BuscaKruskal(int id) {
	int i = 0;
	int idOri = id;
	while (id != conjKruskal[id] && i < (int)conjKruskal.size()) {
		id = this->conjKruskal[id];
		this->conjKruskal[idOri] = id;
		i++;
	}
	return id;
}

std::vector<int> Grafo::Dijkstra(int id1, int id2) {
	std::vector<int> retorno;
	retorno.clear();
	std::vector<int> custoMenorCaminho;
	custoMenorCaminho.clear();
	/**

	Atribua valor zero à estimativa do custo mínimo do vértice s (a raiz da busca) e infinito às demais estimativas;
	Atribua um valor qualquer aos precedentes (o precedente de um vértice t é o vértice que precede t no caminho de custo mínimo de s para t);
	Enquanto houver vértice aberto:
		seja k um vértice ainda aberto cuja estimativa seja a menor dentre todos os vértices abertos;
			feche o vértice k
			Para todo vértice j ainda aberto que seja sucessor de k faça:
				some a estimativa do vértice k com o custo do arco que une k a j;
				caso esta soma seja melhor que a estimativa anterior para o vértice j, substitua-a e anote k como precedente de j.

	**/
	for (int i = 0; i < this->tamGrafo; i++)
		custoMenorCaminho.push_back(INT_MAX);
	this->conjDijkstra.clear();

	this->conjDijkstra.push_back(id1);
	vector<int> precedentes;
	precedentes.clear();
	//algoritmo para ordem a lista de vertices

	int id;
	int i = 0;
	//ordena os conjuntos por uma ordem meio milagrosa, verifica todos q tem conexao
	for (i = 0; i < this->tamGrafo; i++) {
		id = this->conjDijkstra[i];
		if (id < this->tamGrafo && id >= 0) {
			std::vector<int> lVizinhos = Vizinhos(id);
			for (int j = 0; j < (int)lVizinhos.size(); j++) {
				int idTmp = lVizinhos[j];
				if (!ExisteConjDijkstra(idTmp)) {
					this->conjDijkstra.push_back(idTmp);
				}
			}
		} else
			break;
	}
	for (int j = 0; j < this->tamGrafo; j++) {
		precedentes.push_back(id1);
	}

	//seta como zero o caminho inicial
	custoMenorCaminho[id1] = 0;
	int u = id1;
	while (!this->conjDijkstra.empty()) {
		//remove o 1o elemento do vector e coloca ele na variavel id
		u = this->conjDijkstra.front();
		Vertice * uVertice = new Vertice(this->GetVertice(u));
		this->conjDijkstra.erase(this->conjDijkstra.begin());
		for (int j = 0; j < (int)this->conjDijkstra.size(); j++) {
			int v = this->conjDijkstra[j];
			if (Conexao(u,v) ) {
				if (custoMenorCaminho[v] > custoMenorCaminho[u] + uVertice->GetAresta(v)->GetPeso() ) {
					custoMenorCaminho[v] = (custoMenorCaminho[u] != INT_MAX ? custoMenorCaminho[u] + uVertice->GetAresta(v)->GetPeso() : custoMenorCaminho[u]);
					precedentes[v] = u;
				}
			}
		}
		free(uVertice);
	}
	id = id2;
	if (custoMenorCaminho[id] == INT_MAX)
		retorno.clear();
	else {
		while (true) {
			retorno.push_back(id);
			id = precedentes[id];
			if (id == id1 || id == precedentes[id]) {
				retorno.push_back(id);
				break;
			}
		}
	}
	return retorno;
}

bool Grafo::ExisteConjDijkstra(int id) {
	for (int i=0; i < (int)this->conjDijkstra.size(); i++) {
		if (this->conjDijkstra[i] == id)
			return true;
	}
	return false;
}


/**
 * Implementacao Classe Aresta 
 **/
Aresta::Aresta() {
	this->id1 = -1;
	this->id2 = -1;
	this->peso = INT_MIN;
}

Aresta::Aresta(int id1, int id2, int peso) {
	this->id1 = id1;
	this->id2 = id2;
	this->peso = peso;
}
Aresta::Aresta(const Aresta & cAresta) {
	this->id1 = cAresta.GetId1();
	this->id2 = cAresta.GetId2();
	this->peso = cAresta.GetPeso();
}

bool Aresta::SetConexao(int id1, int id2) {
	bool retorno = false;
	if ((id1 == -1 && id2 == -1) || (this->id1 == -1 && this->id2 == -1)) {
		this->id1 = id1;
		this->id2 = id2;
		retorno = true;
	}
	return retorno;
}
int Aresta::GetId1() const {
	return this->id1;
}
int Aresta::GetId2() const {
	return this->id2;
}

int Aresta::GetPeso() const {
	return this->peso;
}
bool Aresta::SetPeso(int peso) {
	bool retorno = false;
	if (peso != this->peso) {
		this->peso = peso;
		retorno = true;
	}
	return retorno;
}
bool Aresta::Conexao(int id1,int id2) const {
	bool retorno = false;
	if (this->id1 == id1 && this->id2 == id2)
		retorno = true;
	return retorno;
}

bool Aresta::operator!=(const Aresta & b) {
	if (this->peso != b.peso) return true;
	if (this->id1 != b.id1) return true;
	if (this->id2 != b.id2) return true;
	return false;
}
bool Aresta::operator==(const Aresta & b) {
	if (this->peso != b.peso) return false;
	if (this->id1 != b.id1) return false;
	if (this->id2 != b.id2) return false;
	return true;
}
bool Aresta::operator==(int null) {
	bool retorno = false;
	if (null == 0)
		if (this->id1 == -1 && this->id2 == -1 && this->peso == INT_MIN)
			retorno = true;
	return retorno;
}
bool Aresta::operator!=(int null) {
	bool retorno = false;
	if (null == 0)
		if (this->id1 != -1 && this->id2 != -1 && this->peso != INT_MIN)
			retorno = true;
	return retorno;
}

/**
 * Implementacao classe Vertice
 **/
Vertice::Vertice() {
	this->id = -1;
	this->informacao.clear();
	this->tamLista = 0;
	this->lArestas = new Aresta();

	this->x = 5;
	this->y = 5;
	this->Dx = 1;
	this->Dy = 1;
	this->raio = 10;
	this->cor = {255,255,0};
}
Vertice::Vertice(const Vertice & cVertice) {
	this->id = cVertice.GetId();
	this->tamLista = cVertice.GetTamLista();
	this->informacao = cVertice.GetInformacao();
	this->lArestas = new Aresta [ tamLista ];
	for (int i = 0; i < tamLista; i++)
		this->lArestas[i] = *cVertice.GetAresta(i);

	this->x = 6;
	this->y = 6;
	this->Dx = 1;
	this->Dy = 1;
	this->raio = 10;
	this->cor = {255,255,0};
}
void Vertice::setDx(int X) {
	this->Dx = X;
}
void Vertice::setDy(int Y) {
	this->Dy = Y;
}
void Vertice::setY(int Y) {
	this->y = Y;
}
void Vertice::setX(int X) {
	this->x = X;
}
	
	
	
Vertice::Vertice(int id,const std::string informacao, int tamLista) {
	this->id = id;
	this->informacao = informacao;
	this->tamLista = tamLista;
	this->lArestas = new Aresta [ tamLista ];

	this->x = 0;
	this->y = 0;
	this->Dx = 0;
	this->Dy = 0;
	this->raio = 5;
	this->cor = {255,255,255};
}

bool Vertice::SetId(int id) {
	return false;
}
bool Vertice::SetInformacao(const std::string) {
	this->informacao = informacao;
	return true;
}
bool Vertice::SetConexao(int id2, int peso) {
	bool retorno = false;
	if (id2 >= 0  && id2 < tamLista && peso != INT_MIN) {
		if (this->lArestas[id2].Conexao(this->GetId(), id2)) {
			this->lArestas[id2].SetPeso(peso);
			//retorno = false;
		} else {
			this->lArestas[id2].SetConexao(this->GetId(), id2);
			this->lArestas[id2].SetPeso(peso);
		}
		retorno = true;
	}
	return retorno;
}
int Vertice::GetId() const {
	return this->id;
}
int Vertice::GetTamLista() const {
	return false;
}
Aresta * Vertice::GetAresta(int id2) const {
	Aresta * retorno = new Aresta();
	if (this->ExisteAresta(id2))
		retorno = &this->lArestas[id2];
	return retorno;
}
std::string Vertice::GetInformacao() const {
	return this->informacao;
}
bool Vertice::DelConexao(int id2) {
	bool retorno = false;
	if (id2 >= 0 && id2 < tamLista) {
		this->lArestas[id2].SetPeso(INT_MIN);
		this->SetConexao(-1,-1);
	}
	return retorno;
}
bool Vertice::Existe() const {
	bool retorno = false;
	if (this->GetId() != -1)
		retorno = true;
	return retorno;
}
bool Vertice::ExisteAresta(int id2) const {
	bool retorno = false;
	Aresta * cAresta = &this->lArestas[id2];
	
	if (cAresta != NULL)
		retorno = true;
	return retorno;
}
bool Vertice::Conexao(int id2) const {
	bool retorno = false;
	if (id2 >= 0 && id2 < tamLista) {
		retorno = (this->lArestas[id2].GetPeso() != INT_MIN ? true : false);
	}
	return retorno;
}

void Vertice::NoLaco() {
	//implementacao do algoritmo de repulsao e atracao
}
void Vertice::NaRenderizacao(SDL_Surface * planoExibicao) {
	cout << this->y << " " << this->x << " " << this->cor.r << " " << endl;
	filledCircleRGBA(planoExibicao, this->x, this->y, this->raio, this->cor.r, this->cor.g, this->cor.b, 255);
}





bool Vertice::operator==(int null) {
	bool retorno = false;
	//if (null == 0)
	return retorno;
}
bool Vertice::operator!=(const Vertice & b) {
	if (this->id != b.id) return true;
	if (this->informacao != b.informacao) return true;
	
	return false;
}
bool Vertice::operator!=(int null) {
	bool retorno = false;
	//if (null == 0)
	return retorno;
}
bool Vertice::operator==(const Vertice & b) {
	if (this->id != b.id) return false;
	if (this->informacao != b.informacao) return false;

	return true;
}

/**
 * Implementacao classe Heap
 **/
Heap::Heap(int tamHeap) {
	this->tamHeapMax = tamHeap;
	this->tamHeap = 0;
	this->arrayHeap = new Aresta [ this->tamHeapMax ];
	this->tipo = MAXIMO;
}
//Constutor de Copia
Heap::Heap(const Heap & cHeap) {
	this->tamHeapMax = cHeap.GetTamHeapMax();
	this->tamHeap = cHeap.GetTamHeap();
	
	this->arrayHeap = new Aresta [ this->tamHeapMax ];
	Aresta * cAresta;
	for (int i=0; i < this->tamHeap; i++) {
		cAresta = cHeap.GetAresta(i);
		this->Push(cAresta);
	}
	this->tipo = cHeap.GetTipo();
}


Aresta * Heap::GetAresta(int id) const {
	Aresta * retorno;
	retorno = &this->arrayHeap[id];
	return retorno;
}

int Heap::GetTamHeap() const {
	return this->tamHeap;
}
int Heap::GetTamHeapMax() const {
	return this->tamHeapMax;
}
int Heap::GetTipo() const {
	return this->tipo;
}
void Heap::SetTipo(int tipo) {
	this->tipo = tipo;
}
bool Heap::Push(Aresta * dados) {
	bool retorno = true;
	try {
		if (dados == NULL)
			throw 1;
		if (this->tamHeap >= this->tamHeapMax)
			throw 2;
		int indice = this->tamHeap;
		this->arrayHeap[indice] = *dados;
		if (indice-1 >= 0) {
			//calculo para encontrar pai: abs(ceil((indice  / (float)2) - 1))
			this->BalanceUp( (int) abs(ceil((indice / (float)2) - 1)),indice);
		}
		this->tamHeap++;
	} catch(int e) {
		string msgErro;
		msgErro = "ERRO: ";
		switch(e) {
			case 1:
				msgErro+= "Heap::Push dados nulos";
				break;
			case 2:
				msgErro+= "Heap::Push Limite da tabela excedido";
				break;
		}
		cout << msgErro << endl;
		retorno = false;
	}
	return retorno;
}

Aresta * Heap::Pop() {
	//retorna o topo
	Aresta * retorno = new Aresta(arrayHeap[0]);
	//seta o peso da posicao como menor negativo
	this->arrayHeap[0].SetPeso(INT_MIN);
	
	//pega ultimo indice do vetor
	int indice = this->tamHeap-1;
	//troca o 1o e ultimo indice de lugar
	this->TrocaAresta(0, indice);
	BalanceDown(0);
	this->tamHeap--;
	//troca de posicoes
	
	return retorno;
}
bool Heap::TrocaAresta(int id1, int id2) const {
	bool retorno = true;

	Aresta cAresta = this->arrayHeap[id1];
	this->arrayHeap[id1] = this->arrayHeap[id2];
	this->arrayHeap[id2] = cAresta;

	return retorno;
}
bool Heap::BalanceUp(int posPai, int posFilho) const {
	bool retorno = false;
	try {
		//verifica se filho e maior que pai, faz a troca, chama proximo
		if (this->tipo == MAXIMO) {
			if (arrayHeap[posFilho].GetPeso() > arrayHeap[posPai].GetPeso()) {
				this->TrocaAresta(posFilho, posPai);
				retorno = true;
				if (posPai > 0)
					retorno = (this->BalanceUp(abs(ceil((posPai  / (float)2) - 1)), posPai) ? true : retorno);
			}
		} else if (this->tipo == MINIMO) {
			if (arrayHeap[posFilho].GetPeso() < arrayHeap[posPai].GetPeso()) {
				this->TrocaAresta(posPai, posFilho);
				retorno = true;
				if (posPai > 0)
					retorno = (this->BalanceUp(abs(ceil((posPai  / (float)2) - 1)), posPai) ? true : retorno);
			}
		}
	} catch (...) {
		retorno = false;
	}
		return retorno;
}

bool Heap::BalanceDown(int posPai) const {
	bool retorno = false;
	try {
		//verifica se filho e maior que pai, faz a troca, chama proximo
		int posFilhoEsq = (posPai * 2) + 1;
		int posFilhoDir = (posPai * 2) + 2;
		int maiorFilho = -1;
		int menorFilho = -1;
		//se filho a esquerda for nulo, nao tem filhos
		if (posFilhoEsq < this->tamHeap && posFilhoDir < this->tamHeap) {
			if (arrayHeap[posFilhoEsq] != 0 && arrayHeap[posFilhoEsq].GetPeso() != INT_MIN) {
				//se filho a direita for nulo, entao filho a esquerda eh o maior
				if (this->tipo == MAXIMO) {
					if (arrayHeap[posFilhoDir] != 0 && arrayHeap[posFilhoDir].GetPeso() != INT_MIN) //se nao for nulo testa pra ver qual o maior
						maiorFilho = arrayHeap[posFilhoEsq].GetPeso() > arrayHeap[posFilhoDir].GetPeso() ? posFilhoEsq : posFilhoDir;
					else
						maiorFilho = posFilhoEsq;
					
					if (arrayHeap[maiorFilho].GetPeso() > arrayHeap[posPai].GetPeso()) {
						this->TrocaAresta(maiorFilho, posPai);
						retorno = true;
						retorno = (this->BalanceDown(maiorFilho) ? true : retorno);
					}
				} else if (this->tipo == MINIMO) {
					if (arrayHeap[posFilhoDir] != 0 && arrayHeap[posFilhoDir].GetPeso() != INT_MIN) //se nao for nulo testa pra ver qual o maior
						menorFilho = arrayHeap[posFilhoEsq].GetPeso() < arrayHeap[posFilhoDir].GetPeso() ? posFilhoEsq : posFilhoDir;
					else
						menorFilho = posFilhoEsq;
					
					if (arrayHeap[menorFilho].GetPeso() < arrayHeap[posPai].GetPeso()) {
						this->TrocaAresta(menorFilho, posPai);
						retorno = true;
						retorno = (this->BalanceDown(menorFilho) ? true : retorno);
					}
				}
			}
		}
	} catch (int e) {
		string msgErro;
		msgErro = "ERRO: ";
		switch(e) {
			case 1:
				break;
		}
		cout << msgErro << endl;
	}
		return retorno;
}

void Heap::ListaHeap() {
	for (int i = 0; i < tamHeap;i++) {
		cout << "id1: " << arrayHeap[i].GetId1();
		cout << " id2: " << arrayHeap[i].GetId2();
		cout << " peso: " << arrayHeap[i].GetPeso() << endl;
	}
}
Excecao::Excecao() {
}
bool Excecao::Tratamento(string msgErro) const {
	cout << msgErro << endl;
	return true;
}
bool Excecao::Tratamento(stringstream msgErro) const {
	cout << msgErro << endl;
	return true;
}
