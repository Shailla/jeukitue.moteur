/*
 * UdpInterlocutor.cpp
 *
 *  Created on: 18 juin 2013
 *      Author: vgdj7997
 */

#include "reseau/new/UdpInterlocutor.h"

UdpInterlocutor::UdpInterlocutor() {
}

UdpInterlocutor::~UdpInterlocutor() {
}

void UdpInterlocutor::connect() {
	// Obtenir port de réponse UDP
	// Envoyer au distant demande connexion et fournir port de réponse
	// Obtenir acceptation connexion de la part du client

	setConnexionStatus(CONNEXION_STATUS::CONNECTED);
}

void UdpInterlocutor::close() {
	// Envoyer message déconnexion au distant
	// Passer statut TechnicalInterlocutor à Déconnecté
	// Libérer les ressources (sockets, ...)
}

