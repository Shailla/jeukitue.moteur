

#ifndef __JKT__CFG_H
#define __JKT__CFG_H

#define VERSION_DAY		31
#define VERSION_MONTH	10
#define VERSION_YEAR	2004

#include <string>
#include <vector>
#ifdef WIN32
	#include <windows.h>
#endif

#include "SDL.h"
#include "GL/gl.h"
#include "GL/glext.h"

using namespace std;

class CCfg {
	static const char* CST_GEN_PLAY_INTRO;

	static const char* CST_VID_DISPLAY_POS;
	static const char* CST_VID_DISPLAY_SIZE;
	static const char* CST_VID_FULLSCREEN;

	static const char* CST_AUD_OUTPUT;
	static const char* CST_AUD_MIXER;
	static const char* CST_AUD_DRIVER;
	static const char* CST_AUD_DRIVERRECORD;

	static const char* CST_COM_AVANCER;
	static const char* CST_COM_RECULER;
	static const char* CST_COM_GAUCHE;
	static const char* CST_COM_DROITE;
	static const char* CST_COM_TIR1;
	static const char* CST_COM_TIR2;
	static const char* CST_COM_MONTER;
	static const char* CST_COM_SELECTWEAPONUP;
	static const char* CST_COM_SELECTWEAPONDOWN;

	static const char* CST_CEN_IP;
	static const char* CST_CEN_PORT;

	static const char* CST_NET_SERVEUR;
	static const char* CST_NET_IP;
	static const char* CST_NET_PORT;
	static const char* CST_NET_PORTTREE;
	static const char* CST_TREE_UPDATE_CLIENTTOSERVER_DELAY;
	static const char* CST_TREE_UPDATE_SERVERTOCLIENT_DELAY;


	static const char* CST_JOU_NOM;
	static const char* CST_JOU_MAPNOM;
	static const char* CST_JOU_OUTLINEVISIBILITY;
	static const char* CST_JOU_SKINVISIBILITY;

	static const char* CST_PLU_ACT_BEGIN;
	static const char* CST_PLU_ACT_END;

	static const char* CST_WEB_HTML_PORT;

	static const char* CST_DEB_SONPERFORMANCES;
	static const char* CST_DEB_SONSPECTRE;
	static const char* CST_DEB_AFFICHEFICHIER;
	static const char* CST_DEB_AFFICHENORMAUX;


	class CGeneral {
	public :
		bool _playIntro;
	};

public:
	class CAudio {
	public :
		static const int AVAILABLE_AUDIO_OUTPUTS_NBR;
		static const char *AVAILABLE_AUDIO_OUTPUTS[];

		int m_Output;
		int m_Mixer;
		int m_Driver;
		int m_DriverRecord;

		void Init();
		bool testInitAndSaveConfiguration(int driver, int output, int mixer, int driverRecord);
	};

	class CComID {
	public:
		SDLKey key;		// Touche du clavier correspondant
		int mouse;		// Touche de la souris correspondant
	};
private:

	class CCommandes {
	public:
		CComID Avancer;				// Avancer
		CComID Reculer;				// Reculer
		CComID Gauche;				// Aller à gauche
		CComID Droite;				// Aller à droite
		CComID Tir1;				// Tir primaire
		CComID Tir2;				// Tir secondaire
		CComID Monter;				// Monter
		CComID SelectWeaponUp;		// Sélectionner arme suivante
		CComID SelectWeaponDown;	// Sélectionner arme précédente
		const char* resolve(CComID com);					// Retourne la touche ou le bouton en texte
	};

	class CDisplay {
		friend class CCfg;
		bool m_bFullscreen;
		void InitSDL();
		void InitOpenGL();
        void InitAgar();
	public :
		string position;
		int X;
		int Y;
		bool Fullscreen() const		{	return m_bFullscreen;	}
		void Fullscreen(bool on)	{	m_bFullscreen = on;		}
		void Init();
		void ChangeVideoSize(int x, int y);
	};

	class CCentralisateur {
		friend class CCfg;
	protected:
		Uint16 m_Port;				// Port à utiliser
		string m_IpServer;			// Adresse IP du serveur (s'il s'agit d'un client)
	public:
		Uint16 getPort() const			{ return m_Port; }
		void setPort( Uint16 port )		{ m_Port = port; }
		string getIp() const		{ return m_IpServer; }
		void setIp( string ip )		{ m_IpServer = ip; }
	};

	class CReseau {		// Informations sur la config réseau
		friend class CCfg;
	protected:
		Uint16 _Port;						// Port à utiliser
		Uint16 _portTree;					// Port à utiliser
		string _IpServer;					// Adresse IP du serveur (s'il s'agit d'un client)
		int _treeUpdateClientToServerDelay;	// Temps mini entre 2 mises à jour des données de l'arbre par le client sur le serveur
		int _treeUpdateServerToClientDelay;	// Temps mini entre 2 mises à jour des données de l'arbre par le serveur sur le client
	public:
		string getIpServer() const								{ return _IpServer;							}
		Uint16 getServerPort() const							{ return _Port;								}
		Uint16 getServerPortTree() const						{ return _portTree;							}
		int getTreeUpdateClientToServerDelay() const			{ return _treeUpdateClientToServerDelay;	}
		int getTreeUpdateServerToClientDelay() const			{ return _treeUpdateServerToClientDelay;	}

		void setIpServer(const string& ipServeur)										{ _IpServer = ipServeur;	}
		void setPort(Uint16 port)														{ _Port = port;				}
		void setPortTree(Uint16 portTree)												{ _portTree = portTree;		}
		void setTreeUpdateClientToServerDelay(int treeUpdateClientToServerDelay)	{ _treeUpdateClientToServerDelay = treeUpdateClientToServerDelay;	}
		void setTreeUpdateServerToClientDelay(int treeUpdateServerToClientDelay)	{ _treeUpdateServerToClientDelay = treeUpdateServerToClientDelay;	}

		void Init();
	};

	class CJoueur {		// Informations du joueur
	public:
		string nom;					// Nom du joueur
		string mapName;				// Nom de la map réprésentant le skin du joueur
		bool skinVisibility;		// Indique si le joueur (son skin) est affiché ou non
		bool outlineVisibility;		// Indique si les contours physiques du joueur (ellipsoïde qui l'entoure) est visible
	};

	class CPlugin {		// Configuration du moteur de plugin
	public:
		vector<string> _defaultPluging;
	};

	class CDebug {
	public:
		bool bSonPerformances;		// Indique si l'usage CPU du son doit être affiché
		bool bSonSpectre;			// Affiche le spectre sonore
		bool bAfficheFichier;		// Indique si l'ouverture des fichiers ASE doit se faire en mode debug (=>affichage dans la console durant la lecture)
		bool bAfficheNormaux;
		bool cubicMeterVisibility;	// Indique si le cube d'un mètre de côté servant à calibrer les dimensions de la scène est visible ou non
		bool axesMeterVisibility;	// Indique si les axes d'un mètre de côté servant à calibrer les dimensions de la scène sont visibles ou non
	};

	class CWeb {		// Configuration du moteur de plugin
		friend class CCfg;
	protected:
		Uint16 _htmlServerPort;
	public:

		void setHtmlServerPort(Uint16 htmlServerPort)		{ _htmlServerPort = htmlServerPort;		}
		Uint16 getHtmlServerPort()							{ return _htmlServerPort;				}
	};

	string nomFichierConfig;

public:
	CGeneral General;				// Configuration générale
	CDisplay Display;				// Configuration de l'affichage
	CAudio Audio;					// Configuration audio
	CCommandes Commandes;			// Configuration des commandes
	CCentralisateur Centralisateur;	// Configuration de la connexion au centralisateur
	CReseau Reseau;					// Configuration du réseau
	CJoueur Joueur;					// Informations sur le joueur principal (nom,...)
	CPlugin Plugin;					// Configuration du moteur de plugins
	CDebug Debug;					// Paramètres servant au débuggage
	CWeb Web;						// Paramètres du serveur web
		
	CCfg();

	void NommeConfig(const string &nomFichier);
	void AfficheDateCompilation();		// Affiche le n° de version du programme

	void Lit();			// Lit le fichier de configuration
	void Ecrit();		// Sauvegarde la configuration

	/**
	 * Retourne sous forme de texte descriptif le bouton souris en texte.
	 */
	static const char* resolve(Uint8 mouse);

	/**
	 * Retourne sous forme de texte descriptif la touche identifiée dans SDL.
	 */
	static const char* resolve(const SDLKey sym);

	/**
	 * Retourne sous forme de texte descriptif l'événement SDL.
	 */
	static void resolve(const SDL_Event* sdlEvent, string& description);
};

#endif
