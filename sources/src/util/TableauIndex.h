
#ifndef __JKT__TABLEAUINDEX_H
#define __JKT__TABLEAUINDEX_H

namespace jkt
{

template <class X> class TableauIndex
{
private:
	int m_Max;				// Nombre de places dans ce tableau
	int m_Nbr;				// Nombre d'éléments actuellement présents dans le tableau
	bool _manageDeletion;	// Indique si le tableau doit exécuter un delete lors des suppression de ses éléments
	X** m_XTableau;			// Tableau des objets

	// Constructeurs/destructeur
public:
	TableauIndex(bool manageDeletion);			// Crée un tableau indexé pouvant contenir nbr objets
	TableauIndex(int nbr, bool manageDeletion);	// Crée un tableau indexé pouvant contenir nbr objets
	~TableauIndex();							// Détruit le tableau (ne détruis pas les éléments par contre)

	void clear();								// Réinitialise le tableau avec une nouvelle nulle, en fonction de '_manageDeletion' les éléments pointés sont supprimés
	void reinit(int nbr);						// Réinitialise le tableau avec une nouvelle taille, en fonction de '_manageDeletion' les éléments pointés sont supprimés

	int getMax() const;							// Retourne le nombre de places disponibles dans le tableau
	int getNbr() const;							// Retourne le nombre d'éléments actuellement présents dans le tableau
	bool add(int id, X* objet);					// Ajoute un élément avec l'identifient demandé
	int add(X *objet);							// Ajoute l'élément à la première place de libre
	bool Supprime(int id);						// Supprime du tableau l'élément identifié, en fonction de '_manageDeletion' l'élément pointé est supprimé
	X* operator[](int id);						// Retourne un pointeur sur l'élément indexé 'pos'

	/**
	 * Renvoie l'index qui atteint l'élément suivant.
	 *
	 * @param id index à mettre à jour
	 * @return index de l'élément suivant ou -1 si la fin du tableau a été atteinte
	 */
	int IndexSuivant(int id);		// Renvoie l'index de l'élément après l'élément indexé par 'pos'

	/**
	 * Met à jour l'index pour qu'il atteigne l'élément suivant.
	 *
	 * @param id index à mettre à jour. -1 en retour signifie que la fin du tableau a été atteinte
	 * @return true si la fin du tableau a été atteinte, false sinon
	 */
	bool Suivant(int &id);
};

}	// JktUtils

#endif

