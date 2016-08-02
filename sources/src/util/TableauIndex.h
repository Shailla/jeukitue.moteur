
#ifndef __JKT__TABLEAUINDEX_H
#define __JKT__TABLEAUINDEX_H

namespace jkt
{

template <class X> class CTableauIndex
{
private:
	int m_Max;		// Nombre de places dans ce tableau
	int m_Nbr;		// Nombre d'éléments actuellement présents dans le tableau
	X** m_XTableau;	// Tableau des objets

		// Constructeurs/destructeur
public:
	CTableauIndex(int nbr);			// Crée un tableau indexé pouvant contenir nbr objets
	~CTableauIndex();				// Détruit le tableau (ne détruis pas les éléments par contre)

	int getMax();					// Retourne le nombre de places disponibles dans le tableau
	int getNbr();					// Retourne le nombre d'éléments actuellement présents dans le tableau
	bool Ajoute(int pos, X *objet);	// Ajoute un élément à la pos° position du tableau
	int Ajoute(X *objet);			// Ajoute l'élément à la première place de libre
	bool Supprime(int pos);			// Supprime du tableau le pos° élément
	X* operator[](int pos);			// Retourne un pointeur sur l'élément indexé 'pos'

	/**
	 * Renvoie l'index qui atteint l'élément suivant.
	 *
	 * @param pos index à mettre à jour
	 * @return index de l'élément suivant ou -1 si la fin du tableau a été atteinte
	 */
	int IndexSuivant(int pos);		// Renvoie l'index de l'élément après l'élément indexé par 'pos'

	/**
	 * Met à jour l'index pour qu'il atteigne l'élément suivant.
	 *
	 * @param pos index à mettre à jour. -1 en retour signifie que la fin du tableau a été atteinte
	 * @return true si la fin du tableau a été atteinte, false sinon
	 */
	bool Suivant(int &pos);
};

}	// JktUtils

#endif

