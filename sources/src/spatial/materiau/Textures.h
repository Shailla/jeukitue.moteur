
#pragma warning( disable : 4290 )	// Evite les warnings sur les déclaration de throw

#ifndef __JKT__TEXTURES_H
#define __JKT__TEXTURES_H

#include "util/Erreur.h"

namespace JktMoteur
{
unsigned int LitFichierTexture( const std::string &nomFichier ) throw(JktUtils::CErreur);
unsigned int LitFichierTextureAlpha( const std::string &nomFichier, float alpha );

Uint32 getPixel(SDL_Surface *surface, int x, int y);

}	// JktMoteur

#endif

