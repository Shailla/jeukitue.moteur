#ifndef __JKT__MOTEURPARTICULESNEIGE_H
#define __JKT__MOTEURPARTICULESNEIGE_H

#include "util/V3D.h"
#include "spatial/MoteurParticules.h"

namespace JktMoteur
{

class CMoteurParticulesNeige : public CMoteurParticules
{
		// Variables
	unsigned int nbrParticules;		// Nombre max de particules
	float masse;					// Masse du centre attracteur
	CParticule *ListeParticules;	// Liste des particules
	unsigned int texName;			// Texture de la particule
	unsigned lastTempsNew;			// Date de la dernière création d'une particule
public:
	CV3D centre;						// Position du centre attracteur
	CV3D vitesse;
		// Fonctions membres
private:
	void GenereTextureParticule();

public:
		// Constructeurs
	CMoteurParticulesNeige( CV3D pos_centre, unsigned int nbr, float m );
		// Fonctions membres
	void Affiche();		// Affiche l'ensemble des particules
};

}	// JktMoteur

#endif
