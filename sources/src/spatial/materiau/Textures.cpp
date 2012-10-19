
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"

using namespace std;

#include "util/Erreur.h"
#include "ressource/RessourcesLoader.h"
#include "util/utils_SDL.h"
#include "spatial/widget/Icone.h"
#include "spatial/widget/Texture.h"
#include "main/jkt.h"

#include "spatial/materiau/Textures.h"

using namespace JktUtils;

namespace JktMoteur
{

void* litFichierImage(const string &nomFichier, float alpha) throw(CErreur) {
	#define largeur 128
	#define hauteur 128

	Uint8 Alpha = (Uint8)(alpha*256.0f);
	SDL_Surface *image;

	string fichier = nomFichier;
	JktUtils::RessourcesLoader::getFileRessource(fichier);

	image = IMG_Load(fichier.c_str());		// Lit le fichier image de la texture
	if(image == 0) {
		string erreur;
		erreur = "\nErreur : Echec d'ouverture d'image de texture : " + fichier;
		throw CErreur(0, erreur);
	}

	SDL_LockSurface(image);

	// Convertit la texture aux dimensions largeur*hauteur
	Uint8 (*pixels)[hauteur][4] = new Uint8[largeur][hauteur][4];

	SDL_PixelFormat *fmt = image->format;
	Uint32 pixel;
	Uint8 red, green, blue;
	int x,y;

	for(float i=0 ; i<largeur ; i++)		// Mets les dimensions de l'image à un format compatible
		for(float j=0 ; j<hauteur ; j++)	// avec openGL (puissance de 2)
		{
			x = (unsigned int)(i*(float)image->w/largeur );
			y = (unsigned int)((hauteur-j-1)*(float)image->h/hauteur );

			if(x>image->w)
				x = image->w;

			if(y>image->h)
				y = image->h;

			pixel = util_SDL_getPixel( image, x, y );
			SDL_GetRGB( pixel, fmt, &red, &green, &blue );

			pixels[(unsigned int)i][(unsigned int)j][0] = red;
			pixels[(unsigned int)i][(unsigned int)j][1] = green;
			pixels[(unsigned int)i][(unsigned int)j][2] = blue;
			pixels[(unsigned int)i][(unsigned int)j][3] = Alpha;
		}

	SDL_UnlockSurface(image);
	SDL_FreeSurface(image);

	return pixels;
}

Texture* litFichierTexture(const string &nomFichier, float alpha, void* pixels) {
	bool isAlphaActive;

	if(alpha >= 0) {
		isAlphaActive = true;
	}
	else {
		isAlphaActive = false;
	}

	Texture* texture = new Texture(nomFichier, isAlphaActive, (GLsizei) hauteur, (GLsizei)largeur, pixels);

	addGraphicObjectToInitialize(texture);

	return texture;
}


Icone* litFichierIcone(const string &nomFichier, float alpha, void* pixels) {
	bool isAlphaActive;

	if(alpha >= 0) {
		isAlphaActive = true;
	}
	else {
		isAlphaActive = false;
	}

	Icone* icone = new Icone(nomFichier, isAlphaActive, (GLsizei) hauteur, (GLsizei)largeur, pixels);

	addGraphicObjectToInitialize(icone);

	return icone;
}

}	// JktMoteur
