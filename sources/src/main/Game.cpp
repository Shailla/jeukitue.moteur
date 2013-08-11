
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

class CGame;

#include "util/Erreur.h"
#include "util/V3D.h"
#include "util/Tableau.cpp"
#include "util/TableauIndex.cpp"
#include "son/DemonSons.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "main/RequeteProcess.h"
#include "spatial/Map.h"
#include "reseau/Reseau.h"
#include "util/Trace.h"
#include "main/Clavier.h"

#include "reseau/enumReseau.h"
#include "main/Game.h"

using namespace JktNet;

extern CReseau Reseau;

#define Pi 3.14159265f			//nombre pi pour les calculs

CGame::CGame() {
	m_Mode = JKT_MODE_PARTIE_NULL;

	_pTabIndexPlayer = NULL;	// Pas de liste de joueurs
	m_pMap = NULL;			// Pas de map
	m_Erwin = NULL;			// Pas de joueur actif
	m_bGravite = true;		// Par défaut la gravité est active
}

bool CGame::isModeServer() {
	return m_Mode == JKT_MODE_PARTIE_SERVER;
}

bool CGame::isModeClient() {
	return m_Mode == JKT_MODE_PARTIE_CLIENT;
}

bool CGame::isModeLocal() {
	return m_Mode == JKT_MODE_PARTIE_LOCAL;
}

bool CGame::isModeNull() {
	return m_Mode == JKT_MODE_PARTIE_NULL;
}

void CGame::setStatutClient( JktNet::StatutClient statut ) {
TRACE().p( TRACE_ERROR, "CGame::setStatusClient(status=%d) begin%T", statut, this );

	Reseau.getClient()->setStatut( statut );

TRACE().p( TRACE_ERROR, "CGame::setStatusClient() end%T", this );
}

JktNet::StatutClient CGame::getStatutClient() {
TRACE().p( TRACE_ERROR, "CGame::getStatutClient() begin%T", this );

	JktNet::StatutClient statut;
	if( m_Mode==JKT_MODE_PARTIE_CLIENT )
		statut = Reseau.getClient()->getStatut();
	else
		statut = JKT_STATUT_CLIENT_NULL;

TRACE().p( TRACE_ERROR, "CGame::getStatutClient() -> %d end%T", statut, this );
	return statut;
}

void CGame::setPlayerList(int nbr) {
	if(_pTabIndexPlayer) {
		delete _pTabIndexPlayer;
	}

	_pTabIndexPlayer = new CTableauIndex<CPlayer>(nbr);

	cout << endl << "setPlayerList";

	int curseur = -1;

	while(_pTabIndexPlayer->Suivant(curseur)) {
		CPlayer* player = _pTabIndexPlayer->operator [](curseur);

		cout << endl << "\t" << curseur << ":" << player;
	}
}

void CGame::quit() {
	if(_pTabIndexPlayer)
		delete _pTabIndexPlayer;

	_pTabIndexPlayer = 0;
}

bool CGame::openMap(const string &nomFichierMap) throw(JktUtils::CErreur) {
TRACE().p( TRACE_INFO, "CGame::openMap(nomFichierMap=%s) begin%T", nomFichierMap.c_str(), this );

	bool result = true;

	if( m_pMap )
		delete m_pMap;

	try {
		m_pMap = new JktMoteur::CMap( nomFichierMap );
	}
	catch(JktUtils::CErreur& erreur) {
		cerr << endl << __FILE__ << ":" << __LINE__ <<  " " << erreur.toString() << endl;
		result = false;
	}

TRACE().p( TRACE_INFO, "CGame::openMap() -> %b end%T", result, this );
	return result;
}

void CGame::changeActiveMap(JktMoteur::CMap* map) {
	if( m_pMap )
		delete m_pMap;

	m_pMap = map;
}

void CGame::setStatutServer(JktNet::StatutServer statut) {
	if( m_Mode==JKT_MODE_PARTIE_SERVER )
		Reseau.getServer()->setStatut( statut );
}

JktNet::StatutServer CGame::getStatutServer() {
	if( m_Mode==JKT_MODE_PARTIE_SERVER )
		return Reseau.getServer()->getStatut();
	else
		return JKT_STATUT_SERVER_NULL;
}

void CGame::Quit() {
	m_Mode = JKT_MODE_PARTIE_NULL;

	m_Erwin = 0;		// Oublie qu'il y a eu un joueur principal

	if( m_pMap ) {
		delete m_pMap;
		m_pMap = 0;
	}

	cout << "\nDestruction des joueurs begin";
	int curseur = -1;

	if(_pTabIndexPlayer) {
		while(_pTabIndexPlayer->Suivant(curseur)) {
			CPlayer* player = (*_pTabIndexPlayer)[curseur];

			if(player) {
				cout << endl << "Destruction du joueur '" << player->nom() << "'" << flush;
				delete player;
			}
		}

		cout << "\nDestruction de la liste des joueurs";
		delete _pTabIndexPlayer;	// Destruction du tableau des joueurs
		_pTabIndexPlayer = 0;
	}
	cout << "\nDestruction des joueurs end";

	m_Mode = JKT_MODE_PARTIE_NULL;	// Passe effectivemtn en mode 'aucune partie en cours'
}

void CGame::setModeNull() {
	m_Mode = JKT_MODE_PARTIE_NULL;
}

void CGame::setModeLocal() {
	m_Mode = JKT_MODE_PARTIE_LOCAL;
}

void CGame::setModeClient() {
	m_Mode = JKT_MODE_PARTIE_CLIENT;
}

void CGame::setModeServer() {
	m_Mode = JKT_MODE_PARTIE_SERVER;
}

JktNet::CClient *CGame::getClient()
{	return Reseau.getClient();		}

JktNet::CServer *CGame::getServer()
{	return Reseau.getServer();		}

void CGame::Refresh() {
		// Rafraichissement de la map
	m_pMap->Refresh( this );

		// Rafraichissement des projectils des joueurs
	CPlayer *player;
	int curseur = -1;

	while(_pTabIndexPlayer->Suivant(curseur)) {		// Pour chaque joueur
		player = _pTabIndexPlayer->operator [](curseur);

		if(player) {
			player->RefreshProjectils();
		}
	}
}

void CGame::GereContactPlayers() {	// Gère les contacts de tous les joueurs avec la map
	CPlayer *player;
	int curseur = -1;
	while(_pTabIndexPlayer->Suivant(curseur)) {		// Pour chaque joueur
		player = _pTabIndexPlayer->operator [](curseur);

		if(player) {
			player->Pente(0.0f);
			m_pMap->GereContactPlayer(player);		// Gère les contact avec la map de player
		}
	}
}

void CGame::AffichePlayers() {
	CPlayer *player;
	int curseur = -1;

	while(_pTabIndexPlayer->Suivant(curseur)) {
		player = _pTabIndexPlayer->operator [](curseur);

		if(player) {
			player->Affiche(); 				//affiche un seul joueur pour le moment
			player->AfficheProjectils(); 	// Affiche les projectils lancés par 'player'
		}
	}
}

void CGame::AfficheProjectils() {	// Affiche tous les projectils
	// Rafraichissement des projectils des joueurs
	CPlayer *player;
	int curseur = -1;

	while(_pTabIndexPlayer->Suivant(curseur)) {	// Pour chaque joueur
		player = _pTabIndexPlayer->operator [](curseur);

		if(player) {
			player->AfficheProjectils();
		}
	}
}

CPlayer *CGame::Erwin() {
	return m_Erwin;
}

void CGame::Erwin(CPlayer *erwin) {
	m_Erwin = erwin;
}

void CGame::deleteErwin() {
	if(m_Erwin)
		delete m_Erwin;
	m_Erwin = NULL;
}

JktMoteur::CMap *CGame::getMap() {
	return m_pMap;
}

void CGame::afficheToutesTextures(int x, int y, int tailleX, int tailleY) const {	// Affiche à l'écran toutes les textures de la Map
	if(m_pMap)
		m_pMap->afficheToutesTextures(x, y, tailleX, tailleY, 6, 6, 0);
}

void CGame::afficheViseur(int x, int y) const {
		// Affichage du focus sur l'arme active
	glDisable( GL_TEXTURE_2D );
	glDepthMask( GL_FALSE );
	glDisable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );

	glLineWidth( 1.5 );
	glColor3f( 1.0f, 1.0f, 1.0f );

	glPushMatrix();
	glTranslatef( (float)x, (float)y, 0 );
	glBegin( GL_LINES );
		glVertex2i( -13,	0		);
		glVertex2i( -3,		0		);

		glVertex2i( 13,		0		);
		glVertex2i( 3,		0		);

		glVertex2i( 0,		-13		);
		glVertex2i( 0,		-3		);

		glVertex2i( 0,		13		);
		glVertex2i( 0,		3		);
	glEnd();
	glPopMatrix();
}

void CGame::deplaceTousPlayer() {
	CPlayer *player;
	int curseur = -1;

	while(_pTabIndexPlayer && _pTabIndexPlayer->Suivant(curseur)) {
		player = _pTabIndexPlayer->operator [](curseur);

		if(player) {
			player->deplace();
		}
	}
}

void CGame::faitTousRequetesClavier() {
	CPlayer *player;
	int curseur = -1;

	while(_pTabIndexPlayer && _pTabIndexPlayer->Suivant(curseur)) {	//exécute les requêtes clavier sur les joueurs
		player = _pTabIndexPlayer->operator[](curseur);

		if(player) {
			player->faitRequeteClavier();
		}
	}
}

void CGame::faitTousPlayerGravite() {
	int curseur = -1;
	CPlayer *player;			//Prends le premier player

	while(_pTabIndexPlayer && _pTabIndexPlayer->Suivant(curseur)) {
		player = _pTabIndexPlayer->operator [](curseur);

		if(player) {
			player->exeActionFunc();	//exécute l'action périodique (gravité,...) du joueur
		}
	}
}

void CGame::timer() {
	if( m_Erwin ) {
		if( m_bGravite )			// Si la gravité est active
			faitTousPlayerGravite();

		Refresh();		// Rafraichi les classes qui ont besoin de l'être (celles de type CMouve et CProjectil)

		faitTousRequetesClavier();		// Exécute les requêtes clavier sur tous les joueurs
	}

		// Teste les contacts des joueurs avec les parois
	if( m_pMap )
		GereContactPlayers();	// Gère tous les contacts entre la map et les joueurs

	deplaceTousPlayer();	//déplace tous les joueurs de leurs vitesses
}

void CGame::Gravite(bool gravite) {
	m_bGravite = gravite;
}

bool CGame::Gravite() const {
	return m_bGravite;
}
