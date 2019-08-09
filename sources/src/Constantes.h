#ifndef __JKT__CONSTANTES_H
#define __JKT__CONSTANTES_H

const float Pi = 3.14159265f;									// Nombre pi pour les calculs
const float QUANTUM_VITESSE_PLAYER = 0.003f;					// Pas d'augmentation de la vitesse du joueur (accélération discrète)

const float GRAVITY_ACCELERATION = 9.81f;						// Accélération de la gravité en m/s^2
const float PLAYER_ACCELERATION = 0.7f;							// Accélération d'un joueur qui court en m/s^2
const float MAX_SPEED_PLAYER_ON_GROUND = 10.0f;					// Vitesse maximale d'un joueur avec les pieds au sol, en m/s
const float MAX_SPEED_PLAYER = 100.0f;							// Vitesse maximale d'un joueur avec les pieds au sol, en m/s
const float PROJECTIL_LAUNCH_SPEED = 10.0f;						// Vitesse à laquelle un projectil est lancé par un joueur

const int DELAY_TIMER = 10;										// Temps entre 2 calculs de scènes (en ms)
const float TAILLEFONT = 0.7f;									// Taille des fontes
const float DISTANCE_INTER_JOUEURS_ENTRY_POINT = 5.0f;

#endif	// __JKT__CONSTANTES_H
