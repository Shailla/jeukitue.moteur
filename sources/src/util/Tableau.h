
#ifndef __JKT__TABLEAU_H
#define __JKT__TABLEAU_H

//////////////////////////////////////////////////////////////////////////////////////
//Cette classe est une liste chaînée pouvant contenir n'importe quelle classe		//
//puisqu'elle est écrite en template												//
//Il y a des fonctions de rajout d'élément et d'extraction du iième					//
//ou du dernier élément																//
//////////////////////////////////////////////////////////////////////////////////////

namespace jkt
{

template <class X> class Tableau {
public:
	template <class U> class TAdr {
	public:
		U* m_adrX;		//Adresse de la classe X associée à ce bloc d'adresse
		TAdr* m_adrAvant;	//Adresse du bloc d'adresse contenant la classe X précédente
		TAdr* m_adrApres;	//Adresse du bloc d'adresse contenant la classe X suivante (0 si rien après)

			//CONSTRUCTEURS / DESTRUCTEUR
		TAdr()			//Initialisation d'un bloc vide
		{
			m_adrApres = 0;
			m_adrAvant = 0;
			m_adrX = 0;
		};

		TAdr(U* adrXNew)
		{
			m_adrApres = 0;
			m_adrAvant = 0;
			m_adrX = adrXNew;
		};

		TAdr* Cherche(TAdr* adr) //Recherche le dernier bloc Adr de la liste chaînée
		{
			if( adr )
			{
				if( adr->m_adrApres )
				{
					adr = adr->m_adrApres->Cherche( m_adrApres );
				}
			}

			return adr;
		}

		TAdr* Cherche(TAdr* adr, unsigned int i)	//Recherche le ième bloc Adr de la liste chaînée
		{										//à partir de adr
			if( i>1 )
			{
				if(adr->m_adrApres != 0)
					adr = adr->m_adrApres->Cherche(m_adrApres,i-1);
				else
					adr = 0;
			}

			return adr;
		}
	};

public:
	typedef TAdr<X> Adr;

	Adr *m_adrAdr;				// Adresse du premier bloc Adr
	Adr *m_adrCurseur;			// Position du curseur courant sur bloc Adr
	unsigned int m_uConteur;		// Nombre d'objets contenus dans tableau

		//Constructeurs / Destructeur
	Tableau();
	Tableau(X* adrX);
	~Tableau();

		//Fonctions membres
	void Ajoute(X* adrXNew);		// Ajoute un élément au tableau
	void Enleve();					// Supprime l'élément pointé par le curseur
	void Enleve(Adr *adr);		// Supprime l'élément demandé
	X* Get();						// Retourne l'adresse du dernier objet X de Tableau
	X* Get(unsigned int i);			// Retourne l'adresse du iième objet X de Tableau
	Adr* Suivant(Adr *adr);	// Renvoie le bloc Adr après adr, 0 sinon
	Adr* BeginAdr();				// Renvoie le premier bloc Adr
	unsigned int Size();			// Retourne le nombre d'objets contenus
};

}	// JktUtils

#endif

