
#ifndef __JKT__DELETEWITHGAME_H
#define __JKT__DELETEWITHGAME_H

#include <set>
using namespace std;

class CDeleteWithGame;

class CDeleteWithGame
{
public:
		// Constructeur/destructeur
	static set<CDeleteWithGame*> m_Tab;
	static void DeleteAll();			// Détruit toutes les classes dérivées de CDeleteWithGame

	CDeleteWithGame();
	~CDeleteWithGame();
};

#endif
