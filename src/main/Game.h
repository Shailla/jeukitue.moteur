
#pragma warning( disable : 4290 )	// Evite les warnings sur les déclaration de throw

#ifndef __JKT__GAME_H
#define __JKT__GAME_H

#include "util/Erreur.h"
#include "util/TableauIndex.h"
#include "main/RequeteProcess.h"
#include "reseau/enumReseau.h"

namespace JktNet
{
	class CClient ;
	class CServer;
}

namespace  JktMoteur
{
	class CMap;
}

class CPlayer;

	// Cette classe est destinée à contenir toutes les paramètres vivants, c'est à dire
	// toutes les données et variables de la partie en cours, du réseau...

class CGame
{
	CPlayer *m_Erwin;							// Pointeur sur le joueur actif
	JktMoteur::CMap	*m_pMap;		// Map en cours de jeu
	int m_Mode;									// Mode de jeu (parie normale, client ou serveur
	bool m_bGravite;							// Indique si la gravité est active

public:
	enum modePartie {
		JKT_MODE_PARTIE_NULL,		// Pas de jeu en cours
		JKT_MODE_PARTIE_LOCAL,		// Mode de jeu local (c'est à dire pas en réseau)
		JKT_MODE_PARTIE_CLIENT,		// Mode client connecté à un serveur
		JKT_MODE_PARTIE_SERVER,		// Mode serveur de jeu
	};

	JktUtils::CTableauIndex<CPlayer> *pTabIndexPlayer;	// Liste indexée des joueurs

	CRequeteProcess RequeteProcess;		// Requetes inter-processus

			// FONCTIONS MEMBRES
		// Constructeurs/destructeur
	CGame();

		// Généralités
	bool isModeNull();		// Indique si aucune partie n'est en cours
	bool isModeLocal();		// Indique si une partie locale est en cours
	bool isModeClient();	// Indique si une partie réseau client est en cours
	bool isModeServer();	// Indique si une partie serveur est en cours
	void Quit();			// Passe en mode aucun jeu en cours
	bool Gravite() const;	// Indique si la gravité est active
	void Gravite(bool gravite);

		// Gestion de la Map
	JktMoteur::CMap *getMap();
	bool openMap( const string &nomFichierMap ) throw(JktUtils::CErreur);

		// Gestion du mode
	void setModeNull();					// Passe en mode "pas de jeu en cours"

		// Gestion du mode local
	void setModeLocal();					// Passe en mode de jeu local

		// Gestion du client
	void setModeClient();							// Crée la classe Client pour un jeu en mode client
	JktNet::CClient *getClient();		// Retourne le pointeur sur la classe CClient
	void setStatutClient( JktNet::StatutClient statut );	// Renseigne le statut du client
	JktNet::StatutClient getStatutClient();							// Donne le statut du client

		// Gestion du serveur
	void setModeServer();							// Crée la classe Server pour un jeu en mode serveur
	JktNet::CServer *getServer();		// Retourne le pointeur sur la classe CServer
	void setStatutServer( JktNet::StatutServer statut );				// Renseigne le statut du serveur
	JktNet::StatutServer getStatutServer();							// Donne le statut du serveur

		// Gestion du jeu
	void AffichePlayers();			// Affiche tous les joueurs
	void AfficheProjectils();		// Affiche tous les projectils
	void Refresh();					// Rafraichi les classe qui en ont besoin
	void GereContactPlayers();		// Gère les contacts entre tous les joueurs et la map
	void setPlayerList(int nbr);	// Indique le nombre de joueurs de la partie
	void quit();					// Quitte la partie en cours

		// Gestion du joueur principal (actif)
	CPlayer *Erwin();								// Revoie le joueur principal
	void Erwin( CPlayer *erwin );					// Set le joueur principal
	void deleteErwin();								// Détruit le joueur principal s'il existe

	void afficheToutesTextures(int x, int y) const;
	void afficheViseur(int x, int y) const;			// Affiche le joueur à la position voulue sur l'écran
	void deplaceTousPlayer();
	void faitTousRequetesClavier();
	void faitTousPlayerGravite();
	void timer();
};

#endif

