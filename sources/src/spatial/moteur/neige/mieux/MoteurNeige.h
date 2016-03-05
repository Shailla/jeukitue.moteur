#ifndef __JKT__MOTEURNEIGE_H
#define __JKT__MOTEURNEIGE_H

#include <queue>

#include "util/V3D.h"

#include "spatial/moteur/MoteurParticules.h"
#include "spatial/moteur/neige/mieux/Flocon.h"

using namespace std;

class CGame;

namespace JktMoteur
{

class MoteurNeige : public CMoteurParticules {
	int _nbrParticules;					// Nombre total de flocons
	int _nbrParticulesOnGround;			// Nombre de flocons au sole
	Flocon* _flocons;					// Liste de tous les flocons
	queue<Flocon*> _floconsOnGround;	// Liste des flocons actuellement au sol
	unsigned int _texName;				// Texture du flocon

	JktUtils::CV3D _centre;
	JktUtils::CV3D _taille;
	JktUtils::CV3D _vitesse;

	void GenereTextureParticule();

	/** Calcule une position al�atoire pour une nouvelle particule. */
	void randomPosition(Flocon& flocon);
	void randomVelocity(Flocon& flocon);

public:
	/**
	 * Constructeur.
	 *
	 * @param nbrParticules nombre de particules dans le moteur
	 * @param position position du centre du moteur de particules dans le sc�ne OpenGL
	 * @param taille vecteur dimension du parall�l�pip�de dans lequel sont affich�es les particules
	 */
	MoteurNeige(int nbrParticules, int nbrParticulesOnGround, const CV3D& centre, const CV3D& taille);
	~MoteurNeige();

	// Fonctions d'intialisation et lib�ration
	void initGL() override;			// Initialisation de l'objet g�om�trique
	void freeGL() override;			// Lib�ration des ressources de l'objet dans le contexte OpenGL

	/** Affiche la neige. */
	void affiche() override;			// Affiche l'ensemble des particules
	void Refresh(CGame* game);
};

}	// JktMoteur

#endif
