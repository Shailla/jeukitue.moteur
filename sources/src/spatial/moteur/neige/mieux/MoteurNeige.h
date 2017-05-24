#ifndef __JKT__MOTEURNEIGE_H
#define __JKT__MOTEURNEIGE_H

#include <queue>

#include "util/V3D.h"

#include "spatial/moteur/MoteurParticules.h"
#include "spatial/moteur/neige/mieux/Flocon.h"

class CGame;

namespace jkt
{

class MoteurNeige : public CMoteurParticules {
	int _nbrParticules;					// Nombre total de flocons
	int _nbrParticulesOnGround;			// Nombre de flocons au sole
	Flocon* _flocons;					// Liste de tous les flocons
	std::queue<Flocon*> _floconsOnGround;	// Liste des flocons actuellement au sol
	unsigned int _texName;				// Texture du flocon

	jkt::CV3D _centre;
	jkt::CV3D _taille;
	jkt::CV3D _vitesse;

	void GenereTextureParticule();

	/** Calcule une position aléatoire pour une nouvelle particule. */
	void randomPosition(Flocon& flocon);
	void randomVelocity(Flocon& flocon);

public:
	/**
	 * Constructeur.
	 *
	 * @param nbrParticules nombre de particules dans le moteur
	 * @param position position du centre du moteur de particules dans le scène OpenGL
	 * @param taille vecteur dimension du parallélépipède dans lequel sont affichées les particules
	 */
	MoteurNeige(CMap* parent);
	~MoteurNeige();

	MapObject* clone() override;
	void init() throw(CErreur) override;

	bool Lit(TiXmlElement* el, MapLogger* mapLogger) override;	// Lit l'objet depuis un fichier Map
	bool Save(TiXmlElement* element) override;					// Sauve l'objet dans un fichier Map

	// Fonctions d'intialisation et libération
	void initGL() override;			// Initialisation de l'objet géométrique
	void freeGL() override;			// Libération des ressources de l'objet dans le contexte OpenGL

	/** Affiche la neige. */
	void Affiche() override;								// Affiche l'ensemble des particules
	void AfficheHighlighted(float r,float v,float b) override;
	void refresh(CGame* game) override;
};

}	// JktMoteur

#endif
