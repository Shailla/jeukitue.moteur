

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

class CCfg
{
	class CComID
	{
	public:
		SDLKey key;		// Touche du clavier correspondant
		int mouse;		// Touche de la souris correspondant
	};

	class CAudio
	{ 
	public :
		int m_Output;
		int m_Mixer;
		int m_Driver;
		int m_DriverRecord;

		bool Init();
	};

	class CCommandes
	{
	public:
		CComID Avancer;		// Avancer
		CComID Reculer;		// Reculer
		CComID Gauche;		// Aller à gauche
		CComID Droite;		// Aller à droite
		CComID Tir1;		// Tir primaire
		CComID Tir2;		// Tir secondaire
		CComID Monter;		// Monter
		char *resolve(CComID com);		// Retourne la touche ou le bouton en texte
		char *resolve(Uint8 mouse);		// Retourne le bouton souris en texte
		char *resolve(SDLKey sym);		// Retourne la touche en texte
	};

	class CDisplay
	{
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

	class CReseau		// Informations sur la config réseau
	{
		friend class CCfg;
	protected:
		Uint16 m_Port;	// Port à utiliser
		string m_IpServer;		// Adresse IP du serveur (s'il s'agit d'un client)
	public:
		bool serveur;	// true s'il s'agit d'un serveur, false si c'est un client
		Uint16 getPort() const			{ return m_Port;	}
		string getIpServer() const		{ return m_IpServer;		}
		void setPort( Uint16 port )		{ m_Port = port;	}
		void Init();
	};

	class CJoueur		// Informations du joueur
	{
	public:
		string nom;
	};

	class CDebug
	{
	public:
		bool bSonPerformances;		// Indique si l'usage CPU du son doit être affiché
		bool bSonSpectre;			// Affiche le spectre sonore
		bool bAfficheFichier;		// Indique si l'ouverture des fichiers ASE doit se faire en mode debug (=>affichage dans la console durant la lecture)
		bool bReseauDebit;
		bool bAfficheNormaux;
	};

	string nomFichierConfig;

public:
	CDisplay Display;		// Configuration de l'affichage
	CAudio Audio;			// Configuration audio
	CCommandes Commandes;	// Configuration des commandes
	CReseau Reseau;			// Configuration du réseau
	CJoueur Joueur;			// Informations sur le joueur principal (nom,...)
	CDebug Debug;			// Paramètres servant au débuggage
		
	CCfg();

	void NommeConfig( const string &nomFichier );
	void AfficheDateCompilation();		// Affiche le n° de version du programme

	void Lit();			// Lit le fichier de configuration
	void Ecrit();		// Sauvegarde la configuration
};

#endif

