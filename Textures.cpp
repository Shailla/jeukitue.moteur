
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

#include "Erreur.h"
#include "RessourcesLoader.h"
#include "utils_SDL.h"

#include "Textures.h"

using namespace JKT_PACKAGE_UTILS;

namespace JKT_PACKAGE_MOTEUR3D
{

unsigned int LitFichierTexture(const string &nomFichier) throw(CErreur)
{
	#define largeur 128
	#define hauteur 128

	unsigned int texName = 0;
	
	string fichier = nomFichier;
	JKT_PACKAGE_UTILS::RessourcesLoader::getFileRessource(fichier);

	SDL_Surface *image;

	image = IMG_Load(fichier.c_str());		// Lit le fichier image de la texture
	if( image==0 )
	{
		string erreur;
		erreur = "\nErreur : Echec d'ouverture d'image de texture : " + fichier;
		cerr << endl << erreur << endl;
		throw CErreur( 0, erreur );
	}

	SDL_LockSurface(image);

		// Convertit la texture aux dimensions largeur*hauteur
	Uint8 tab[largeur][hauteur][4];

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
			tab[(unsigned int)i][(unsigned int)j][0] = red;
			tab[(unsigned int)i][(unsigned int)j][1] = green;
			tab[(unsigned int)i][(unsigned int)j][2] = blue;
			tab[(unsigned int)i][(unsigned int)j][3] = (Uint8)0;
		}
	
	SDL_UnlockSurface( image );
	SDL_FreeSurface( image );

		// Crée la texture
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures( 1, &texName );
	glBindTexture( GL_TEXTURE_2D, texName );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, largeur, hauteur,
			0, GL_RGBA, GL_UNSIGNED_BYTE, tab );

	return texName;
}

unsigned int LitFichierTextureAlpha( const string &nomFichier, float alpha )
{
	#define largeur 128
	#define hauteur 128

	unsigned int texName = 0;
	Uint8 Alpha = (Uint8)(alpha*256.0f);
	SDL_Surface *image;

	string fichier = nomFichier;
	JKT_PACKAGE_UTILS::RessourcesLoader::getFileRessource(fichier);

	image = IMG_Load(fichier.c_str());		// Lit le fichier image de la texture
	if(image == 0)
	{
		string erreur;
		erreur = "\nErreur : Echec d'ouverture d'image de texture : " + fichier;
		throw CErreur(0, erreur);
	}

	SDL_LockSurface( image );

		// Convertit la texture aux dimensions largeur*hauteur
	Uint8 tab[largeur][hauteur][4];

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
			tab[(unsigned int)i][(unsigned int)j][0] = red;
			tab[(unsigned int)i][(unsigned int)j][1] = green;
			tab[(unsigned int)i][(unsigned int)j][2] = blue;
			tab[(unsigned int)i][(unsigned int)j][3] = Alpha;
		}
	
	SDL_UnlockSurface( image );
	SDL_FreeSurface( image );

		// Crée la texture
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures( 1, &texName );
	glBindTexture( GL_TEXTURE_2D, texName );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, largeur, hauteur,
			0, GL_RGBA, GL_UNSIGNED_BYTE, tab );

	return texName;
}

}	// JKT_PACKAGE_MOTEUR3D
