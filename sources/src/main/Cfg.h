

#ifndef __JKT__CFG_H
#define __JKT__CFG_H

#define VERSION_DAY		31
#define VERSION_MONTH	10
#define VERSION_YEAR	2004

#include <string>
#ifdef WIN32
	#include <windows.h>
#endif

#include "SDL.h"
#include "GL/gl.h"
#include "glext.h"

using namespace std;

class CCfg {

	class CComID {
	public:
		SDLKey key;		// Touche du clavier correspondant
		int mouse;		// Touche de la souris correspondant
	};

	class CAudio {
	public :
		int m_Output;
		int m_Mixer;
		int m_Driver;
		int m_DriverRecord;

		bool Init();
	};

	class CCommandes {
	public:
		CComID Avancer;		// Avancer
		CComID Reculer;		// Reculer
		CComID Gauche;		// Aller à gauche
		CComID Droite;		// Aller à droite
		CComID Tir1;		// Tir primaire
		CComID Tir2;		// Tir secondaire
		CComID Monter;		// Monter
		const char* resolve(CComID com);					// Retourne la touche ou le bouton en texte
	};

	class CDisplay {
		friend class CCfg;
		bool m_bFullscreen;
		void InitSDL();
		void InitOpenGL();
        void InitAgar();
		bool chargeGLExtension(const char* ext, string& extensions);
	public :
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
		Uint16 _Port;			// Port à utiliser
		string _IpServer;		// Adresse IP du serveur (s'il s'agit d'un client)
		bool _serveur;	// true s'il s'agit d'un serveur, false si c'est un client
	public:
		Uint16 getPort() const							{ return _Port;				}
		string getIpServer() const						{ return _IpServer;			}
		bool isServeur() const							{ return _serveur;			}

		void setIpServer(const string& ipServeur)		{ _IpServer = ipServeur;	}
		void setPort(Uint16 port)						{ _Port = port;				}
		void setServeur(bool serveur)					{ _serveur = serveur;		}

		void Init();
	};

	class CJoueur {		// Informations du joueur
	public:
		string nom;					// Nom du joueur
		string mapName;				// Nom de la map réprésentant le skin du joueur
		bool skinVisibility;		// Indique si le joueur (son skin) est affiché ou non
		bool outlineVisibility;		// Indique si les contours physiques du joueur (ellipsoïde qui l'entoure) est visible
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

	string nomFichierConfig;

public:
	CDisplay Display;				// Configuration de l'affichage
	CAudio Audio;					// Configuration audio
	CCommandes Commandes;			// Configuration des commandes
	CCentralisateur Centralisateur;	// Configuration de la connexion au centralisateur
	CReseau Reseau;					// Configuration du réseau
	CJoueur Joueur;					// Informations sur le joueur principal (nom,...)
	CDebug Debug;					// Paramètres servant au débuggage
		
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
