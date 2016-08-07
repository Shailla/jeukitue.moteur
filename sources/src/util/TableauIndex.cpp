

#ifndef __JKT__TABLEAUINDEX_CPP
#define __JKT__TABLEAUINDEX_CPP

#include <iostream>

#include "TableauIndex.h"

namespace jkt
{

template<class X>
TableauIndex<X>::TableauIndex(bool manageDeletion) : TableauIndex(0, manageDeletion) {
}

template<class X>
TableauIndex<X>::TableauIndex(int nbr, bool manageDeletion) {
	_manageDeletion = manageDeletion;

	m_Max = nbr;					// Nombre d'objets dans le tableau
	m_Nbr = 0;						// Aucun élément dans le tableau actuellement
	m_XTableau = new X*[ nbr ];		// Création du tableau

	for(int i=0 ; i < m_Max ; i++)	// Mise à zéro des éléments du tableau
		m_XTableau[i] = 0;
}

template<class X>
void TableauIndex<X>::clear() {
	reinit(0);
}

template<class X>
void TableauIndex<X>::reinit(int nbr) {
	// Suppression des éléments existants
	for(int i=0 ; i < m_Max ; i++) {
		if(_manageDeletion && m_XTableau[i]) {
			delete m_XTableau[i];
		}
	}

	delete[] m_XTableau;		// Destruction du tableau

	// Création d'un nouveau tableau
	m_Max = nbr;					// Nombre d'objets dans le tableau
	m_Nbr = 0;						// Aucun élément dans le tableau actuellement
	m_XTableau = new X*[ nbr ];		// Création du tableau

	for(int i=0 ; i < m_Max ; i++)	// Mise à zéro des éléments du tableau
		m_XTableau[i] = 0;
}

template<class X>
TableauIndex<X>::~TableauIndex() {
	delete[] m_XTableau;		// Destruction du tableau
}


template<class X>
bool TableauIndex<X>::add(int id, X *objet) {	// Ajoute le pos° élément du tableau
													// s'il n'existe pas encore
	if(objet && 0 <= id && id < m_Max) {			// Vérifie qu'il n'y a pas dépassement du tableau
		if( !(m_XTableau[id]) ) {		// Vérifie que la case indexée par 'pos' est vide
			m_XTableau[id] = objet;
			m_Nbr++;			// Compte le nouvel élément
			return true;		// L'élément a été ajouté
		}
	}

	return false;		// L'élément n'a pas été ajouté à cause d'un problème
}

template<class X>
int TableauIndex<X>::add(X *objet) {	// Ajoute l'élément à la première place de libre
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
bool TableauIndex<X>::Supprime(int id) {		// Supprime l'élément du tableau
	if(0 <= id && id < m_Max) {		// S'il n'y a pas de dépassement
		if(_manageDeletion && m_XTableau[id]) {
			delete m_XTableau[id];
		}

		m_XTableau[id] = 0;
		m_Nbr--;			// Décompte le nouvel élément
		return true;		// L'élément a été supprimé du tableau
	}

	return false;		// Aucun élément n'a pas été supprimé
}

template<class X>
X *TableauIndex<X>::operator[](int id)		// Retourne le pointeur sur l'élément indexé 'id'
{
	if(0 <= id && id < m_Max) {
		return m_XTableau[id];
	}
	else {
		return 0;
	}
}

template<class X>
inline
int TableauIndex<X>::getMax() const {
	return m_Max;
}

template<class X>
inline
int TableauIndex<X>::getNbr() const {
	return m_Nbr;
}

template<class X>
int TableauIndex<X>::IndexSuivant(int id) {
	if(id < 0) {			// Initialisation d'un index
		id = -1;
	}

	if(id < m_Max - 1) {	// Incrément d'un index
		id++;

		while((id < m_Max) && (m_XTableau[id] == 0))		// Cherche l'élément après la position 'id'
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
bool TableauIndex<X>::Suivant(int &id) {
	id = IndexSuivant(id);

	return id >= 0;
}

}	// JktUtils

#endif

