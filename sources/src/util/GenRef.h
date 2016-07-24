#ifndef __JKT__GENREF_H
#define __JKT__GENREF_H

/*
G�n�rateur de r�f�rences recycl�es, une r�f�rence est un 'unsigned int' unique.
Les r�f�rences sont fournies incr�mentalement. Lorsqu'une r�f�rence est supprim�e
alors la s�quence est rompue et des blocs de fa�on transparente afin de pouvoir r�allouer
la r�f�rence supprim�e.

Exemple :
    - Action [Demande d'une r�f�rence] -> 0
    - Action [Demande d'une r�f�rence] -> 1
    - ...
    - Action [Demande d'une r�f�rence] -> 6
    - Action [Demande d'une r�f�rence] -> 7
On a pour l'instant 1 seul bloc : {0,1,2,3,4,5,6,7}
    - Action [Suppression de la r�f�rence 3]
On a maintenant 2 blocs : {0,1,2} et {4,5,6,7}
    - Action [Demande d'un r�f�rence] -> 3
On a � nouveau 1 seul bloc : {0,1,2,3,4,5,6,7}

*/

#include <vector>
#include <string>

namespace jkt
{
class CGenRef
{
	class CBloc
	{
		friend class CGenRef;
		unsigned int m_Debut;	// D�but du bloc
		unsigned int m_Fin;		// Fin du bloc
		CBloc(unsigned int,unsigned int);
	};
	std::vector<CBloc> sequence;
public:
	CGenRef();
	~CGenRef();

	unsigned int genRef();			// G�n�re une nouvelle r�f�rence
	void delRef(unsigned int ref);	// Supprime une r�f�rence si elle existe
	std::string toString();
};
}

#endif

