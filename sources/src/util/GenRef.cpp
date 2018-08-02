
#include <sstream>
#include <string>
#include <iostream>

#include "Trace.h"

#include "GenRef.h"

using namespace std;

namespace jkt
{

CGenRef::CBloc::CBloc(unsigned int debut, unsigned int fin) {
	m_Debut = debut;
	m_Fin = fin;
}

CGenRef::CGenRef() {
	sequence.push_back(CBloc(0,0));	// Cr�ation d'un premier bloc vide (r�f�rence 0 = r�f�rence nulle)
}

CGenRef::~CGenRef() {
}

unsigned int CGenRef::genRef() {
	vector<CBloc>::iterator iter = sequence.begin();

	vector<CBloc>::iterator iterPremier = iter;	// Permier bloc
	CBloc& blocPremier = *iterPremier;

	iter++;
	vector<CBloc>::iterator iterSecond = iter;	// Second bloc
	CBloc& blocSecond = *iterSecond;

	unsigned int ref = ++blocPremier.m_Fin;	// Incr�mente la fin du premier bloc

	if(iterSecond != sequence.end()) {	// S'il y a un second bloc
	    if((*iterSecond).m_Debut > 0) {
            if(ref == (unsigned int)(blocSecond.m_Debut-1)) {	// Si fin du premier = d�but du second-1, dans ce cas raccorde les 2 blocs
                (*iterPremier).m_Fin = blocSecond.m_Fin;
                sequence.erase(iterSecond);
            }
	    }
	}

	return ref;
}

string CGenRef::toString() {
	ostringstream txt;
	vector<CBloc>::iterator iter = sequence.begin();

	for( ; iter!=sequence.end(); iter++) {
		CBloc& bloc = *iter;
		txt << "[" << bloc.m_Debut << "," << bloc.m_Fin << "]";
	}

	return txt.str();
}

void CGenRef::delRef(unsigned ref) {
	if(ref == 0) {
		cout << endl << "Tentative de suppresion de la r�f�rence nulle";
		return;
	}

	vector<CBloc>::iterator iter;

		// Recherche du bloc contenant 'ref'
	for(iter = sequence.begin() ; iter!=sequence.end(); iter++){
		CBloc& bloc = *iter;

		if(ref >= bloc.m_Debut && ref <= bloc.m_Fin) {
			break;
		}
	}

	if(iter == sequence.end() || ref<(*iter).m_Debut) {
		LOGDEBUG(("Tentative de suppression d'une r�f�rence inconnue 'ref=%d'", ref));
		cout << endl << "Tentative de suppression d'une r�f�rence inconnue 'ref=" << ref << "'";
		return;
	}

	if((*iter).m_Debut == ref) {		// 'ref' est au d�but du bloc
		(*iter).m_Debut++;

		if((*iter).m_Debut > (*iter).m_Fin) {	// Si le bloc est vide
			sequence.erase(iter);	// Supprime-le
		}
	}
	else if((*iter).m_Fin == ref) {	// 'ref' est � la fin du bloc
		(*iter).m_Fin--;

		if((*iter).m_Fin < (*iter).m_Debut) {	// Si le bloc est vide
			sequence.erase(iter);	// Supprime-le
		}
	}
	else {	// 'ref' est � l'int�rieur du bloc, alors casse le bloc en 2
		unsigned int fin = (*iter).m_Fin;

		(*iter).m_Fin = ref-1;

		sequence.insert(++iter,CBloc(ref+1,fin));
	}

	return;
}

}
