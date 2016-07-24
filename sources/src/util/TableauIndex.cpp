

#ifndef __JKT__TABLEAUINDEX_CPP
#define __JKT__TABLEAUINDEX_CPP

#include <iostream>

#include "TableauIndex.h"

namespace jkt
{

template<class X>
CTableauIndex<X>::CTableauIndex(int nbr) {
	m_Max = nbr;					// Nombre d'objets dans le tableau
	m_Nbr = 0;						// Aucun �l�ment dans les cases du tableau
	m_XTableau = new X*[ nbr ];		// Cr�ation du tableau

	for( int i=0 ; i<nbr ; i++ )	// Mise � z�ro des �l�ments du tableau
		m_XTableau[i] = NULL;
}

template<class X>
CTableauIndex<X>::~CTableauIndex() {
	delete m_XTableau;		// Destruction du tableau
}


template<class X>
bool CTableauIndex<X>::Ajoute(int pos, X *objet)	// Ajoute le pos� �l�ment du tableau
{													// s'il n'existe pas encore
	if(objet && pos<m_Max) {		// V�rifie qu'il n'y a pas d�passement du tableau
		if( !(m_XTableau[pos]) ) {		// V�rifie que la case index�e par 'pos' est vide
			m_XTableau[pos] = objet;
			m_Nbr++;			// Compte le nouvel �l�ment
			return true;		// L'�l�ment a �t� ajout�
		}
	}

	return false;		// L'�l�ment n'a pas �t� ajout� � cause d'un probl�me
}

template<class X>
int CTableauIndex<X>::Ajoute(X *objet) {	// Ajoute l'�l�ment � la premi�re place de libre
	int i = 0;

	while( m_XTableau[i] && (i<m_Max) )
		i++;

	if( i>=m_Max )			// Si aucune place n'a �t� trouv�e dans le tableau
		return -1;			// Retour sans avoir plac� le nouvel �l�ment dans le tableau
	
	m_XTableau[i] = objet;	// Place l'�l�ment � la place trouv�e
	m_Nbr++;				// Compte le nouvel �l�ment

	return i;				// Retourne l'index trouv�
}

template<class X>
bool CTableauIndex<X>::Supprime( int pos )		// Supprime le pos� �l�ment du tableau
{
	if( pos < m_Max )		// S'il n'y a pas de d�passement
	{
		m_XTableau[ pos ] = 0;
		m_Nbr--;			// D�compte le nouvel �l�ment
		return true;		// L'�l�ment a �t� supprim� du tableau
	}

	return false;					// L�l�ment n'a pas �t� supprim�
}

template<class X>
X *CTableauIndex<X>::operator[]( int pos )		// Retourne le pointeur sur l'�l�ment index� 'pos'
{
#ifdef _DEBUG
	if( pos > m_Max )
		std::cerr << std::endl << __FILE__ << ":" << __LINE__ << "Erreur : acces 'CTableauIndex' dors limite";
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
		pos = -1;
	}

	if(pos < m_Max - 1) {	// Incr�ment d'un index
		pos++;

		while((pos<m_Max) && (m_XTableau[pos]==NULL))		// Cherche l'�l�ment apr�s la position 'pos'
			pos++;
	}
	else {					// Fin d'un index
		pos = -1;
	}
	
	if(pos >= m_Max) {
		pos = -1;
	}

	return pos;		// Envoie la position de l'�l�ment trouv� ou de m_Nbr pour indiquer la fin de liste
}

template<class X>
bool CTableauIndex<X>::Suivant(int &pos) {
	pos = IndexSuivant(pos);

	return pos >= 0;
}

}	// JktUtils

#endif

