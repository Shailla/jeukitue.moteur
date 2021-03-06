#pragma warning( disable : 4290 )	// Evite les warnings sur les d�claration de throw

#ifndef __JKT__GAME_H
#define __JKT__GAME_H

#include "util/Erreur.h"
#include "util/TableauIndex.h"
#include "main/RequeteProcess.h"
#include "reseau/enumReseau.h"

#include "data/ServeurDataTree.h"
#include "data/ClientDataTree.h"

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

	// Cette classe est destin�e � contenir toutes les param�tres vivants, c'est � dire
	// toutes les donn�es et variables de la partie en cours, du r�seau...

class CGame {
public:
	enum ModePartie {
		JKT_MODE_PARTIE_NULL,		// Pas de jeu en cours
		JKT_MODE_PARTIE_LOCAL,		// Mode de jeu local (c'est � dire pas en r�seau)
		JKT_MODE_PARTIE_CLIENT,		// Mode client connect� � un serveur
		JKT_MODE_PARTIE_SERVER,		// Mode serveur de jeu
	};

private:
	CPlayer* _erwin;				// Pointeur sur le joueur actif
	JktMoteur::CMap	*_map;			// Map en cours de jeu
	ModePartie _mode;				// Mode de jeu (parie normale, client ou serveur)
	bool _gravite;					// Indique si la gravit� est active

public:
	JktUtils::CTableauIndex<CPlayer> *_pTabIndexPlayer;	// Liste index�e des joueurs

	ServeurDataTree* _serverDataTree;
	ClientDataTree* _clientDataTree;

	CRequeteProcess RequeteProcess;		// Requetes inter-processus

		// FONCTIONS MEMBRES
	// Constructeurs/destructeur
	CGame();

	// G�n�ralit�s
	bool isModeNull();		// Indique si aucune partie n'est en cours
	bool isModeLocal();		// Indique si une partie locale est en cours
	bool isModeClient();	// Indique si une partie r�seau client est en cours
	bool isModeServer();	// Indique si une partie serveur est en cours
	void Quit();			// Passe en mode aucun jeu en cours
	bool Gravite() const;	// Indique si la gravit� est active
	void Gravite(bool gravite);

	// Gestion de la Map
	JktMoteur::CMap *getMap();
	void changeActiveMap(JktMoteur::CMap* map);
	bool openMap( const string &nomFichierMap ) throw(JktUtils::CErreur);

	// Gestion du mode
	void setModeNull();					// Passe en mode "pas de jeu en cours"

	// Gestion du mode local
	void setModeLocal();					// Passe en mode de jeu local

	// Gestion du client
	void setModeClient();									// Cr�e la classe Client pour un jeu en mode client
	JktNet::CClient *getClient();							// Retourne le pointeur sur la classe CClient
	void setStatutClient( JktNet::StatutClient statut );	// Renseigne le statut du client
	JktNet::StatutClient getStatutClient();					// Donne le statut du client

	// Gestion du serveur
	void setModeServer();									// Cr�e la classe Server pour un jeu en mode serveur
	JktNet::CServer *getServer();							// Retourne le pointeur sur la classe CServer
	void setStatutServer( JktNet::StatutServer statut );	// Renseigne le statut du serveur
	JktNet::StatutServer getStatutServer();					// Donne le statut du serveur

	void setServerDataTree(ServeurDataTree* serverDataTree);
	ServeurDataTree* getServerDataTree();

	void setClientDataTree(ClientDataTree* clientDataTree);
	ClientDataTree* getClientDataTree();

	// Gestion du jeu
	void AffichePlayers();			// Affiche tous les joueurs
	void AfficheProjectils();		// Affiche tous les projectils

	void setPlayerList(int nbr);	// Indique le nombre de joueurs de la partie
	int getMaxPlayers() const;

	// Gestion du joueur principal (actif)
	CPlayer* Erwin();								// Renvoie le joueur principal
	void Erwin(CPlayer *erwin);					// Set le joueur principal

	// Gestion des joueurs
	void deletePlayers();								// D�truit tous les joueurs

private:
	void Refresh();					// Rafraichi les classe qui en ont besoin
	void deplaceTousPlayer();
	void faitTousRequetesClavier();
	void faitTousPlayerGravite();
	void GereContactPlayers();		// G�re les contacts entre tous les joueurs et la map

public:
	void quitCurrentMap();

	// Fonctions d'affichage
	void afficheToutesTextures(int x, int y, int tailleX, int tailleY) const;
	void afficheViseur(int x, int y) const;			// Affiche le joueur � la position voulue sur l'�cran

	void timer();
};

#endif

