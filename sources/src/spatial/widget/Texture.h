/*
 * Texture.h
 *
 *  Created on: 15 oct. 2012
 *      Author: Erwin
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "string"

using namespace std;

#include "SDL.h"

#include "spatial/widget/GraphicObject.h"

namespace jkt {
class CV3D;
}

using namespace jkt;

namespace jkt {

/**
 * Icone affichage avec OpenGL.
 */
class Texture : public GraphicObject {
	/** Nom du fichier de l'image qui a servi à créer l'icone. */
	string _nomFichier;

	/** Indique si la transparence est active ou non pour cette icone. */
	bool _alpha;

	/** Hauteur de l'icone en pixels. */
	int _hauteur;

	/** Largeur de l'icone en pixels. */
	int _largeur;

	/** Pixels de l'icone. */
	Uint8* _pixels;

	/** Référence OpenGL attribuée à la texture, 0 si la référence n'a pas encore été attribuée. */
	unsigned int _glTexName;

public:
	/**
	 * Constructeur.
	 *
	 * nomFichier : Ressource du fichier image qui a servi à créer l'icone.
	 * alpha : Indique si la transparence est active pour cette icone.
	 * hauteur : Hauteur de l'icone en pixels.
	 * largeur : Largeur de l'icone en pixels.
	 * pixels : Pixels de l'icone.
	 */
	Texture(const string& nomFichier, bool alpha, int hauteur, int largeur, Uint8* pixels);

	/** Destructeur. */
	virtual ~Texture();

	/** Alloue les ressources OpenGL de la texture. */
	void initializeGraphicObject(void);

	/** Libère les ressources OpenGL. */
	void destructGraphicObject(void);

	unsigned int getGlTexName(void);

	/**
	 * Affiche la texture dans un rectangle aux coordonnées indiquées.
	 *
	 *
	 */
	void afficheQuad(const CV3D& a, const CV3D& b, const CV3D& c, const CV3D& d);
};

} /* namespace jkt */
#endif /* ICONE_H_ */
