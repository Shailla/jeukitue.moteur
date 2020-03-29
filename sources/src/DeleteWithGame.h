
#ifndef __JKT__DELETEWITHGAME_H
#define __JKT__DELETEWITHGAME_H

#include <set>

#include "includes.h"

class CDeleteWithGame {

public:
		// Constructeur/destructeur
	CDeleteWithGame();
	~CDeleteWithGame();

    static std::set<CDeleteWithGame*>* m_Tab;
	static void DeleteAll();			// Détruit toutes les classes dérivées de CDeleteWithGame
};

#endif
