/*
 * Icone.h
 *
 *  Created on: 15 oct. 2012
 *      Author: Erwin
 */

#ifndef ICONE_H_
#define ICONE_H_

#include "string"

using namespace std;

#include "SDL.h"

#include "spatial/widget/GraphicObject.h"

namespace jkt {

/**
 * Icone affichage avec OpenGL.
 */
class Icone : public GraphicObject {
	/** Nom du fichier de l'image qui a servi � cr�er l'icone. */
	string _nomFichier;

	/** Indique si la transparence est active ou non pour cette icone. */
	bool _alpha;

	/** Hauteur de l'icone en pixels. */
	int _hauteur;

	/** Largeur de l'icone en pixels. */
	int _largeur;

	/** Pixels de l'icone. */
	Uint8* _pixels;

	/** R�f�rence OpenGL attribu�e � la texture, 0 si la r�f�rence n'a pas encore �t� attribu�e. */
	unsigned int _glTexName;

public:
	/**
	 * Constructeur.
	 *
	 * nomFichier : Ressource du fichier image qui a servi � cr�er l'icone.
	 * alpha : Indique si la transparence est active pour cette icone.
	 * hauteur : Hauteur de l'icone en pixels.
	 * largeur : Largeur de l'icone en pixels.
	 * pixels : Pixels de l'icone.
	 */
	Icone(const string& nomFichier, bool alpha, int hauteur, int largeur, Uint8* pixels);

	/** Destructeur. */
	virtual ~Icone();

	/** Alloue les ressources OpenGL de l'icone. */
	void initializeGraphicObject(void);

	/** Lib�re les ressources OpenGL. */
	void destructGraphicObject(void);

	/**
	 * Affiche l'icone aux coordonn�es indiqu�es.
	 *
	 * left : Position gauche du cadre de l'icone.
	 * right : Position droite du cadre de l'icone.
	 * down : Position basse du cadre de l'icone.
	 * up : Position haute du cadre de l'icone.
	 */
	void affiche(float left, float right, float down, float up);
};

} /* namespace jkt */
#endif /* ICONE_H_ */
