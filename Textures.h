
#pragma warning( disable : 4290 )	// Evite les warnings sur les déclaration de throw

#ifndef __JKT__TEXTURES_H
#define __JKT__TEXTURES_H

#include "Erreur.h"

namespace JKT_PACKAGE_MOTEUR3D
{
unsigned int LitFichierTexture( const std::string &nomFichier ) throw(JKT_PACKAGE_UTILS::CErreur);
unsigned int LitFichierTextureAlpha( const std::string &nomFichier, float alpha );

Uint32 getPixel(SDL_Surface *surface, int x, int y);

}	// JKT_PACKAGE_MOTEUR3D

#endif

