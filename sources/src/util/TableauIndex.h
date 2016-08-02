
#ifndef __JKT__TABLEAUINDEX_H
#define __JKT__TABLEAUINDEX_H

namespace jkt
{

template <class X> class TableauIndex
{
private:
	int m_Max;				// Nombre de places dans ce tableau
	int m_Nbr;				// Nombre d'�l�ments actuellement pr�sents dans le tableau
	bool _manageDeletion;	// Indique si le tableau doit ex�cuter un delete lors des suppression de ses �l�ments
	X** m_XTableau;			// Tableau des objets

	// Constructeurs/destructeur
public:
	TableauIndex(bool manageDeletion);			// Cr�e un tableau index� pouvant contenir nbr objets
	TableauIndex(int nbr, bool manageDeletion);	// Cr�e un tableau index� pouvant contenir nbr objets
	~TableauIndex();							// D�truit le tableau (ne d�truis pas les �l�ments par contre)

	void clear();								// R�initialise le tableau avec une nouvelle nulle, en fonction de '_manageDeletion' les �l�ments point�s sont supprim�s
	void reinit(int nbr);						// R�initialise le tableau avec une nouvelle taille, en fonction de '_manageDeletion' les �l�ments point�s sont supprim�s

	int getMax() const;							// Retourne le nombre de places disponibles dans le tableau
	int getNbr() const;							// Retourne le nombre d'�l�ments actuellement pr�sents dans le tableau
	bool add(int id, X* objet);					// Ajoute un �l�ment avec l'identifient demand�
	int add(X *objet);							// Ajoute l'�l�ment � la premi�re place de libre
	bool Supprime(int id);						// Supprime du tableau l'�l�ment identifi�, en fonction de '_manageDeletion' l'�l�ment point� est supprim�
	X* operator[](int id);						// Retourne un pointeur sur l'�l�ment index� 'pos'

	/**
	 * Renvoie l'index qui atteint l'�l�ment suivant.
	 *
	 * @param id index � mettre � jour
	 * @return index de l'�l�ment suivant ou -1 si la fin du tableau a �t� atteinte
	 */
	int IndexSuivant(int id);		// Renvoie l'index de l'�l�ment apr�s l'�l�ment index� par 'pos'

	/**
	 * Met � jour l'index pour qu'il atteigne l'�l�ment suivant.
	 *
	 * @param id index � mettre � jour. -1 en retour signifie que la fin du tableau a �t� atteinte
	 * @return true si la fin du tableau a �t� atteinte, false sinon
	 */
	bool Suivant(int &id);
};

}	// JktUtils

#endif

