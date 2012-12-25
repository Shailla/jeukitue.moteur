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

		// Fonctions membres
	virtual void Affiche() = 0;		// Affiche l'ensemble des particules
};

}	// JktMoteur

#endif
