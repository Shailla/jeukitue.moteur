

#ifndef __JKT__TABLEAUINDEX_CPP
#define __JKT__TABLEAUINDEX_CPP

#include <iostream>

#include "TableauIndex.h"

namespace JktUtils
{

template<class X>
CTableauIndex<X>::CTableauIndex(int nbr) {
	m_Max = nbr;					// Nombre d'objets dans le tableau
	m_Nbr = 0;						// Aucun élément dans les cases du tableau
	m_XTableau = new X*[ nbr ];		// Création du tableau

	for( int i=0 ; i<nbr ; i++ )	// Mise à zéro des éléments du tableau
		m_XTableau[i] = NULL;
}

template<class X>
CTableauIndex<X>::~CTableauIndex() {
	delete m_XTableau;		// Destruction du tableau
}


template<class X>
bool CTableauIndex<X>::Ajoute(int pos, X *objet)	// Ajoute le pos° élément du tableau
{													// s'il n'existe pas encore
	if(pos<m_Max) {		// Vérifie qu'il n'y a pas dépassement du tableau
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
	int i = 0;

	while( m_XTableau[i] && (i<m_Max) )
		i++;

	if( i>=m_Max )			// Si aucune place n'a été trouvée dans le tableau
		return -1;			// Retour sans avoir placé le nouvel élément dans le tableau
	
	m_XTableau[i] = objet;	// Place l'élément à la place trouvée
	m_Nbr++;				// Compte le nouvel élément

	return i;				// Retourne l'index trouvé
}

template<class X>
bool CTableauIndex<X>::Supprime( int pos )		// Supprime le pos° élément du tableau
{
	if( pos < m_Max )		// S'il n'y a pas de dépassement
	{
		m_XTableau[ pos ] = 0;
		m_Nbr--;			// Décompte le nouvel élément
		return true;		// L'élément a été supprimé du tableau
	}

	return false;					// Lélément n'a pas été supprimé
}

template<class X>
X *CTableauIndex<X>::operator[]( int pos )		// Retourne le pointeur sur l'élément indexé 'pos'
{
#ifdef _DEBUG
	if( pos > m_Max )
		std::cerr << std::endl << "Erreur : acces 'CTableauIndex' dors limite";
#endif
	return m_XTableau[pos];
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
int CTableauIndex<X>::IndexSuivant(int pos) {
	if(pos < 0) {			// Initialisation d'un index
		pos = 0;
	}
	else if(pos < m_Max - 1) {	// Incrément d'un index
		pos++;

		while((pos<m_Max) && (m_XTableau[pos]==NULL))		// Cherche l'élément après la position 'pos'
			pos++;
	}
	else {					// Fin d'un index
		pos = -1;
	}
	
	if(pos >= m_Max) {
		pos = -1;
	}

	return pos;		// Envoie la position de l'élément trouvé ou de m_Nbr pour indiquer la fin de liste
}

template<class X>
bool CTableauIndex<X>::Suivant(int &pos) {
	pos = IndexSuivant(pos);

	return pos >= 0;
}

}	// JktUtils

#endif

