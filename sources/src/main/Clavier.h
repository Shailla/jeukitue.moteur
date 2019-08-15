
#ifndef __CLAVIER_H
#define __CLAVIER_H

class CClavier {
public :
	bool m_bIndic;		// Indique si une requête de déplacement clavier existe pour le prochain affichage

	float m_fAvance;	// Nombre sans unité, exemple 1.0fIndique si le joueur veut avancer
	float m_fDroite;	// Nombre sans unité, exemple 1.0fIndique si le joueur veut aller vers la droite
	float m_fMonte;		// Nombre sans unité, exemple 1.0fIndique si le joueur veut monter

		// CONSTRUCTEURS
	CClavier();
	
	void reset();
};

#endif


