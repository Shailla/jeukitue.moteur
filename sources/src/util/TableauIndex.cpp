

#ifndef __JKT__TABLEAUINDEX_CPP
#define __JKT__TABLEAUINDEX_CPP

#include <iostream>

#include "TableauIndex.h"

namespace jkt
{

template<class X>
CTableauIndex<X>::CTableauIndex(int nbr) {
	m_Max = nbr;					// Nombre d'objets dans le tableau
	m_Nbr = 0;						// Aucun élément dans le tableau actuellement
	m_XTableau = new X*[ nbr ];		// Création du tableau

	for(int i=0 ; i < nbr ; i++)	// Mise à zéro des éléments du tableau
		m_XTableau[i] = 0;
}

template<class X>
CTableauIndex<X>::~CTableauIndex() {
	delete m_XTableau;		// Destruction du tableau
}


template<class X>
bool CTableauIndex<X>::Ajoute(int pos, X *objet) {	// Ajoute le pos° élément du tableau
													// s'il n'existe pas encore
	if(objet && pos < m_Max) {			// Vérifie qu'il n'y a pas dépassement du tableau
		if( !(m_XTableau[pos]) ) {		// Vérifie que la case indexée par 'pos' est vide
			m_XTableau[pos] = objet;
			m_Nbr++;			// Compte le nouvel élément
			return true;		// L'élément a été ajouté
		}
	}

	return false;		// L'élément n'a pas été ajouté à cause d'un problème
}

template<class X>
int CTableauIndex<X>::Ajoute(X *objet) {	// Ajoute l'élément à la première place de libre
	int id = 0;

	while(m_XTableau[id] && (id < m_Max))
		id++;

	if(id >= m_Max)			// Si aucune place n'a été trouvée dans le tableau
		return -1;			// Retour sans avoir placé le nouvel élément dans le tableau
	
	m_XTableau[id] = objet;	// Place l'élément à la place trouvée
	m_Nbr++;				// Compte le nouvel élément

	return id;				// Retourne l'index trouvé
}

template<class X>
bool CTableauIndex<X>::Supprime(int id) {		// Supprime le pos° élément du tableau
	if(id < m_Max) {		// S'il n'y a pas de dépassement
		m_XTableau[id] = 0;
		m_Nbr--;			// Décompte le nouvel élément
		return true;		// L'élément a été supprimé du tableau
	}

	return false;					// Lélément n'a pas été supprimé
}

template<class X>
X *CTableauIndex<X>::operator[](int id)		// Retourne le pointeur sur l'élément indexé 'pos'
{
#ifdef _DEBUG
	if( pos > m_Max )
		std::cerr << std::endl << __FILE__ << ":" << __LINE__ << "Erreur : acces 'CTableauIndex' dors limite";
#endif
	return m_XTableau[id];
}

template<class X>
inline
int CTableauIndex<X>::getMax()
{	return m_Max;		}

template<class X>
inline
int CTableauIndex<X>::getNbr()
{	return m_Nbr;		}

template<class X>
int CTableauIndex<X>::IndexSuivant(int id) {
	if(id < 0) {			// Initialisation d'un index
		id = -1;
	}

	if(id < m_Max - 1) {	// Incrément d'un index
		id++;

		while((id<m_Max) && (m_XTableau[id]==NULL))		// Cherche l'élément après la position 'id'
			id++;
	}
	else {					// Fin d'un index
		id = -1;
	}
	
	if(id >= m_Max) {
		id = -1;
	}

	return id;		// Envoie la position de l'élément trouvé ou de m_Nbr pour indiquer la fin de liste
}

template<class X>
bool CTableauIndex<X>::Suivant(int &id) {
	id = IndexSuivant(id);

	return id >= 0;
}

}	// JktUtils

#endif

