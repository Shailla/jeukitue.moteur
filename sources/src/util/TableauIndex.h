
#ifndef __JKT__TABLEAUINDEX_H
#define __JKT__TABLEAUINDEX_H

namespace jkt
{

template <class X> class CTableauIndex
{
private:
	int m_Max;		// Nombre de places dans ce tableau
	int m_Nbr;		// Nombre d'�l�ments actuellement pr�sents dans le tableau
	X** m_XTableau;	// Tableau des objets

		// Constructeurs/destructeur
public:
	CTableauIndex(int nbr);			// Cr�e un tableau index� pouvant contenir nbr objets
	~CTableauIndex();				// D�truit le tableau (ne d�truis pas les �l�ments par contre)

	int getMax();					// Retourne le nombre de places disponibles dans le tableau
	int getNbr();					// Retourne le nombre d'�l�ments actuellement pr�sents dans le tableau
	bool Ajoute(int pos, X *objet);	// Ajoute un �l�ment � la pos� position du tableau
	int Ajoute(X *objet);			// Ajoute l'�l�ment � la premi�re place de libre
	bool Supprime(int pos);			// Supprime du tableau le pos� �l�ment
	X* operator[](int pos);			// Retourne un pointeur sur l'�l�ment index� 'pos'

	/**
	 * Renvoie l'index qui atteint l'�l�ment suivant.
	 *
	 * @param pos index � mettre � jour
	 * @return index de l'�l�ment suivant ou -1 si la fin du tableau a �t� atteinte
	 */
	int IndexSuivant(int pos);		// Renvoie l'index de l'�l�ment apr�s l'�l�ment index� par 'pos'

	/**
	 * Met � jour l'index pour qu'il atteigne l'�l�ment suivant.
	 *
	 * @param pos index � mettre � jour. -1 en retour signifie que la fin du tableau a �t� atteinte
	 * @return true si la fin du tableau a �t� atteinte, false sinon
	 */
	bool Suivant(int &pos);
};

}	// JktUtils

#endif

