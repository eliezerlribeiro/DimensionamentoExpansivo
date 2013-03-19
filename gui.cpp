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
		int x = 100;
		int y = 100;
		int raio = 5;
		for (int i=0; i< mGrafo->GetTamGrafo();i++) {
			 x= (rand()%500) + 50;
			 y= (rand()%500) + 50;
			 mGrafo->lVertice[i].setX(x);
			 mGrafo->lVertice[i].setY(y);
		}
	} catch (int e) {
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
	for(int i = 0;i < (int) mGrafo->GetTamGrafo();i++) {
		for(int j = 0;j < (int) mGrafo->GetTamGrafo();j++) {
			if(j != i){
				double dx = mGrafo->lVertice[j].x - mGrafo->lVertice[i].x;
				double dy = mGrafo->lVertice[j].y - mGrafo->lVertice[i].y;
				double hypotenuse = sqrt(pow(dx, 2) + pow(dy, 2));
				double force = 0;	
				if ( mGrafo->Vizinho(i , j) ){
				   force = (hypotenuse -spring) / 2.0;
				}
				else{
				   force = -((mGrafo->lVertice[j].raio * mGrafo->lVertice[i].raio) / pow(hypotenuse, 2)) * charge;
				}
				dx /= hypotenuse;
				dy /= hypotenuse;
				dx *= force;
				dy *= force;
				
				mGrafo->lVertice[i].setDx(mGrafo->lVertice[i].Dx+dx);
				mGrafo->lVertice[i].setDy(mGrafo->lVertice[i].Dy+dy);
			}
		}
		if(mGrafo->lVertice[i].x + mGrafo->lVertice[i].Dx < planoExibicao->h)
			mGrafo->lVertice[i].setX(mGrafo->lVertice[i].x + mGrafo->lVertice[i].Dx);
			
		if(mGrafo->lVertice[i].y + mGrafo->lVertice[i].Dy < planoExibicao->w)
			mGrafo->lVertice[i].setY(mGrafo->lVertice[i].y + mGrafo->lVertice[i].Dy);
		
		if(mGrafo->lVertice[i].x < 50){
			mGrafo->lVertice[i].setX(50);
		}
		
		if(mGrafo->lVertice[i].y < 50){
			mGrafo->lVertice[i].setY(50);
		}
		
		mGrafo->lVertice[i].setDx(mGrafo->lVertice[i].Dx * damping);
		mGrafo->lVertice[i].setDy(mGrafo->lVertice[i].Dy * damping);

	
	}
	SDL_Delay(100);
}

void App::NaRenderizacao() {
	SDL_FillRect(planoExibicao, NULL, SDL_MapRGB(planoExibicao->format, 0, 0, 0));
/**
	for(int i = 0;i < (int) Entidade::listaEntidades.size();i++) {
		if(!Entidade::listaEntidades[i]) continue;
		Entidade::listaEntidades[i]->NaRenderizacao(planoExibicao);
	}
**/
	for(int i = 0;i < mGrafo->GetTamGrafo();i++) {
		mGrafo->GetVertice(i).NaRenderizacao(planoExibicao);
		for (int j = 0; j < mGrafo->GetTamGrafo(); j++) {
			 if (mGrafo->Conexao(i,j)) {
			 lineRGBA(planoExibicao, mGrafo->GetVertice(i).x, mGrafo->GetVertice(i).y, mGrafo->GetVertice(j).x, mGrafo->GetVertice(j).y, mGrafo->GetVertice(i).cor.r , mGrafo->GetVertice(i).cor.g, mGrafo->GetVertice(i).cor.b, 255);
		 }
		}
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
