#include "FSuperficie.h"
#include <iostream>
using namespace std;

FSuperficie::FSuperficie() {
}
 
SDL_Surface * FSuperficie::NoCarregar(char * Arquivo) {
    
    SDL_Surface* Surf_Temp = NULL;
    SDL_Surface* Surf_Retorno = NULL;

    if((Surf_Temp = IMG_Load(Arquivo)) == NULL) {
        return NULL;
    }
 
    Surf_Retorno = SDL_DisplayFormatAlpha(Surf_Temp);
    SDL_FreeSurface(Surf_Temp);
 
    return Surf_Retorno;
}

bool FSuperficie::NoDesenhar(SDL_Surface * Surf_Dest, SDL_Surface * Surf_Ori, int X, int Y) {
    if(Surf_Dest == NULL || Surf_Ori == NULL) {
        return false;
    }

    SDL_Rect DestR;
 
    DestR.x = X;
    DestR.y = Y;
 
    SDL_BlitSurface(Surf_Ori, NULL, Surf_Dest, &DestR);
 
    return true;
}

bool FSuperficie::NoDesenhar(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Ori, int X, int Y, int X2, int Y2, int W, int H) {
    if(Surf_Dest == NULL || Surf_Ori == NULL) {
        return false;
    }
 
    SDL_Rect DestR;
 
    DestR.x = X;
    DestR.y = Y;
 
    SDL_Rect OriR;
 
    OriR.x = X2;
    OriR.y = Y2;
    OriR.w = W;
    OriR.h = H;
 
    SDL_BlitSurface(Surf_Ori, &OriR, Surf_Dest, &DestR);
 
    return true;
}

bool FSuperficie::Transparencia(SDL_Surface * Surf_Dest, int R, int G, int B) {
    if(Surf_Dest == NULL) {
        return false;
    }
 
    SDL_SetColorKey(Surf_Dest, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Surf_Dest->format, R, G, B));
 
    return true;
}

//rotate image
/**
bool FSuperficie::Rotacionar(SDL_Surface * Surf_Orig, double angulo, double zoom, int smooth) {
	cout << "Girando e rodando: " << angulo << endl;
    //give initial for rotate image

    SDL_Surface * rotatefile;

    //get rotate here
    //rotatefile = rotozoomSurface(Surf_Orig, angulo, zoom, smooth);
    rotatefile = rotozoomSurface(Surf_Orig, angulo, zoom, smooth);

    //get optimizing
    Surf_Orig = SDL_DisplayFormat(rotatefile);

    //free surface
    SDL_FreeSurface(rotatefile);

    return true;
}
**/
