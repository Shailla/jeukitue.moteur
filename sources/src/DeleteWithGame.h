
#ifndef __JKT__DELETEWITHGAME_H
#define __JKT__DELETEWITHGAME_H

#include <set>
using namespace std;

class CDeleteWithGame;

class CDeleteWithGame {

public:
		// Constructeur/destructeur
	CDeleteWithGame();
	~CDeleteWithGame();

    static set<CDeleteWithGame*>* m_Tab;
	static void DeleteAll();			// D�truit toutes les classes d�riv�es de CDeleteWithGame
};

#endif
