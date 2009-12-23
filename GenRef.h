#ifndef __JKT__GENREF_H
#define __JKT__GENREF_H

/*	Générateur de références optimisé, une référence est un 'unsigned int' unique
*/

#include <list>
#include <string>

namespace JKT_PACKAGE_UTILS
{
class CGenRef
{
	class CBloc
	{
		friend CGenRef;
		unsigned int m_Debut;	// Début du bloc
		unsigned int m_Fin;		// Fin du bloc
		CBloc(unsigned int,unsigned int);
	};
	std::list<CBloc> sequence;
public:
	CGenRef();
	~CGenRef();

	unsigned int genRef();			// Génère une nouvelle référence
	void delRef(unsigned int ref);	// Supprime une référence si elle existe
	std::string toString();
};
}

#endif

