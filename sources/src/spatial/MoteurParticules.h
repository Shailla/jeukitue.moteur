#ifndef __JKT__MOTEURPARTICULES_H
#define __JKT__MOTEURPARTICULES_H

namespace JktMoteur
{

class CMoteurParticules {
public:
		// Fonctions membres
private:

public:
		// Constructeurs
	CMoteurParticules(void);
	virtual ~CMoteurParticules(void);

	virtual void initGL() = 0;								// Initialisation de l'objet g�om�trique
	virtual void freeGL() = 0;								// Lib�ration des ressources de l'objet dans le contexte OpenGL

	virtual void Affiche() = 0;		// Affiche l'ensemble des particules
};

}	// JktMoteur

#endif
