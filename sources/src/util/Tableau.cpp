

#ifndef __JKT__TABLEAU_CPP
#define __JKT__TABLEAU_CPP

#include <iostream>

#include "Tableau.h"

using namespace std;

namespace jkt
{
template<class X>	
Tableau<X>::Tableau()
{
	m_adrAdr = 0;		//Initialisation vide
	m_adrCurseur = 0;
	m_uConteur = 0;
}

template<class X>
Tableau<X>::Tableau(X* adrXNew)
{	
	m_adrAdr = new Adr(adrXNew);
	m_adrCurseur = m_adrAdr;	//Pointe le curseur sur le premier �l�ment Adr de Tableau
	m_uConteur = 1;				// Mets conteur d'objects � 1
}

template<class X>
Tableau<X>::~Tableau()	//D�truit tous les �l�ments Adr de la liste cha�n�e
{
	Adr *adr1, *adr2;

	int i = 0;
	adr1 = m_adrAdr->Cherche( m_adrAdr );	//Recherche le dernier bloc Adr de Tableau
	while( adr1 )
	{
		adr2 = adr1;
		adr1 = adr2->m_adrAvant;
		delete adr2;
		i++;
	}
	m_adrAdr = 0;
}

template<class X>
void Tableau<X>::Ajoute(X* adrXNew)	//Ajoute un �l�ment � la liste cha�n�e Tableau
{
	Adr* adrDernier; 
	
	m_uConteur++;	//Augmente le nombre d'�l�ments

	if(m_adrAdr == 0)
	{
		m_adrAdr = new Adr( adrXNew );
		m_adrCurseur = m_adrAdr;
	}
	else
	{	
		adrDernier = m_adrAdr->Cherche( m_adrAdr );		//Cherche dernier bloc Adr existant
														//dans Tableau
		adrDernier->m_adrApres = new Adr( adrXNew );		
		adrDernier->m_adrApres->m_adrAvant = adrDernier;//Impl�mente l'adresse du bloc pr�c�dent
														//dans le nouveau bloc
	}
}

template<class X>
void Tableau<X>::Enleve()	// Enl�ve l'�l�ment actif du tableau
{	
	if( m_uConteur )	// V�rifie si le tableau n'est pas vide
	{
		if( m_adrCurseur )
		{
			Adr *oldAdr = m_adrCurseur;

			if( m_adrAdr==m_adrCurseur ) //Si il s'agit du premier bloc Adr du tableau
			{
				if( m_adrCurseur->m_adrApres ) // Si y'a encore un �l�ment apr�s 
				{
					m_adrAdr = m_adrCurseur->m_adrApres;
					m_adrAdr->m_adrAvant = 0;
					m_adrCurseur = 0;
				}
				else	// Sinon y'a plus d'�l�ments dans le tableau
				{
					m_adrAdr = 0;
					m_adrCurseur = 0;
				}
			}
			else
			{
				if( m_adrCurseur->m_adrApres )	// Si y'a un �l�ment apr�s
					m_adrCurseur->m_adrApres->m_adrAvant = m_adrCurseur->m_adrAvant;	

				m_adrCurseur->m_adrAvant->m_adrApres = m_adrCurseur->m_adrApres;

				m_adrCurseur = m_adrCurseur->m_adrAvant;
			}

			m_uConteur--;	// Diminue le nombre d'�l�ments
			delete( oldAdr );	// Efface le bloc Adr qui a �t� enlev�
		}
		else
			cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur tableau : � d�finir 2\n";
	}
	else
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur tableau.enl�ve : tableau vide 1\n";
}

template<class X>
void Tableau<X>::Enleve(Adr *adr)	// Enl�ve l'�l�ment actif du tableau
{	
	if( m_uConteur )	// V�rifie si le tableau n'est pas vide
	{
		if( adr )
		{
			Adr *oldAdr = adr;

			if( m_adrAdr==adr ) //Si il s'agit du premier bloc Adr du tableau
			{
				if( adr->m_adrApres ) // Si y'a encore un �l�ment apr�s 
				{
					m_adrAdr = adr->m_adrApres;
					m_adrAdr->m_adrAvant = 0;
					adr = 0;
				}
				else	// Sinon y'a plus d'�l�ments dans le tableau
				{
					m_adrAdr = 0;
					adr = 0;
				}
			}
			else
			{
				if( adr->m_adrApres )	// Si y'a un �l�ment apr�s
					adr->m_adrApres->m_adrAvant = adr->m_adrAvant;	

				adr->m_adrAvant->m_adrApres = adr->m_adrApres;

				adr = adr->m_adrAvant;
			}

			m_uConteur--;	// Diminue le nombre d'�l�ments
			delete( oldAdr );	// Efface le bloc Adr qui a �t� enlev�
		}
		else
			cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur tableau : � d�finir 2\n";
	}
	else
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur tableau.enl�ve : tableau vide 1\n";
}

template<class X>
X* Tableau<X>::Get( )
{
	return m_adrAdr->Cherche( m_adrAdr )->m_adrX;
}

template<class X>
X* Tableau<X>::Get(unsigned int i)
{
	Adr* adr = m_adrAdr->Cherche(m_adrAdr, i);
	
	if(adr != 0)
		return adr->m_adrX;
	else
		return 0;
}

template<class X>
typename Tableau<X>::Adr* Tableau<X>::Suivant(typename Tableau<X>::Adr* adr)
{
	return adr->m_adrApres;	// Renvoie le bloc Adr suivant
}

template<class X>
typename Tableau<X>::Adr* Tableau<X>::BeginAdr()
{
	return m_adrAdr;		// Renvoie le premier bloc Adr du tableau
}

template<class X>
unsigned int Tableau<X>::Size()
{
	return m_uConteur;
}

}	// JktUtils

#endif

