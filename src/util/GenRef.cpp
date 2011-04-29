
#include <sstream>
#include <string>
#include <iostream>

#include "Trace.h"

#include "GenRef.h"

using namespace std;

namespace JktUtils
{

CGenRef::CBloc::CBloc(unsigned int debut, unsigned int fin)
{
	m_Debut = debut;
	m_Fin = fin;
}

CGenRef::CGenRef()
{
	sequence.push_back(CBloc(0,0));	// Création d'un premier bloc vide
									// (référence 0 = référence nulle)
}

CGenRef::~CGenRef()
{
}

unsigned int CGenRef::genRef()
{
	list<CBloc>::iterator iter = sequence.begin();

	list<CBloc>::iterator iterPremier = iter;	// Permier bloc
	iter++;
	list<CBloc>::iterator iterSecond = iter;	// Second bloc

	unsigned int ref = ++(*iterPremier).m_Fin;	// Incrémente la fin du premier bloc

	if(iterSecond != sequence.end())	// S'il y a un second bloc
	{
	    if((*iterSecond).m_Debut > 0) {
            if(ref == (unsigned int)((*iterSecond).m_Debut-1))	// Si fin du premier = début du second-1
            {		// Dans ce cas raccorde les 2 blocs
                (*iterPremier).m_Fin = (*iterSecond).m_Fin;
                sequence.erase(iterSecond);
            }
	    }
	}

	return ref;
}

string CGenRef::toString()
{
	ostringstream txt;
	list<CBloc>::iterator iter = sequence.begin();

	for( ; iter!=sequence.end(); iter++)
	{
		txt << "[" << (*iter).m_Debut << "," << (*iter).m_Fin << "]";
	}

	return txt.str();
}

void CGenRef::delRef(unsigned ref)
{
	if(ref == 0)
	{
		cout << endl << "Tentative de suppresion de la référence nulle";
		return;
	}

	list<CBloc>::iterator iter = sequence.begin();

		// Recherche du bloc contenant 'ref'
	for( ; iter!=sequence.end(); iter++)
	{
		if(ref >= (*iter).m_Debut && ref <= (*iter).m_Fin)
		{
			break;
		}
	}

	if(iter == sequence.end() || ref<(*iter).m_Debut)
	{
		TRACE().p( TRACE_ERROR, "Tentative de suppression d'une référence inconnue 'ref=%d'", ref);
		cout << endl << "Tentative de suppression d'une référence inconnue 'ref=" << ref << "'";
		return;
	}

	if((*iter).m_Debut == ref)		// 'ref' est au début du bloc
	{
		(*iter).m_Debut++;
		if((*iter).m_Debut > (*iter).m_Fin)	// Si le bloc est vide
		{
			sequence.erase(iter);	// Supprime-le
		}
	}
	else if((*iter).m_Fin == ref)	// 'ref' est à la fin du bloc
	{
		(*iter).m_Fin--;
		if((*iter).m_Fin < (*iter).m_Debut)	// Si le bloc est vide
		{
			sequence.erase(iter);	// Supprime-le
		}
	}
	else	// 'ref' est à l'intérieur du bloc, alors casse le bloc en 2
	{
		unsigned int fin = (*iter).m_Fin;

		(*iter).m_Fin = ref-1;

		sequence.insert(++iter,CBloc(ref+1,fin));
	}

	return;
}

}
