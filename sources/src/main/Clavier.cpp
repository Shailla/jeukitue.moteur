
#include "main/Clavier.h"

CClavier::CClavier() {
	reset();
}

void CClavier::reset() {
	m_fAvance = 0.0;
	m_fDroite = 0.0;
	m_fMonte = 0.0;
	m_bIndic = false; 	// Ppas de déplacement demandé par défaut
}
