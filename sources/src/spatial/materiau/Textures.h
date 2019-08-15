#pragma warning( disable : 4290 )	// Evite les warnings sur les d�claration de throw

#ifndef __JKT__TEXTURES_H
#define __JKT__TEXTURES_H

#include "util/Erreur.h"

namespace jkt
{
class Icone;
class Texture;

/**
 * Lit un fichier image pour cr�er une texture affichable dans OpenGL.
 *
 * nomFichier : Chemin complet du fichier de l'image.
 * alpha : Valeur de la transparence comprise entre 0.0 et 1.0. L'alpha est d�sactiv� si alpha < 0.
 */
Uint8* litFichierImage(const std::string &nomFichier, float alpha) noexcept(false);

/**
 * Cr�e une texture affichable dans OpenGL � partir des donn�es d'une image.
 *
 * nomFichier : Ressource ou identificateur quelquonque de l'image (utilis� pour les logs applicatifs).
 * alpha : Valeur de la transparence comprise entre 0.0 et 1.0. L'alpha est d�sactiv� si alpha < 0.
 * pixels : Donn�es de l'image.
 */
Texture* litFichierTexture(const std::string &nomFichier, float alpha, Uint8* pixels);

/**
 * Lit un fichier image pour cr�er une icone affichable dans OpenGL.
 *
 * nomFichier : Ressource ou identificateur quelquonque de l'image (utilis� pour les logs applicatifs).
 * alpha : Valeur de la transparence comprise entre 0.0 et 1.0. L'alpha est d�sactiv� si alpha < 0.
 * pixels : Donn�es de l'image.
 */
Icone* litFichierIcone(const std::string &nomFichier, float alpha, Uint8* pixels);

Uint32 getPixel(SDL_Surface *surface, int x, int y) ;

}	// JktMoteur

#endif

