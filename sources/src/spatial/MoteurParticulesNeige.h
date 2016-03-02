#ifndef __JKT__MOTEURPARTICULESNEIGE_H
#define __JKT__MOTEURPARTICULESNEIGE_H

#include "util/V3D.h"
#include "spatial/MoteurParticules.h"

namespace JktMoteur
{

class CMoteurParticulesNeige : public CMoteurParticules {
	unsigned int _nbrParticules;		// Nombre max de particules
	float masse;					// Masse du centre attracteur
	CParticule* _particules;		// Liste des particules
	unsigned int _texName;			// Texture de la particule
	unsigned _lastTempsNew;			// Date de la derni�re cr�ation d'une particule

	/** Position du centre du moteur de particules. */
	JktUtils::CV3D _centre;
	/** Taille du moteur de particules. */
	JktUtils::CV3D _taille;
	JktUtils::CV3D _vitesse;

	void GenereTextureParticule();

	/** Calcule une position al�atoire pour une nouvelle particule. */
	CV3D getRandomParticulePosition();

public:
	/**
	 * Constructeur.
	 *
	 * @param nbrParticules nombre de particules dans le moteur
	 * @param position position du centre du moteur de particules dans le sc�ne OpenGL
	 * @param taille vecteur dimension du parall�l�pip�de dans lequel sont affich�es les particules
	 */
	CMoteurParticulesNeige(unsigned int nbrParticules, const CV3D& position, const CV3D& taille);
	~CMoteurParticulesNeige();

	// Fonctions d'intialisation et lib�ration
	void initGL() override;			// Initialisation de l'objet g�om�trique
	void freeGL() override;			// Lib�ration des ressources de l'objet dans le contexte OpenGL

	/** Affiche la neige. */
	void Affiche() override;		// Affiche l'ensemble des particules
};

}	// JktMoteur

#endif
