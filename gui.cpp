#include "gui.h"

int main(int argc, char* argv[]) {

	App grafoGui;
	grafoGui.argc = argc;
	grafoGui.argv = *argv;
	
	return grafoGui.NaExecucao();
}

App::App() {
	planoExibicao = NULL;
	rodando = true;
	argc = 0;
	argv = 0;
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
		if ((planoExibicao = SDL_SetVideoMode(1024, 768, 24, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
			throw 2;


		bool debug = false;
		string argumento;

		if (argc > 1) {
			for(int i = 1; i < argc; i++) {
				argumento = argv[i];
				if (argumento.find("--debug") != std::string::npos) {
					debug = true;
				}
			}
		}

		string entrada;
		int tamGrafo;
		int estado = INICIAL;
		int v1;
		int v2;
		int peso;

		int id, id2;
		string identificacao, comando;
		std::vector<std::string> arrayGrafo, arrayVertice, arrayConexao, arrayAcao;
		char delim = ' ';
		char aspas = '"';
		while (cin.good() && cin) {
			getline (cin,entrada);
			if (entrada.find("*Vertices") != std::string::npos) {
				estado = GERA_GRAFO;
			} else if (entrada.find("*Edges") != std::string::npos) {
				estado = GERA_CONEXOES;
				getline (cin,entrada); //le proxima linha
			} else if (entrada.find("*Arcs") != std::string::npos) {
				estado = GERA_CONEXOES;
				mGrafo->SetDirecionado(true);
				getline (cin,entrada); //le proxima linha
			} else if (entrada.find("*Queries") != std::string::npos) {
				estado = ACOES;
				getline (cin,entrada); //le proxima linha
			}
			if (entrada[entrada.size()-1] == '@') break;
			switch(estado) {
				case GERA_GRAFO:
					//tamGrafo = stoi(entrada[9]);

					arrayGrafo = split(entrada,delim);
					if (arrayGrafo.size() <= 0) {
						estado = INICIAL;
					} else {
						tamGrafo = std::stoi(arrayGrafo[1]);
						if (debug) cout << "Criando Grafo: " << tamGrafo << endl;
						mGrafo = new Grafo(tamGrafo);
						estado = SETA_VERTICE;
					}
					break;
				case GERA_CONEXOES:
					arrayConexao = split(entrada,delim);
					if (arrayConexao.size() <= 0) {
						estado = INICIAL;
					} else {
						if (debug) cout << "Gerando Conexoes: " << "v1: " << arrayConexao[0] << " v2: " << arrayConexao[1] << " peso: " << arrayConexao[2];
						v1 = std::stoi(arrayConexao[0]);
						v2 = std::stoi(arrayConexao[1]);
						peso = std::stoi(arrayConexao[2]);
						mGrafo->CriaConexao(v1, v2, peso);
					}
					break;
				case ACOES:
					arrayAcao = split(entrada,delim);
					if (arrayAcao.size() <= 0) {
						estado = INICIAL;
					} else {
						if (debug) cout << "Acoes: " << entrada << endl;
						string acao = arrayAcao[0];
						if (acao.find("get") != std::string::npos) {
							id = std::stoi(arrayAcao[1]);
							mGrafo->JsonVertice(id);
						} else if (acao.find("delete") != std::string::npos) {
							id = std::stoi(arrayAcao[1]);
							mGrafo->JsonRemove(id);
						} else if (acao.find("vizinhos") != std::string::npos) {
							id = std::stoi(arrayAcao[1]);
							mGrafo->JsonVizinhos(id);
						} else if (acao.find("conexao") != std::string::npos) {
							id = std::stoi(arrayAcao[1]);
							id2 = std::stoi(arrayAcao[2]);
							mGrafo->JsonConexao(id,id2);
						} else if (acao.find("ordemtopologica") != std::string::npos) {
							mGrafo->JsonOrdemTopologica();

						} else if (acao.find("arvoreminima") != std::string::npos) {
							mGrafo->JsonArvoreMinima();

						} else if (acao.find("menorcaminho") != std::string::npos) {
							id = std::stoi(arrayAcao[1]);
							id2 = std::stoi(arrayAcao[2]);
							mGrafo->JsonMenorCaminho(id, id2);

						} else {
							cout << "Acao nao reconhecida: " << entrada << endl;
						}
					}
					break;
				case SETA_VERTICE:
					arrayVertice = split(entrada,aspas);
					if (arrayVertice.size() <= 0) {
						estado = INICIAL;
					} else {
						id = std::stoi(arrayVertice[0]);
						identificacao = arrayVertice[1];
						if (debug) cout << "Setando Vertice" << " id: " << id << " identificacao: " << identificacao << endl;
						mGrafo->SetVertice(id,identificacao);
					}
					break;
			}
		}
		int raio = 50;
		int x = raio;
		int y = raio;
		SDL_Color naosei = {255,255,0};
		for (int i = 0; i < mGrafo->GetTamGrafo();i++) {
			Vertice * nVertice = new Vertice(mGrafo->GetVertice(i));
			EntidadeVertice * nNodo = new EntidadeVertice(nVertice, raio);
			nNodo->x = rand()%planoExibicao->h;
			nNodo->y = rand()%planoExibicao->w;
			nNodo->cor = naosei;
			x+= nNodo->raio;
			//y+= nNodo->raio;
			Entidade::listaEntidades.push_back(nNodo);
		}

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

void App::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	if (sym == SDLK_q) {
		this->OnExit();
	}
}

void App::OnExit() {
	rodando = false;
}

double spring = 2; // atração
double damping = 2;
double charge = 2000;  // Repulsão
double Mass = 0.2;

void App::NoLaco() {
	for(int i = 0;i < (int) Entidade::listaEntidades.size();i++) {
		if(!Entidade::listaEntidades[i]) continue;
		for(int j = 0;j < (int) Entidade::listaEntidades.size();j++) {
			if(!Entidade::listaEntidades[i]) continue;
			if(j != i){
				double dx = Entidade::listaEntidades[j]->x - Entidade::listaEntidades[i]->x;
				double dy = Entidade::listaEntidades[j]->y - Entidade::listaEntidades[i]->y;
				double hypotenuse = sqrt(pow(dx, 2) + pow(dy, 2));
				double force = 0;
				if (rand()%1){
				   force = (hypotenuse -spring) / 2.0;
				}
				else{
				   force = -((Entidade::listaEntidades[j]->raio * Entidade::listaEntidades[i]->raio) / pow(hypotenuse, 2)) * charge;
				}
				dx /= hypotenuse;
				dy /= hypotenuse;
				dx *= force;
				dy *= force;
				Entidade::listaEntidades[i]->Dx += dx;
				Entidade::listaEntidades[i]->Dy += dy;
			}
		}
		if(Entidade::listaEntidades[i]->x + Entidade::listaEntidades[i]->Dx < planoExibicao->h)
			Entidade::listaEntidades[i]->x += Entidade::listaEntidades[i]->Dx;
			
		if(Entidade::listaEntidades[i]->y + Entidade::listaEntidades[i]->Dy < planoExibicao->w)
			Entidade::listaEntidades[i]->y += Entidade::listaEntidades[i]->Dy;
		
		if(Entidade::listaEntidades[i]->x < 0){
			Entidade::listaEntidades[i]->x = 1;
		}
		
		if(Entidade::listaEntidades[i]->y < 0){
			Entidade::listaEntidades[i]->y = 1;
		}
		
		Entidade::listaEntidades[i]->Dx *= damping;
		Entidade::listaEntidades[i]->Dy *= damping;
		Entidade::listaEntidades[i]->NoLaco();
	
	}
	SDL_Delay(100);
}

void App::NaRenderizacao() {
	SDL_FillRect(planoExibicao, NULL, SDL_MapRGB(planoExibicao->format, 0, 0, 0));

	//lineRGBA(planoExibicao,
    //           20, 10,
    //           70, 90,
    //           255, 0, 0, 255);
	//ellipseRGBA(planoExibicao,planoExibicao->w / 2, planoExibicao->h / 2, 50, 50, 255, 255, 0, 200);

	for(int i = 0;i < (int) Entidade::listaEntidades.size();i++) {
		if(!Entidade::listaEntidades[i]) continue;
		Entidade::listaEntidades[i]->NaRenderizacao(planoExibicao);
	}
	SDL_UpdateRect(planoExibicao, 0, 0, planoExibicao->w, planoExibicao->h);

	SDL_Flip(planoExibicao);
}

void App::NaLimpeza() {
	SDL_Quit();
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}
