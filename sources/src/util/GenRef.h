#ifndef __JKT__GENREF_H
#define __JKT__GENREF_H

/*
Générateur de références recyclées, une référence est un 'unsigned int' unique.
Les références sont fournies incrémentalement. Lorsqu'une référence est supprimée
alors la séquence est rompue et des blocs de façon transparente afin de pouvoir réallouer
la référence supprimée.

Exemple :
    - Action [Demande d'une référence] -> 0
    - Action [Demande d'une référence] -> 1
    - ...
    - Action [Demande d'une référence] -> 6
    - Action [Demande d'une référence] -> 7
On a pour l'instant 1 seul bloc : {0,1,2,3,4,5,6,7}
    - Action [Suppression de la référence 3]
On a maintenant 2 blocs : {0,1,2} et {4,5,6,7}
    - Action [Demande d'un référence] -> 3
On a à nouveau 1 seul bloc : {0,1,2,3,4,5,6,7}

*/

#include <vector>
#include <string>

namespace JktUtils
{
class CGenRef
{
	class CBloc
	{
		friend class CGenRef;
		unsigned int m_Debut;	// Début du bloc
		unsigned int m_Fin;		// Fin du bloc
		CBloc(unsigned int,unsigned int);
	};
	std::vector<CBloc> sequence;
public:
	CGenRef();
	~CGenRef();

	unsigned int genRef();			// Génère une nouvelle référence
	void delRef(unsigned int ref);	// Supprime une référence si elle existe
	std::string toString();
};
}

#endif

