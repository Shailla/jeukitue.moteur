
#include "util/V3D.h"

namespace JKT_PACKAGE_MOTEUR3D
{

class CMoteurParticules
{
		// Variables
	unsigned int nbrParticules;		// Nombre max de particules
	float masse;					// Masse du centre attracteur
	CParticule *ListeParticules;	// Liste des particules
	unsigned int texName;			// Texture de la particule
	unsigned lastTempsNew;	// date de la dernière création d'une particule
public:
	CV3D centre;						// Position du centre attracteur
	CV3D vitesse;
		// Fonctions membres
private:
	void GenereTextureParticule();

public:
		// Constructeurs
	CMoteurParticules( CV3D pos_centre, unsigned int nbr, float m );
		// Fonctions membres
	void Affiche();		// Affiche l'ensemble des particules
};

}	// JKT_PACKAGE_MOTEUR3D
