

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

#include "GL/glew.h"
#include "SDL.h"

#include "main/config/ComId.h"

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
	static const char* CST_COM_CAMERA_MONTER;
	static const char* CST_COM_CAMERA_DESCENDRE;
	static const char* CST_COM_CAMERA_DROITE;
	static const char* CST_COM_CAMERA_GAUCHE;
	static const char* CST_COM_CAMERA_RECULER;
	static const char* CST_COM_CAMERA_AVANCER;
	static const char* CST_COM_VUE_RECULER;
	static const char* CST_COM_VUE_AVANCER;

	static const char* CST_COM_TIR1;
	static const char* CST_COM_TIR2;
	static const char* CST_COM_MONTER;
	static const char* CST_COM_SELECTWEAPONUP;
	static const char* CST_COM_SELECTWEAPONDOWN;
	static const char* CST_COM_GRAVITY;
	static const char* CST_COM_TEXTURES;

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
	static const char* CST_JOU_HITBOX_VISIBILITY;
	static const char* CST_JOU_SOLIDBOX_VISIBILITY;
	static const char* CST_JOU_SKIN_VISIBILITY;

	static const char* CST_PLU_ACT_BEGIN;
	static const char* CST_PLU_ACT_END;

	static const char* CST_WEB_TCP_SERVER_TIMEOUT;
	static const char* CST_WEB_TCP_SERVER_CLIENTS_SIZE;
	static const char* CST_WEB_HTML_PORT;

	static const char* CST_DEB_SONPERFORMANCES;
	static const char* CST_DEB_SONSPECTRE;
	static const char* CST_DEB_AFFICHEFICHIER;
	static const char* CST_DEB_AFFICHENORMAUX;
	static const char* CST_DEB_AXES_METER_VISIBILITY;
	static const char* CST_DEB_CUBIC_METER_VISIBILITY;


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

private:

	class CCommandes {
	public:
		CComID Avancer;				// Avancer
		CComID Reculer;				// Reculer
		CComID Gauche;				// Aller à gauche
		CComID Droite;				// Aller à droite
		CComID CameraMonter;		// Monter la caméra
		CComID CameraDescendre;		// Descendre la caméra
		CComID CameraDroite;		// Déplacer à droite la caméra
		CComID CameraGauche;		// Déplacer à gauche la caméra
		CComID CameraReculer;		// Reculer la caméra
		CComID CameraAvancer;		// Avancer la caméra
		CComID VueReculer;			// Avancer la caméra
		CComID VueAvancer;			// Avancer la caméra
		CComID Tir1;				// Tir primaire
		CComID Tir2;				// Tir secondaire
		CComID Monter;				// Monter
		CComID SelectWeaponUp;		// Sélectionner arme suivante
		CComID SelectWeaponDown;	// Sélectionner arme précédente
		CComID Gravity;				// Annuler / activer la gravité
		CComID Textures;			// Afficher / masquer le damier des textures
		const char* resolve(CComID com);					// Retourne la touche ou le bouton en texte
	};

	class CDisplay {
		friend class CCfg;
		bool m_bFullscreen;
		void InitSDL();
		void InitOpenGL();
		void InitAgar();
	public :
		std::string position;
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
		Uint16 m_Port;				// Port � utiliser
		std::string m_IpServer;			// Adresse IP du serveur (s'il s'agit d'un client)
	public:
		Uint16 getPort() const			{ return m_Port; }
		void setPort( Uint16 port )		{ m_Port = port; }
		std::string getIp() const		{ return m_IpServer; }
		void setIp( std::string ip )		{ m_IpServer = ip; }
	};

	class CReseau {		// Informations sur la config r�seau
		friend class CCfg;
	protected:
		Uint16 _Port;						// Port � utiliser
		Uint16 _portTree;					// Port � utiliser
		std::string _IpServer;					// Adresse IP du serveur (s'il s'agit d'un client)
		int _treeUpdateClientToServerDelay;	// Temps mini entre 2 mises � jour des donn�es de l'arbre par le client sur le serveur
		int _treeUpdateServerToClientDelay;	// Temps mini entre 2 mises � jour des donn�es de l'arbre par le serveur sur le client
	public:
		std::string getIpServer() const								{ return _IpServer;							}
		Uint16 getServerPort() const							{ return _Port;								}
		Uint16 getServerPortTree() const						{ return _portTree;							}
		int getTreeUpdateClientToServerDelay() const			{ return _treeUpdateClientToServerDelay;	}
		int getTreeUpdateServerToClientDelay() const			{ return _treeUpdateServerToClientDelay;	}

		void setIpServer(const std::string& ipServeur)										{ _IpServer = ipServeur;	}
		void setPort(Uint16 port)														{ _Port = port;				}
		void setPortTree(Uint16 portTree)												{ _portTree = portTree;		}
		void setTreeUpdateClientToServerDelay(int treeUpdateClientToServerDelay)	{ _treeUpdateClientToServerDelay = treeUpdateClientToServerDelay;	}
		void setTreeUpdateServerToClientDelay(int treeUpdateServerToClientDelay)	{ _treeUpdateServerToClientDelay = treeUpdateServerToClientDelay;	}

		void Init();
	};

	class CJoueur {		// Informations du joueur
	public:
		std::string nom;					// Nom du joueur
		std::string mapName;				// Nom de la map r�pr�sentant le skin du joueur
		bool skinVisibility;		// Indique si le joueur (son skin) est affich� ou non
		bool hitboxVisibility;		// Indique si la zone de frappe des joueurs est visible
		bool solidboxVisibility;	// Indique si le contours physiques du joueur (ellipso�de qui l'entoure) est visible
	};

	class CPlugin {		// Configuration du moteur de plugin
	public:
		std::vector<std::string> _defaultPluging;
	};

	class CDebug {
	public:
		bool bSonPerformances;		// Indique si l'usage CPU du son doit �tre affich�
		bool bSonSpectre;			// Affiche le spectre sonore
		bool bAfficheFichier;		// Indique si l'ouverture des fichiers ASE doit se faire en mode debug (=>affichage dans la console durant la lecture)
		bool bAfficheNormaux;
		bool cubicMeterVisibility;	// Indique si le cube d'un m�tre de c�t� servant � calibrer les dimensions de la sc�ne est visible ou non
		bool axesMeterVisibility;	// Indique si les axes d'un m�tre de c�t� servant � calibrer les dimensions de la sc�ne sont visibles ou non
	};

	class CWeb {		// Configuration du moteur de plugin
		friend class CCfg;
	protected:
		int _htmlTcpTimeout;
		int _htmlTcpClientsSize;
		Uint16 _htmlServerPort;
	public:

		void setHtmlTcpTimeout(int htmlTcpTimeout)			{ _htmlTcpTimeout = htmlTcpTimeout;		}
		int getHtmlTcpTimeout()								{ return _htmlTcpTimeout;				}

		void setHtmlTcpClientsSize(int htmlTcpClientsSize)	{ _htmlTcpClientsSize = htmlTcpClientsSize;		}
		int getHtmlTcpClientsSize()							{ return _htmlTcpClientsSize;				}

		void setHtmlServerPort(Uint16 htmlServerPort)		{ _htmlServerPort = htmlServerPort;		}
		Uint16 getHtmlServerPort()							{ return _htmlServerPort;				}
	};

	std::string configFile;

public:
	CGeneral General;				// Configuration g�n�rale
	CDisplay Display;				// Configuration de l'affichage
	CAudio Audio;					// Configuration audio
	CCommandes Commandes;			// Configuration des commandes
	CCentralisateur Centralisateur;	// Configuration de la connexion au centralisateur
	CReseau Reseau;					// Configuration du r�seau
	CJoueur Joueur;					// Informations sur le joueur principal (nom,...)
	CPlugin Plugin;					// Configuration du moteur de plugins
	CDebug Debug;					// Param�tres servant au d�buggage
	CWeb Web;						// Param�tres du serveur web

	CCfg();

	void NommeConfig(const std::string &configFileBaseName, const std::string& configFileSuffixName);
	void AfficheDateCompilation();		// Affiche le n� de version du programme

	void Lit();			// Lit le fichier de configuration
	void Ecrit();		// Sauvegarde la configuration

	/**
	 * Retourne sous forme de texte descriptif le bouton souris en texte.
	 */
	static const char* resolve(Uint8 mouse);

	/**
	 * Retourne sous forme de texte descriptif la touche identifi�e dans SDL.
	 */
	static const char* resolve(const SDLKey sym);

	/**
	 * Retourne sous forme de texte descriptif l'�v�nement SDL.
	 */
	static void resolve(const SDL_Event* sdlEvent, std::string& description);
};

#endif
