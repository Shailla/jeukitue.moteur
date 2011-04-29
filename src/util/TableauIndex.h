
#ifndef __JKT__TABLEAUINDEX_H
#define __JKT__TABLEAUINDEX_H

namespace JktUtils
{

template <class X> class CTableauIndex
{
private:
	int m_Max;		// Nombre de places dans ce tableau
	int m_Nbr;		// Nombre d'éléments actuellement présents dans le tableau
	X** m_XTableau;	// Tableau des objets
	bool m_bParent;	// Indique si les éléments du tableau doivent être détruits en même temps que lui

		// Constructeurs/destructeur
public:
	CTableauIndex(int nbr, bool bParent);	// Crée un tableau indexé pouvant contenir nbr objets
	~CTableauIndex();				// Détruit le tableau (ne détruis pas les éléments par contre)

	int getMax();					// Retourne le nombre de places disponibles dans le tableau
	int getNbr();					// Retourne le nombre d'éléments actuellement présents dans le tableau
	bool Ajoute(int pos,X *objet);	// Ajoute un élément à la pos° position du tableau
	int Ajoute( X *objet );			// Ajoute l'élément à la première place de libre
	bool Supprime(int pos);			// Supprime du tableau le pos° élément
	X* operator[](int pos);			// Retourne un pointeur sur l'élément indexé 'pos'
	int Suivant( int pos );			// Renvoie l'index de l'élément après l'élément indexé par 'pos'
	bool bSuivant( int &pos );		// Comme Suivant + document pos + vérifie si dernier élément
};

}	// JktUtils

#endif

