#pragma warning( disable : 4290 )	// Evite les warnings sur les déclaration de throw

#ifndef __JKT__TEXTURES_H
#define __JKT__TEXTURES_H

#include "util/Erreur.h"

namespace JktMoteur
{
class Icone;
class Texture;

/**
 * Lit un fichier image pour créer une texture affichable dans OpenGL.
 *
 * nomFichier : Chemin complet du fichier de l'image.
 * alpha : Valeur de la transparence comprise entre 0.0 et 1.0. L'alpha est désactivé si alpha < 0.
 */
Uint8* litFichierImage(const std::string &nomFichier, float alpha) throw(JktUtils::CErreur);

/**
 * Crée une texture affichable dans OpenGL à partir des données d'une image.
 *
 * nomFichier : Ressource ou identificateur quelquonque de l'image (utilisé pour les logs applicatifs).
 * alpha : Valeur de la transparence comprise entre 0.0 et 1.0. L'alpha est désactivé si alpha < 0.
 * pixels : Données de l'image.
 */
Texture* litFichierTexture(const std::string &nomFichier, float alpha, Uint8* pixels);

/**
 * Lit un fichier image pour créer une icone affichable dans OpenGL.
 *
 * nomFichier : Ressource ou identificateur quelquonque de l'image (utilisé pour les logs applicatifs).
 * alpha : Valeur de la transparence comprise entre 0.0 et 1.0. L'alpha est désactivé si alpha < 0.
 * pixels : Données de l'image.
 */
Icone* litFichierIcone(const std::string &nomFichier, float alpha, Uint8* pixels);

Uint32 getPixel(SDL_Surface *surface, int x, int y) throw(JktUtils::CErreur);

}	// JktMoteur

#endif

