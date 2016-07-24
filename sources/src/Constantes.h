#ifndef __JKT__CONSTANTES_H
#define __JKT__CONSTANTES_H

const float Pi = 3.14159265f;									// Nombre pi pour les calculs
const float QUANTUM_VITESSE_PLAYER = 0.003f;					// Pas d'augmentation de la vitesse du joueur (accélération discrête)
const float MAX_VITESSE_PLAYER = 20.0f*QUANTUM_VITESSE_PLAYER;	// Vitesse maximale d'un joueur (en nombre de QUANTUM_VITESSE_PLAYER)
const int DELAY_TIMER = 10;										// Temps entre 2 calculs de scènes (en ms)
const float TAILLEFONT = 0.7f;									// Taille des fontes
const float DISTANCE_INTER_JOUEURS_ENTRY_POINT = 5.0f;

#endif	// __JKT__CONSTANTES_H
