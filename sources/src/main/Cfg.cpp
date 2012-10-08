
#include <string>
#include <fstream>
#include <iostream>

#include <agar/config/have_opengl.h>
#include <agar/config/have_sdl.h>
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/opengl.h>

using namespace std;

#ifdef WIN32
	#include <windows.h>
	#include <io.h>
	#include <direct.h>
#endif
#include <SDL.h>
#include <SDL_net.h>
#include <SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "fmod.h"
#include "fmod_errors.h"

#include "main/Extensions.h"
#include "main/divers.h"
#include "util/Trace.h"
#include "son/audio.h"
#include "ressource/RessourcesLoader.h"


#include "main/Cfg.h"

char **remarquesDriver;
int bpp, flags;
AG_Window* agarWindows;

SDL_Surface *screen;

PFNGLBINDBUFFERARBPROC glBindBuffer;
PFNGLGENBUFFERSARBPROC glGenBuffers;
PFNGLBUFFERDATAARBPROC glBufferData;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;

CCfg::CCfg()
{
TRACE().p( TRACE_OTHER, "Cfg::Cfg()%T", this );
}

void CCfg::AfficheDateCompilation() {
	cout << "Date de compilation : " << __DATE__ << endl;
}

void CCfg::NommeConfig(const string &nomFichier) {
TRACE().p( TRACE_OTHER, "Cfg::NommeConfig(nomFichier=%s)%T", nomFichier.c_str(), this );
	nomFichierConfig = nomFichier;
}

void CCfg::Lit() {
TRACE().p( TRACE_OTHER, "Cfg::Lit()%T", this );
	string mot;

	string nomFichierEntier = "./" + nomFichierConfig + ".ini";
	ifstream fichier(nomFichierEntier.c_str());

	if(!fichier) {
		quit_game("Erreur ouverture fichier de configuration '" + nomFichierEntier + "'", 1);
	}

	do fichier >> mot;
	while( mot!="-------------------------VIDEO-------------------------");

	{
		do fichier >> mot;	while( mot!="Display" );	// Propriétés d'affichage
		fichier >> Display.X >> Display.Y;

		do fichier >> mot;	while( mot!="Fullscreen" );	// Propriétés d'affichage
		fichier >> Display.m_bFullscreen;
	}

	do fichier >> mot;
	while( mot!="-------------------------AUDIO-------------------------" );

	{
		do fichier >> mot;	while( mot!="Output" );
		fichier >> Audio.m_Output;

		do fichier >> mot;	while( mot!="Mixer" );
		fichier >> Audio.m_Mixer;

		do fichier >> mot;	while( mot!="Driver" );
		fichier >> Audio.m_Driver;

		do fichier >> mot; while( mot!="DriverRecord" );
		fichier >> Audio.m_DriverRecord;
	}

	do fichier >> mot;
	while( mot!="-----------------------COMMANDES-----------------------" );

	{
		int crotte;

		do fichier >> mot;	while( mot!="Avancer" );
		fichier >> crotte;
		Commandes.Avancer.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Avancer.mouse = crotte;

		do fichier >> mot;	while( mot!="Reculer" );
		fichier >> crotte;
		Commandes.Reculer.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Reculer.mouse = crotte;

		do fichier >> mot;	while( mot!="Gauche" );
		fichier >> crotte;
		Commandes.Gauche.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Gauche.mouse = crotte;

		do fichier >> mot;	while( mot!="Droite" );
		fichier >> crotte;
		Commandes.Droite.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Droite.mouse = crotte;

		do fichier >> mot;	while( mot!="Tir1" );
		fichier >> crotte;
		Commandes.Tir1.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Tir1.mouse = crotte;

		do fichier >> mot;	while( mot!="Tir2" );
		fichier >> crotte;
		Commandes.Tir2.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Tir2.mouse = crotte;

		do fichier >> mot;	while( mot!="Monter" );
		fichier >> crotte;
		Commandes.Monter.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Monter.mouse = crotte;
	}

	do fichier >> mot;
	while( mot!="--------------------CENTRALISATEUR---------------------" );

	{
		do fichier >> mot;	while( mot!="ip" );
		fichier >> Centralisateur.m_IpServer;

		do fichier >> mot;	while( mot!="port" );
		fichier >> Centralisateur.m_Port;
	}

	do fichier >> mot;
	while( mot!="------------------------RESEAU-------------------------" );

	{
		do fichier >> mot;	while( mot!="Serveur" );
			fichier >> Reseau.serveur;

		do fichier >> mot;	while( mot!="ip" );
		fichier >> Reseau.m_IpServer;

		do fichier >> mot;	while( mot!="port" );
		fichier >> Reseau.m_Port;
	}

	do fichier >> mot;
	while( mot!="------------------------JOUEUR-------------------------" );

	{
		do fichier >> mot;	while( mot!="Nom" );
		fichier >> Joueur.nom;

		do fichier >> mot;	while( mot!="MapNom" );
		fichier >> Joueur.mapName;

		do fichier >> mot;	while( mot!="OutlineVisibility" );
		fichier >> Joueur.outlineVisibility;

		do fichier >> mot;	while( mot!="SkinVisibility" );
		fichier >> Joueur.skinVisibility;
	}

	do fichier >> mot;
	while( mot!="------------------------DEBUG-------------------------" );

	{
		do fichier >> mot;	while( mot!="SonPerformances" );
		fichier >> Debug.bSonPerformances;

		do fichier >> mot;	while( mot!="SonSpectre" );
		fichier >> Debug.bSonSpectre;

		do fichier >> mot;	while( mot!="AfficheFichier" );
		fichier >> Debug.bAfficheFichier;

		do fichier >> mot;	while( mot!="DebitReseau" );
		fichier >> Debug.bReseauDebit;

		do fichier >> mot;	while( mot!="AfficheNormaux" );
		fichier >> Debug.bAfficheNormaux;
	}

	fichier.close();
}

void CCfg::Ecrit() {
	string nomFichierEntier = "./" + nomFichierConfig + ".ini";
	ofstream fichier( nomFichierEntier.c_str() );

	fichier << "\n-------------------------VIDEO-------------------------\n";
	fichier << "\nDisplay\t\t" << Display.X << "\t" << Display.Y;
	fichier << "\nFullscreen\t\t" << Display.Fullscreen();

	fichier << "\n\n\n-------------------------AUDIO-------------------------\n";
	fichier << "\nOutput\t\t" << Audio.m_Output << "\t(" << resolveOutput(Audio.m_Output) << ")";
	fichier << "\nMixer\t\t" << Audio.m_Mixer << "\t(" << resolveMixer(Audio.m_Mixer) << ")";
	fichier << "\nDriver\t\t" << Audio.m_Driver << "\t(" << resolveDriver(Audio.m_Driver) << ")";

	if( Audio.m_DriverRecord!=-1 ) {
		fichier << "\nDriverRecord\t" << Audio.m_DriverRecord << "\t(" << resolveDriverRecord(Audio.m_DriverRecord) << ")";
	}
	else {
		fichier << "\nDriverRecord\t" << Audio.m_DriverRecord << "\t( Pas de driver d'acquisition )";
	}

	fichier << "\n\n\n-----------------------COMMANDES-----------------------\n";
	fichier << "\nAvancer\t\t" << Commandes.Avancer.key << "\t" << Commandes.Avancer.mouse << "\t(" << Commandes.resolve(Commandes.Avancer) << ")";
	fichier << "\nReculer\t\t" << Commandes.Reculer.key << "\t" << Commandes.Reculer.mouse << "\t(" << Commandes.resolve(Commandes.Reculer) << ")";
	fichier << "\nGauche\t\t" << Commandes.Gauche.key << "\t" << Commandes.Gauche.mouse << "\t(" << Commandes.resolve(Commandes.Gauche) << ")";
	fichier << "\nDroite\t\t" << Commandes.Droite.key << "\t" << Commandes.Droite.mouse << "\t(" << Commandes.resolve(Commandes.Droite) << ")";
	fichier << "\nTir1\t\t" << Commandes.Tir1.key << "\t" << Commandes.Tir1.mouse << "\t(" << Commandes.resolve(Commandes.Tir1) << ")";
	fichier << "\nTir2\t\t" << Commandes.Tir2.key << "\t" << Commandes.Tir2.mouse << "\t(" << Commandes.resolve(Commandes.Tir2) << ")";
	fichier << "\nMonter\t\t" << Commandes.Monter.key << "\t" << Commandes.Monter.mouse << "\t(" << Commandes.resolve(Commandes.Monter) << ")";

	fichier << "\n\n\n--------------------CENTRALISATEUR---------------------\n";
	fichier << "\nip\t\t" << Centralisateur.m_IpServer;
	fichier << "\nport\t\t" << Centralisateur.m_Port;

	fichier << "\n\n\n------------------------RESEAU-------------------------\n";
	fichier << "\nServeur\t\t" << Reseau.serveur;
	fichier << "\nip\t\t" << Reseau.m_IpServer;
	fichier << "\nport\t\t" << Reseau.m_Port;

	fichier << "\n\n\n------------------------JOUEUR-------------------------\n";
	fichier << "\nNom\t\t" << Joueur.nom;
	fichier << "\nMapNom\t\t" << Joueur.mapName;
	fichier << "\nOutlineVisibility\t\t" << Joueur.outlineVisibility;
	fichier << "\nSkinVisibility\t\t" << Joueur.skinVisibility;

	fichier << "\n\n\n------------------------DEBUG-------------------------\n";
	fichier << "\nSonPerformances\t\t" << Debug.bSonPerformances;
	fichier << "\nSonSpectre\t\t" << Debug.bSonSpectre;
	fichier << "\nAfficheFichier\t\t" << Debug.bAfficheFichier;
	fichier << "\nDebitReseau\t\t" << Debug.bReseauDebit;
	fichier << "\nAfficheNormaux\t\t" << Debug.bAfficheNormaux;

	fichier.close();
}

const char* CCfg::CCommandes::resolve(const CComID com) {
	if(com.key != 0)
		return resolve( com.key );
	else
		return resolve( com.mouse );
}

const char* CCfg::CCommandes::resolve(const Uint8 mouse) {
	switch(mouse)
	{
	case 1:		return "Souris_gauche";
	case 2:		return "Souris_centre";
	case 3:		return "Souris_droit";
	case 4:		return "Souris_centre_DOWN";
	case 5:		return "Souris_centre_UP";
	default:	return "Souris_?";
	}
}

const char* CCfg::CCommandes::resolve(const SDLKey sym) {
	const char* result;

	switch(sym) {
	case SDLK_BACKSPACE:	result = "BACKSPACE";		break;
	case SDLK_TAB:			result = "TAB";				break;
	case SDLK_CLEAR:		result = "clear";			break;
	case SDLK_RETURN:		result = "ENTER";			break;
	case SDLK_PAUSE:		result = "pause";			break;
	case SDLK_ESCAPE:		result = "escape";			break;
	case SDLK_SPACE:		result = "space";			break;
	case SDLK_EXCLAIM:		result = "!";				break;
	case SDLK_QUOTEDBL:		result = "guillemets";		break;
	case SDLK_HASH:			result = "#";				break;
	case SDLK_DOLLAR:		result = "$";				break;
	case SDLK_AMPERSAND:	result = "&";				break;
	case SDLK_QUOTE:		result = "'";				break;
	case SDLK_LEFTPAREN:	result = "(";				break;
	case SDLK_RIGHTPAREN:	result = ")";				break;
	case SDLK_ASTERISK:		result = "*";				break;
	case SDLK_PLUS:			result = "+";				break;
	case SDLK_COMMA:		result = ",";				break;
	case SDLK_MINUS:		result = "-";				break;
	case SDLK_PERIOD:		result = ".";				break;
	case SDLK_SLASH:		result = "/";				break;
	case SDLK_0:			result = "0";				break;
	case SDLK_1:			result = "1";				break;
	case SDLK_2:			result = "2";				break;
	case SDLK_3:			result = "3";				break;
	case SDLK_4:			result = "4";				break;
	case SDLK_5:			result = "5";				break;
	case SDLK_6:			result = "6";				break;
	case SDLK_7:			result = "7";				break;
	case SDLK_8:			result = "8";				break;
	case SDLK_9:			result = "9";				break;
	case SDLK_COLON:		result = ":";				break;
	case SDLK_SEMICOLON:	result = ";";				break;
	case SDLK_LESS:			result = "<";				break;
	case SDLK_EQUALS:		result = "=";				break;
	case SDLK_GREATER:		result = ">";				break;
	case SDLK_QUESTION:		result = "?";				break;
	case SDLK_AT:			result = "@";				break;
	case SDLK_LEFTBRACKET:	result = "[";				break;
	case SDLK_BACKSLASH:	result = "\\";				break;
	case SDLK_RIGHTBRACKET:	result = "]";				break;
	case SDLK_CARET:		result = "^";				break;
	case SDLK_UNDERSCORE:	result = "_";				break;
	case SDLK_BACKQUOTE:	result = "`";				break;
	case SDLK_a:			result = "a";				break;
	case SDLK_b:			result = "b";				break;
	case SDLK_c:			result = "c";				break;
	case SDLK_d:			result = "d";				break;
	case SDLK_e:			result = "e";				break;
	case SDLK_f:			result = "f";				break;
	case SDLK_g:			result = "g";				break;
	case SDLK_h:			result = "h";				break;
	case SDLK_i:			result = "i";				break;
	case SDLK_j:			result = "j";				break;
	case SDLK_k:			result = "k";				break;
	case SDLK_l:			result = "l";				break;
	case SDLK_m:			result = "m";				break;
	case SDLK_n:			result = "n";				break;
	case SDLK_o:			result = "o";				break;
	case SDLK_p:			result = "p";				break;
	case SDLK_q:			result = "q";				break;
	case SDLK_r:			result = "r";				break;
	case SDLK_s:			result = "s";				break;
	case SDLK_t:			result = "t";				break;
	case SDLK_u:			result = "u";				break;
	case SDLK_v:			result = "v";				break;
	case SDLK_w:			result = "w";				break;
	case SDLK_x:			result = "x";				break;
	case SDLK_y:			result = "y";				break;
	case SDLK_z:			result = "z";				break;
	case SDLK_DELETE:		result = "delete";			break;
	case SDLK_KP0:			result = "KP0";				break;
	case SDLK_KP1:			result = "KP1";				break;
	case SDLK_KP2:			result = "KP2";				break;
	case SDLK_KP3:			result = "KP3";				break;
	case SDLK_KP4:			result = "KP4";				break;
	case SDLK_KP5:			result = "KP5";				break;
	case SDLK_KP6:			result = "KP6";				break;
	case SDLK_KP7:			result = "KP7";				break;
	case SDLK_KP8:			result = "KP8";				break;
	case SDLK_KP9:			result = "KP9";				break;
	case SDLK_KP_PERIOD:	result = "KP_.";			break;
	case SDLK_KP_DIVIDE:	result = "KP_/";			break;
	case SDLK_KP_MULTIPLY:	result = "KP_*";			break;
	case SDLK_KP_MINUS:		result = "KP_-";			break;
	case SDLK_KP_PLUS:		result = "KP_+";			break;
	case SDLK_KP_ENTER:		result = "KP_ENTER";		break;
	case SDLK_KP_EQUALS:	result = "KP_=";			break;
	case SDLK_UP:			result = "up_arrow";		break;
	case SDLK_DOWN:			result = "down_arrow";		break;
	case SDLK_RIGHT:		result = "right_arrow";		break;
	case SDLK_LEFT:			result = "left_arrow";		break;
	case SDLK_INSERT:		result = "insert";			break;
	case SDLK_HOME:			result = "home";			break;
	case SDLK_END:			result = "end";				break;
	case SDLK_PAGEUP:		result = "page_up";			break;
	case SDLK_PAGEDOWN:		result = "page_down";		break;
	case SDLK_F1:			result = "F1";				break;
	case SDLK_F2:			result = "F2";				break;
	case SDLK_F3:			result = "F3";				break;
	case SDLK_F4:			result = "F4";				break;
	case SDLK_F5:			result = "F5";				break;
	case SDLK_F6:			result = "F6";				break;
	case SDLK_F7:			result = "F7";				break;
	case SDLK_F8:			result = "F8";				break;
	case SDLK_F9:			result = "F9";				break;
	case SDLK_F10:			result = "F10";				break;
	case SDLK_F11:			result = "F11";				break;
	case SDLK_F12:			result = "F12";				break;
	case SDLK_F13:			result = "F13";				break;
	case SDLK_F14:			result = "F14";				break;
	case SDLK_F15:			result = "F15";				break;
	case SDLK_NUMLOCK:		result = "numlock";			break;
	case SDLK_CAPSLOCK:		result = "capslock";		break;
	case SDLK_SCROLLOCK:	result = "scrollock";		break;
	case SDLK_RSHIFT:		result = "right_shift";		break;
	case SDLK_LSHIFT:		result = "left_shift";		break;
	case SDLK_RCTRL:		result = "right_ctrl";		break;
	case SDLK_LCTRL:		result = "left_ctrl";		break;
	case SDLK_RALT:			result = "right_alt";		break;
	case SDLK_LALT:			result = "left_alt";		break;
	case SDLK_RMETA:		result = "right_meta";		break;
	case SDLK_LMETA:		result = "left_meta";		break;
	case SDLK_LSUPER:		result = "left_windows";	break;
	case SDLK_RSUPER:		result = "right_windows";	break;
	case SDLK_MODE:			result = "mode_shift";		break;
	case SDLK_HELP:			result = "help";			break;
	case SDLK_PRINT:		result = "print-screen";	break;
	case SDLK_SYSREQ:		result = "SysRq";			break;
	case SDLK_BREAK:		result = "break";			break;
	case SDLK_MENU:			result = "menu";			break;
	case SDLK_POWER:		result = "power";			break;
	case SDLK_EURO:			result = "euro";			break;

	default:				result = "INCONNU";			break;
	}

	return result;
}

bool CCfg::CAudio::Init() {
	TRACE().p( TRACE_SON, "CCfg::CAudio::Init()" );
	unsigned int caps = 0;

		// Initialisation audio de FMOD
	cout << "\nVersion de FMOD\t\t" << FSOUND_GetVersion() << endl;

	if(FSOUND_GetVersion() < FMOD_VERSION) {
		cout << "Error : You are using the wrong DLL version!  You should be using FMOD" <<
			FMOD_VERSION << endl;
		return false;
	}

	FSOUND_SetOutput( m_Output );		// Sélection de l'output

	remarquesDriver = new char*[FSOUND_GetNumDrivers()];
	string rem;

	for(int i=0 ; i<FSOUND_GetNumDrivers() ; i++)
	{
		FSOUND_GetDriverCaps(i, &caps);
		rem = "";

		if (!caps)
			rem = "Supporte software mode uniquement. Supporte mal 3D sound hardware. ";
		if (caps & FSOUND_CAPS_HARDWARE)
			rem += "Supporte hardware 3D sound. ";
		if (caps & FSOUND_CAPS_EAX2)
			rem += "Supporte EAX 2 reverb. ";
		if (caps & FSOUND_CAPS_EAX3)
			rem += "Supporte EAX 3 reverb. ";

		remarquesDriver[i] = new char[rem.size()];
		strcpy( remarquesDriver[i], rem.c_str() );
	}

	if(FSOUND_GetNumDrivers != 0)	// Vérifie s'il y a un driver pour le son
	{
		if(m_Driver<FSOUND_GetNumDrivers() ) {
			FSOUND_SetDriver(m_Driver);	// Sélection du driver configuré
		}
		else	// Le driver configuré ne peut pas fonctionner
		{
			cerr << endl << "Driver son mal configuré, choix par défaut.";
			m_Driver = 0;	// Modification du fichier de configuration
			FSOUND_SetDriver(0);						// Choix du driver par défaut
		}
	}
	else {
		// Pas de driver son
		cerr << endl << "Aucun driver son détecté.";
	}

	FSOUND_SetMixer( m_Mixer );			// Sélection du mixer

		// Initialisation
	if(!FSOUND_Init(44100, 32, FSOUND_INIT_GLOBALFOCUS))
	{
		cerr << endl << "Erreur FSOUND_Init : "<< FMOD_ErrorString(FSOUND_GetError()) << endl;
		FSOUND_Close();
//		return -1;
	}

	FSOUND_DSP_SetActive( FSOUND_DSP_GetFFTUnit(), true );

	if( FSOUND_Record_GetNumDrivers()!=0 ) {	// Vérifie s'il y a un driver d'acquisition du son
		if( m_DriverRecord<FSOUND_Record_GetNumDrivers() ) {
			if( !FSOUND_Record_SetDriver(m_DriverRecord))	// Sélection du driver
			{															// d'entrée pour le micro
				cerr << "Erreur FSOUND_Record_SetDriver : " << FMOD_ErrorString(FSOUND_GetError()) << endl;
				cerr << "\tAVIS AU PROGRAMMEUR" << endl
					<< "Le programme va planter s'il y a une tentative d'utilisation du micro !!!" << endl;
		/*		FSOUND_Close();
				return -1;*/
			}
		}
		else {		// Le driver de record ne peut pas fonctionner
			cerr << endl << "Driver de record son mal configuré, choix par défaut.";
			m_DriverRecord = 0;	// Modification de la config => choix par défaut
			if( !FSOUND_Record_SetDriver(0) ) {		// Sélection du driver de record par défaut
				cerr << "Erreur FSOUND_Record_SetDriver : " << FMOD_ErrorString(FSOUND_GetError()) << endl;
				cerr << "\tAVIS AU PROGRAMMEUR" << endl
					<< "Le programme va planter s'il y a une tentative d'utilisation du micro !!!" << endl;
		/*		FSOUND_Close();
				return -1;*/
			}
		}
	}
	else {		// Aucun driver d'acquisition du son détecté
		m_DriverRecord = -1;
		cerr << endl << "Aucun driver d'acquisition du son détecté.";
	}

		// RESUME DE L'INITIALISATION
	cout << "\n\tRESUME DE L'INITIALISATION AUDIO\n";
	cout << "FSOUND Output Method : " << resolveOutput(FSOUND_GetOutput()) << endl;
	cout << "FSOUND Mixer         : " << resolveMixer(FSOUND_GetMixer()) << endl;
	cout << "FSOUND Driver        : " << resolveDriver(FSOUND_GetDriver()) << endl;

	FSOUND_GetDriverCaps(FSOUND_GetDriver(), &caps);

	if (!caps)
		cout << "\tCe driver supporte seulement le mode software.\n\tIl ne supporte pas bien le son 3D.\n";
	if (caps & FSOUND_CAPS_HARDWARE)
		cout << "\t- Ce driver supporte le son 3D en hardware!\n";
	if (caps & FSOUND_CAPS_EAX2)
		cout << "\t- Ce driver supporte l'EAX 2 reverb!\n";
	if (caps & FSOUND_CAPS_EAX3)
		cout << "\t- Ce driver supporte l'EAX 3 reverb!\n";

	cout << "FSOUND Record Driver : " << resolveDriverRecord(FSOUND_Record_GetDriver()) << endl;

	int num2d, num3d, total;
	FSOUND_GetNumHWChannels( &num2d, &num3d, &total );

	cout << "Hardware 2D channels : " << num2d << endl;
	cout << "Hardware 3D channels : " << num3d << endl;
	cout << "Total hardware channels : " << total << endl;

	return true;
}

void CCfg::CDisplay::Init() {
	InitSDL();
	InitOpenGL();
    InitAgar();
}

void CCfg::CDisplay::InitSDL() {
TRACE().p( TRACE_OTHER, "init_SDL(config) begin" );

#ifdef WIN32
	_putenv("SDL_VIDEODRIVER=directx");	// A FAIRE A FAIRE A FAIRE : code compatible Linux
#endif

	if( SDL_Init( SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_NOPARACHUTE ) < 0 )	// First, initialize SDL's video subsystem
	{
TRACE().p( TRACE_ERROR, "SDL_Init() failed : %s", SDLNet_GetError() );
		cerr << endl << "Error : Video initialization failed: " << SDL_GetError( ) << endl;
        exit( 1 );
    }

	// Icone et titre
	SDL_WM_SetCaption( "JKT 2010", "C'est un jeu qui tue !!!" );	// Titre et icon de la fenêtre

	string iconeFichier = "@Icone/Icone.bmp";
	JktUtils::RessourcesLoader::getFileRessource(iconeFichier);
	SDL_WM_SetIcon( IMG_Load( iconeFichier.c_str() ), 0 );


    const SDL_VideoInfo* info = SDL_GetVideoInfo( );	// Let's get some video information
    if( !info ) {
		cerr << endl << "Error : Video query failed: " << SDL_GetError( ) << endl;
        exit( 1 );
    }

    bpp = info->vfmt->BitsPerPixel;

    if( SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 )==-1 ) {
		cerr << "Erreur init_SDL : SDL_GL_RED_SIZE\n";
		exit( 1 );
	}

    if( SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 )==-1 ) {
		cerr << "Erreur init_SDL : SDL_GL_GREEN_SIZE\n";
		exit( 1 );
	}

    if( SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 )==-1 ) {
		cerr << "Erreur init_SDL : SDL_GL_BLUE_SIZE\n";
		exit( 1 );
	}

    if( SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 )==-1 ) {
		cerr << "Erreur init_SDL : SDL_GL_ALPHE_SIZE\n";
		exit( 1 );
	}

	if( SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 )==-1 ) {
		cerr << "Erreur init_SDL : SDL_GL_DEPTH_SIZE\n";
			exit( 1 );
	}

    if( SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 )==-1 ) {
		cerr << "Erreur init_SDL : SDL_GL_DOUBLEBUFFER\n";
		exit( 1 );
	}

	cout << endl << "hw_available : " << info->hw_available;
	cout << endl << "wm_available : " << info->wm_available;
	cout << endl << "blit_hw:1 : " << info->blit_hw;
	cout << endl << "blit_hw_CC:1 : " << info->blit_hw_CC;
	cout << endl << "blit_hw_A:1 : " << info->blit_hw_A;
	cout << endl << "blit_sw:1 : " << info->blit_sw;
	cout << endl << "blit_sw_CC:1 : " << info->blit_sw_CC;
	cout << endl << "blit_sw_A:1 : " << info->blit_sw_A;
	cout << endl << "blit_fill : " << info->blit_fill;
	cout << endl << "video_mem : " << info->video_mem << endl;

    flags = SDL_OPENGL | SDL_RESIZABLE;

	if( Fullscreen() )
		flags |= SDL_FULLSCREEN;

		// Get available fullscreen/hardware modes
	SDL_Rect **modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);

		// Check if there are any modes available
	if( modes == (SDL_Rect **)0 ) {
		cout << endl << "Aucun mode video n'est disponible" << endl;
		exit( 1 );
	}

	/* Check if our resolution is restricted */
	if(modes == (SDL_Rect **)-1) {
		cout << endl << "Tous les modes video sont disponibles" << endl;
	}
	else {
	/* Print valid modes */
		cout << endl << "Modes video disponibles : ";
		bool isFirst = true;

		for( int i=0 ; modes[i] ; ++i ) {
			isFirst?isFirst = false:cout << " ; ";
			cout << "[" << modes[i]->w << "*" << modes[i]->h << "]";
		}
	}

	if( !SDL_VideoModeOK(X, Y, bpp, flags) ) {
		cout << endl << "WARNING : Mode video indisponible =>";
		cout << "mode choisi par defaut, corrigez la configuration" << endl;
		X = modes[0]->w;
		Y = modes[0]->h;
	}

	screen = SDL_SetVideoMode(X, Y, bpp, flags);		// Set the video mode

	if( screen == 0) {
		cerr << endl << "Error : Video mode set failed: " << SDL_GetError() << endl;
		exit( 1 );
    }

	SDL_WarpMouse( 250, 250 );			//positionnement du curseur de la souris
	SDL_ShowCursor( SDL_DISABLE );		//Cache le curseur de la souris
	SDL_WM_GrabInput( SDL_GRAB_ON );	//Active le mode déplacement relatif de la souris

	SDL_EnableKeyRepeat( 500, 500 );	// Répétition des touches clavier

	char txt1[50];
	cout << "\nDriver video :\t\t";
	if( SDL_VideoDriverName( txt1, 50 ) )
		cout << txt1;

	cout << "\nVideo memory :\t\t" << info->video_mem/1024 << " Mo" << endl;

TRACE().p( TRACE_OTHER, "init_SDL(config) end" );
}

void CCfg::CDisplay::InitOpenGL() {
TRACE().p( TRACE_OTHER, "setup_opengl(width=%d,height=%d) begin", X, Y );
		// Informations openGL
	cout << "Version openGL :\t" << glGetString(GL_VERSION);
	cout << "\nModele de la carte graphique :\t" << glGetString(GL_RENDERER );
	cout << "\nFabricant de la carte graphique :\t" << glGetString(GL_VENDOR );
	cout << "\nExtensions openGL disponibles :\t" << glGetString(GL_EXTENSIONS);
	cout << "\nVersion GLU :\t\t" << gluGetString(GLU_VERSION);

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );	// Vide le tampon chromatique
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glShadeModel( GL_SMOOTH );				// Mode dégradé pour le remplissage des polynomes
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glViewport( 0, 0, X, Y );

		// Récupération de la liste des extensions
	std::string extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));

		// Chargement des extensions
	if(!chargeGLExtension("GL_ARB_vertex_buffer_object",extensions))
		exit(1);

	glGenBuffers = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
	glBindBuffer = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
	glBufferData = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
	glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");

	if(!glGenBuffers) {
		cerr << "\nError : glGenBuffersARB not found\n";
		exit(1);
	}

	if(!glBindBuffer) {
		cerr << "\nError : glBindBufferARB not found\n";
		exit(1);
	}

	if(!glBufferData) {
		cerr << "\nError : glBufferDataARB not found\n";
		exit(1);
	}

	if(!glDeleteBuffers) {
		cerr << "\nError : glDeleteBuffersARB not found\n";
		exit(1);
	}

	cout << "\nErreur OpenGL : " << gluErrorString(glGetError());

TRACE().p( TRACE_OTHER, "setup_opengl() end" );
}

void CCfg::CDisplay::InitAgar() {
TRACE().p( TRACE_OTHER, "setup Agar" );
		// Informations Agar
    AG_AgarVersion agarVersion;
    AG_GetVersion(&agarVersion);

    cout << endl << "Version Agar : " << agarVersion.major;
    cout << "." << agarVersion.minor;
    cout << "." << agarVersion.patch;
    cout << " [" << agarVersion.release << "]";

	// Configuration Agar (librairie de gestion du menu)
	if(AG_InitCore("agar", 0) == -1 || AG_InitVideoSDL(screen, flags) == -1) {
		cerr << "\nERREUR d'initialisation Agar : '" << AG_GetError() << "'";
	}

	// Noms des drivers disponibles pour Agar
	char drvNames[256];
	AG_ListDriverNames(drvNames, sizeof(drvNames));
	cout << endl << "Available drivers : " << drvNames;
}

bool CCfg::CDisplay::chargeGLExtension(const char* ext, string& extensions) {
		// Recherche de l'extension qui nous intéresse : GL_ARB_texture_compression
	if (extensions.find(ext) != std::string::npos) {
		cout << endl << "Extension supportée : " << ext ;
		return true;
	}
	else {
		cerr << endl << "Extension non-supportée : " << ext ;
		return false;
	}
}

void CCfg::CDisplay::ChangeVideoSize(int x, int y) {
TRACE().p( TRACE_OTHER, "changeVideoSize(config) begin" );
	if( !SDL_VideoModeOK(x, y, bpp, flags) ) {
		cerr << "\nMode video invalide :\t\t" << endl;
		return;
	}

	if( screen )
		SDL_FreeSurface( screen );							// Vide la surface

	screen = SDL_SetVideoMode( x, y, bpp, flags );	// Set the video mode
	if(screen == 0)
	{
        cerr << "Video mode set failed: " << SDL_GetError() << endl;
		exit( 1 );
    }

	X = x;
	Y = y;

	InitOpenGL();

TRACE().p( TRACE_OTHER, "changeVideoSize() end" );
}

void CCfg::CReseau::Init() {
	if(SDLNet_Init() == -1) {
TRACE().p( TRACE_ERROR, "SDLNet_Init() failed : %s", SDLNet_GetError() );
		cerr << endl << "Error : SDLNet_Init : %¨s\n" << SDLNet_GetError();
		exit( 1 );
	}
}
