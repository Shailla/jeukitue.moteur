
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
	static void DeleteAll();			// D�truit toutes les classes d�riv�es de CDeleteWithGame
};

#endif
