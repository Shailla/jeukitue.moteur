

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
	m_Nbr = 0;						// Aucun �l�ment dans le tableau actuellement
	m_XTableau = new X*[ nbr ];		// Cr�ation du tableau

	for(int i=0 ; i < m_Max ; i++)	// Mise � z�ro des �l�ments du tableau
		m_XTableau[i] = 0;
}

template<class X>
void TableauIndex<X>::clear() {
	reinit(0);
}

template<class X>
void TableauIndex<X>::reinit(int nbr) {
	// Suppression des �l�ments existants
	for(int i=0 ; i < m_Max ; i++) {
		if(_manageDeletion && m_XTableau[i]) {
			delete m_XTableau[i];
		}
	}

	delete[] m_XTableau;		// Destruction du tableau

	// Cr�ation d'un nouveau tableau
	m_Max = nbr;					// Nombre d'objets dans le tableau
	m_Nbr = 0;						// Aucun �l�ment dans le tableau actuellement
	m_XTableau = new X*[ nbr ];		// Cr�ation du tableau

	for(int i=0 ; i < m_Max ; i++)	// Mise � z�ro des �l�ments du tableau
		m_XTableau[i] = 0;
}

template<class X>
TableauIndex<X>::~TableauIndex() {
	delete[] m_XTableau;		// Destruction du tableau
}


template<class X>
bool TableauIndex<X>::add(int id, X *objet) {	// Ajoute le pos� �l�ment du tableau
													// s'il n'existe pas encore
	if(objet && 0 <= id && id < m_Max) {			// V�rifie qu'il n'y a pas d�passement du tableau
		if( !(m_XTableau[id]) ) {		// V�rifie que la case index�e par 'pos' est vide
			m_XTableau[id] = objet;
			m_Nbr++;			// Compte le nouvel �l�ment
			return true;		// L'�l�ment a �t� ajout�
		}
	}

	return false;		// L'�l�ment n'a pas �t� ajout� � cause d'un probl�me
}

template<class X>
int TableauIndex<X>::add(X *objet) {	// Ajoute l'�l�ment � la premi�re place de libre
	int id = 0;

	while(m_XTableau[id] && (id < m_Max))
		id++;

	if(id >= m_Max)			// Si aucune place n'a �t� trouv�e dans le tableau
		return -1;			// Retour sans avoir plac� le nouvel �l�ment dans le tableau
	
	m_XTableau[id] = objet;	// Place l'�l�ment � la place trouv�e
	m_Nbr++;				// Compte le nouvel �l�ment

	return id;				// Retourne l'index trouv�
}

template<class X>
bool TableauIndex<X>::Supprime(int id) {		// Supprime l'�l�ment du tableau
	if(0 <= id && id < m_Max) {		// S'il n'y a pas de d�passement
		if(_manageDeletion && m_XTableau[id]) {
			delete m_XTableau[id];
		}

		m_XTableau[id] = 0;
		m_Nbr--;			// D�compte le nouvel �l�ment
		return true;		// L'�l�ment a �t� supprim� du tableau
	}

	return false;		// Aucun �l�ment n'a pas �t� supprim�
}

template<class X>
X *TableauIndex<X>::operator[](int id)		// Retourne le pointeur sur l'�l�ment index� 'id'
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

	if(id < m_Max - 1) {	// Incr�ment d'un index
		id++;

		while((id < m_Max) && (m_XTableau[id] == 0))		// Cherche l'�l�ment apr�s la position 'id'
			id++;
	}
	else {					// Fin d'un index
		id = -1;
	}
	
	if(id >= m_Max) {
		id = -1;
	}

	return id;		// Envoie la position de l'�l�ment trouv� ou de m_Nbr pour indiquer la fin de liste
}

template<class X>
bool TableauIndex<X>::Suivant(int &id) {
	id = IndexSuivant(id);

	return id >= 0;
}

}	// JktUtils

#endif

