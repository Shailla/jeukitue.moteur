
#pragma warning( disable : 4290 )	// Evite les warnings sur les déclaration de throw

#ifndef __JKT__TEXTURES_H
#define __JKT__TEXTURES_H

#include "util/Erreur.h"

namespace JktMoteur
{
class Icone;

unsigned int litFichierTexture(const std::string &nomFichier ) throw(JktUtils::CErreur);
unsigned int litFichierTextureAlpha(const std::string &nomFichier, float alpha) throw(JktUtils::CErreur);

Icone* litFichierIconeAlpha(const std::string &nomFichier, float alpha) throw(JktUtils::CErreur);

Uint32 getPixel(SDL_Surface *surface, int x, int y) throw(JktUtils::CErreur);

}	// JktMoteur

#endif

