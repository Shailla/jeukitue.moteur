
#include <string>
#include <fstream>
#include <iostream>

#include <agar/config/have_opengl.h>
#include <agar/core.h>
#include <agar/gui.h>

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

void CCfg::AfficheDateCompilation()
{
	cout << "Date de compilation : " << __DATE__ << endl;
}

void CCfg::NommeConfig( const string &nomFichier )
{
TRACE().p( TRACE_OTHER, "Cfg::NommeConfig(nomFichier=%s)%T", nomFichier.c_str(), this );
	nomFichierConfig = nomFichier;
}

void CCfg::Lit()
{
TRACE().p( TRACE_OTHER, "Cfg::Lit()%T", this );
	string mot;
	string nomFichierEntier = "./" + nomFichierConfig + ".ini";

	ifstream fichier( nomFichierEntier.c_str() );

	do fichier >> mot;	while( mot!="-------------------------VIDEO-------------------------" );
	{
		do fichier >> mot;	while( mot!="Display" );	// Propriétés d'affichage
		fichier >> Display.X >> Display.Y;

		do fichier >> mot;	while( mot!="Fullscreen" );	// Propriétés d'affichage
		fichier >> Display.m_bFullscreen;
	}

	do fichier >> mot;	while( mot!="-------------------------AUDIO-------------------------" );
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
	do fichier >> mot;	while( mot!="-----------------------COMMANDES-----------------------" );
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

	do fichier >> mot;	while( mot!="--------------------CENTRALISATEUR---------------------" );
	{
		do fichier >> mot;	while( mot!="ip" );
		fichier >> Centralisateur.m_IpServer;

		do fichier >> mot;	while( mot!="port" );
		fichier >> Centralisateur.m_Port;
	}

	do fichier >> mot;	while( mot!="------------------------RESEAU-------------------------" );
	{
		do fichier >> mot;	while( mot!="Serveur" );
			fichier >> Reseau.serveur;

		do fichier >> mot;	while( mot!="ip" );
		fichier >> Reseau.m_IpServer;

		do fichier >> mot;	while( mot!="port" );
		fichier >> Reseau.m_Port;
	}

	do fichier >> mot;	while( mot!="------------------------JOUEUR-------------------------" );
	{
		do fichier >> mot;	while( mot!="Nom" );
		fichier >> Joueur.nom;
	}

	do fichier >> mot;	while( mot!="------------------------DEBUG-------------------------" );
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

void CCfg::Ecrit()
{
	string nomFichierEntier = "./" + nomFichierConfig + ".ini";
	ofstream fichier( nomFichierEntier.c_str() );

	fichier << "\n-------------------------VIDEO-------------------------\n";
	fichier << "\nDisplay\t\t" << Display.X << "\t" << Display.Y;
	fichier << "\nFullscreen\t\t" << Display.Fullscreen();

	fichier << "\n\n\n-------------------------AUDIO-------------------------\n";
	fichier << "\nOutput\t\t" << Audio.m_Output << "\t(" << resolveOutput(Audio.m_Output) << ")";
	fichier << "\nMixer\t\t" << Audio.m_Mixer << "\t(" << resolveMixer(Audio.m_Mixer) << ")";
	fichier << "\nDriver\t\t" << Audio.m_Driver << "\t(" << resolveDriver(Audio.m_Driver) << ")";

	if( Audio.m_DriverRecord!=-1 )
	{
		fichier << "\nDriverRecord\t" << Audio.m_DriverRecord << "\t(" << resolveDriverRecord(Audio.m_DriverRecord) << ")";
	}
	else
	{
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

	fichier << "\n\n\n------------------------DEBUG-------------------------\n";
	fichier << "\nSonPerformances\t\t" << Debug.bSonPerformances;
	fichier << "\nSonSpectre\t\t" << Debug.bSonSpectre;
	fichier << "\nAfficheFichier\t\t" << Debug.bAfficheFichier;
	fichier << "\nDebitReseau\t\t" << Debug.bReseauDebit;
	fichier << "\nAfficheNormaux\t\t" << Debug.bAfficheNormaux;

	fichier.close();
}

char *CCfg::CCommandes::resolve(CComID com)
{
	if( com.key!=0 )
		return resolve( com.key );
	else
		return resolve( com.mouse );
}

char *CCfg::CCommandes::resolve(Uint8 mouse)
{
	switch( mouse )
	{
	case 1:		return "Souris_gauche";
	case 2:		return "Souris_centre";
	case 3:		return "Souris_droit";
	case 4:		return "Souris_centre_DOWN";
	case 5:		return "Souris_centre_UP";
	default:	return "Souris_?";
	}
}

char *CCfg::CCommandes::resolve(SDLKey sym)
{
	switch( sym )
	{
	case SDLK_BACKSPACE:	return "BACKSPACE";		break;
	case SDLK_TAB:			return "TAB";			break;
	case SDLK_CLEAR:		return "clear";			break;
	case SDLK_RETURN:		return "ENTER";			break;
	case SDLK_PAUSE:		return "pause";			break;
	case SDLK_ESCAPE:		return "escape";		break;
	case SDLK_SPACE:		return "space";			break;
	case SDLK_EXCLAIM:		return "!";				break;
	case SDLK_QUOTEDBL:		return "guillemets";	break;
	case SDLK_HASH:			return "#";				break;
	case SDLK_DOLLAR:		return "$";				break;
	case SDLK_AMPERSAND:	return "&";				break;
	case SDLK_QUOTE:		return "'";				break;
	case SDLK_LEFTPAREN:	return "(";				break;
	case SDLK_RIGHTPAREN:	return ")";				break;
	case SDLK_ASTERISK:		return "*";				break;
	case SDLK_PLUS:			return "+";				break;
	case SDLK_COMMA:		return ",";				break;
	case SDLK_MINUS:		return "-";				break;
	case SDLK_PERIOD:		return ".";				break;
	case SDLK_SLASH:		return "/";				break;
	case SDLK_0:			return "0";				break;
	case SDLK_1:			return "1";				break;
	case SDLK_2:			return "2";				break;
	case SDLK_3:			return "3";				break;
	case SDLK_4:			return "4";				break;
	case SDLK_5:			return "5";				break;
	case SDLK_6:			return "6";				break;
	case SDLK_7:			return "7";				break;
	case SDLK_8:			return "8";				break;
	case SDLK_9:			return "9";				break;
	case SDLK_COLON:		return ":";				break;
	case SDLK_SEMICOLON:	return ";";				break;
	case SDLK_LESS:			return "<";				break;
	case SDLK_EQUALS:		return "=";				break;
	case SDLK_GREATER:		return ">";				break;
	case SDLK_QUESTION:		return "?";				break;
	case SDLK_AT:			return "@";				break;
	case SDLK_LEFTBRACKET:	return "[";				break;
	case SDLK_BACKSLASH:	return "\\";			break;
	case SDLK_RIGHTBRACKET:	return "]";				break;
	case SDLK_CARET:		return "^";				break;
	case SDLK_UNDERSCORE:	return "_";				break;
	case SDLK_BACKQUOTE:	return "`";				break;
	case SDLK_a:			return "a";				break;
	case SDLK_b:			return "b";				break;
	case SDLK_c:			return "c";				break;
	case SDLK_d:			return "d";				break;
	case SDLK_e:			return "e";				break;
	case SDLK_f:			return "f";				break;
	case SDLK_g:			return "g";				break;
	case SDLK_h:			return "h";				break;
	case SDLK_i:			return "i";				break;
	case SDLK_j:			return "j";				break;
	case SDLK_k:			return "k";				break;
	case SDLK_l:			return "l";				break;
	case SDLK_m:			return "m";				break;
	case SDLK_n:			return "n";				break;
	case SDLK_o:			return "o";				break;
	case SDLK_p:			return "p";				break;
	case SDLK_q:			return "q";				break;
	case SDLK_r:			return "r";				break;
	case SDLK_s:			return "s";				break;
	case SDLK_t:			return "t";				break;
	case SDLK_u:			return "u";				break;
	case SDLK_v:			return "v";				break;
	case SDLK_w:			return "w";				break;
	case SDLK_x:			return "x";				break;
	case SDLK_y:			return "y";				break;
	case SDLK_z:			return "z";				break;
	case SDLK_DELETE:		return "delete";		break;
	case SDLK_KP0:			return "KP0";			break;
	case SDLK_KP1:			return "KP1";			break;
	case SDLK_KP2:			return "KP2";			break;
	case SDLK_KP3:			return "KP3";			break;
	case SDLK_KP4:			return "KP4";			break;
	case SDLK_KP5:			return "KP5";			break;
	case SDLK_KP6:			return "KP6";			break;
	case SDLK_KP7:			return "KP7";			break;
	case SDLK_KP8:			return "KP8";			break;
	case SDLK_KP9:			return "KP9";			break;
	case SDLK_KP_PERIOD:	return "KP_.";			break;
	case SDLK_KP_DIVIDE:	return "KP_/";			break;
	case SDLK_KP_MULTIPLY:	return "KP_*";			break;
	case SDLK_KP_MINUS:		return "KP_-";			break;
	case SDLK_KP_PLUS:		return "KP_+";			break;
	case SDLK_KP_ENTER:		return "KP_ENTER";		break;
	case SDLK_KP_EQUALS:	return "KP_=";			break;
	case SDLK_UP:			return "up_arrow";		break;
	case SDLK_DOWN:			return "down_arrow";	break;
	case SDLK_RIGHT:		return "right_arrow";	break;
	case SDLK_LEFT:			return "left_arrow";	break;
	case SDLK_INSERT:		return "insert";		break;
	case SDLK_HOME:			return "home";			break;
	case SDLK_END:			return "end";			break;
	case SDLK_PAGEUP:		return "page_up";		break;
	case SDLK_PAGEDOWN:		return "page_down";		break;
	case SDLK_F1:			return "F1";			break;
	case SDLK_F2:			return "F2";			break;
	case SDLK_F3:			return "F3";			break;
	case SDLK_F4:			return "F4";			break;
	case SDLK_F5:			return "F5";			break;
	case SDLK_F6:			return "F6";			break;
	case SDLK_F7:			return "F7";			break;
	case SDLK_F8:			return "F8";			break;
	case SDLK_F9:			return "F9";			break;
	case SDLK_F10:			return "F10";			break;
	case SDLK_F11:			return "F11";			break;
	case SDLK_F12:			return "F12";			break;
	case SDLK_F13:			return "F13";			break;
	case SDLK_F14:			return "F14";			break;
	case SDLK_F15:			return "F15";			break;
	case SDLK_NUMLOCK:		return "numlock";		break;
	case SDLK_CAPSLOCK:		return "capslock";		break;
	case SDLK_SCROLLOCK:	return "scrollock";		break;
	case SDLK_RSHIFT:		return "right_shift";	break;
	case SDLK_LSHIFT:		return "left_shift";	break;
	case SDLK_RCTRL:		return "right_ctrl";	break;
	case SDLK_LCTRL:		return "left_ctrl";		break;
	case SDLK_RALT:			return "right_alt";		break;
	case SDLK_LALT:			return "left_alt";		break;
	case SDLK_RMETA:		return "right_meta";	break;
	case SDLK_LMETA:		return "left_meta";		break;
	case SDLK_LSUPER:		return "left_windows";	break;
	case SDLK_RSUPER:		return "right_windows";	break;
	case SDLK_MODE:			return "mode_shift";	break;
	case SDLK_HELP:			return "help";			break;
	case SDLK_PRINT:		return "print-screen";	break;
	case SDLK_SYSREQ:		return "SysRq";			break;
	case SDLK_BREAK:		return "break";			break;
	case SDLK_MENU:			return "menu";			break;
	case SDLK_POWER:		return "power";			break;
	case SDLK_EURO:			return "euro";			break;

	default:				return "INCONNU";		break;
	}
}

bool CCfg::CAudio::Init()
{
	TRACE().p( TRACE_SON, "CCfg::CAudio::Init()" );
	unsigned int caps = 0;

		// Initialisation audio de FMOD
	cout << "\nVersion de FMOD\t\t" << FSOUND_GetVersion() << endl;
	if( FSOUND_GetVersion() < FMOD_VERSION )
	{
		cout << "Error : You are using the wrong DLL version!  You should be using FMOD" <<
			FMOD_VERSION << endl;
		return false;
	}

	FSOUND_SetOutput( m_Output );		// Sélection de l'output

	remarquesDriver = new char*[FSOUND_GetNumDrivers()];
	string rem;
	for( int i=0 ; i<FSOUND_GetNumDrivers() ; i++)
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

	if( FSOUND_GetNumDrivers!=0 )	// Vérifie s'il y a un driver pour le son
	{
		if( m_Driver<FSOUND_GetNumDrivers() )
		{
			FSOUND_SetDriver( m_Driver );	// Sélection du driver configuré
		}
		else	// Le driver configuré ne peut pas fonctionner
		{
			cerr << endl << "Driver son mal configuré, choix par défaut.";
			m_Driver = 0;	// Modification du fichier de configuration
			FSOUND_SetDriver( 0 );						// Choix du driver par défaut
		}
	}
	else
	{
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

	if( FSOUND_Record_GetNumDrivers()!=0 )	// Vérifie s'il y a un driver d'acquisition du son
	{
		if( m_DriverRecord<FSOUND_Record_GetNumDrivers() )
		{
			if( !FSOUND_Record_SetDriver(m_DriverRecord))	// Sélection du driver
			{															// d'entrée pour le micro
				cerr << "Erreur FSOUND_Record_SetDriver : " << FMOD_ErrorString(FSOUND_GetError()) << endl;
				cerr << "\tAVIS AU PROGRAMMEUR" << endl
					<< "Le programme va planter s'il y a une tentative d'utilisation du micro !!!" << endl;
		/*		FSOUND_Close();
				return -1;*/
			}
		}
		else	// Le driver de record ne peut pas fonctionner
		{
			cerr << endl << "Driver de record son mal configuré, choix par défaut.";
			m_DriverRecord = 0;	// Modification de la config => choix par défaut
			if( !FSOUND_Record_SetDriver(0) )	// Sélection du driver de record par défaut
			{
				cerr << "Erreur FSOUND_Record_SetDriver : " << FMOD_ErrorString(FSOUND_GetError()) << endl;
				cerr << "\tAVIS AU PROGRAMMEUR" << endl
					<< "Le programme va planter s'il y a une tentative d'utilisation du micro !!!" << endl;
		/*		FSOUND_Close();
				return -1;*/
			}
		}
	}
	else
	{		// Aucun driver d'acquisition du son détecté
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

	cout << "FSOUND Record Driver : " <<
		resolveDriverRecord(FSOUND_Record_GetDriver()) << endl;

	int num2d, num3d, total;
	FSOUND_GetNumHWChannels( &num2d, &num3d, &total );

	cout << "Hardware 2D channels : " << num2d << endl;
	cout << "Hardware 3D channels : " << num3d << endl;
	cout << "Total hardware channels : " << total << endl;

	return true;
}

void CCfg::CDisplay::Init()
{
	InitSDL();
	InitOpenGL();
    InitAgar();
}

void CCfg::CDisplay::InitSDL()
{
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

    const SDL_VideoInfo* info = SDL_GetVideoInfo( );	// Let's get some video information
    if( !info )
	{
		cerr << endl << "Error : Video query failed: " << SDL_GetError( ) << endl;
        exit( 1 );
    }

    bpp = info->vfmt->BitsPerPixel;

    if( SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 )==-1 )
	{
		cerr << "Erreur init_SDL : SDL_GL_RED_SIZE\n";
		exit( 1 );
	}
    if( SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 )==-1 )
	{
		cerr << "Erreur init_SDL : SDL_GL_GREEN_SIZE\n";
		exit( 1 );
	}
    if( SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 )==-1 )
	{
		cerr << "Erreur init_SDL : SDL_GL_BLUE_SIZE\n";
		exit( 1 );
	}
	if( SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 )==-1 )
	{
		cerr << "Erreur init_SDL : SDL_GL_ALPHE_SIZE\n";
		exit( 1 );
	}
    if( SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 )==-1 )
	{
		cerr << "Erreur init_SDL : SDL_GL_DEPTH_SIZE\n";
			exit( 1 );
	}
    if( SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 )==-1 )
	{
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
	if( modes == (SDL_Rect **)0 )
	{
		cout << endl << "Aucun mode video n'est disponible" << endl;
		exit( 1 );
	}

	/* Check if our resolution is restricted */
	if(modes == (SDL_Rect **)-1)
	{
		cout << endl << "Tous les modes video sont disponibles" << endl;
	}
	else
	{
	/* Print valid modes */
		cout << endl << "Modes video disponibles : " << endl;

		for( int i=0 ; modes[i] ; ++i )
			cout << "\t" << modes[i]->w << " x " << modes[i]->h << endl;
	}

	if( !SDL_VideoModeOK(X, Y, bpp, flags) )
	{
		cout << endl << "WARNING : Mode video indisponible =>";
		cout << "mode choisi par defaut, corrigez la configuration" << endl;
		X = modes[0]->w;
		Y = modes[0]->h;
	}

	screen = SDL_SetVideoMode(X, Y, bpp, flags);		// Set the video mode
	if( screen == 0)
	{
		cerr << endl << "Error : Video mode set failed: " << SDL_GetError() << endl;
		exit( 1 );
    }

	SDL_WarpMouse( 250, 250 );			//positionnement du curseur de la souris
	SDL_ShowCursor( SDL_DISABLE );		//Cache le curseur de la souris
	SDL_WM_GrabInput( SDL_GRAB_ON );	//Active le mode déplacement relatif de la souris

	SDL_EnableKeyRepeat( 500, 500 );	// Répétition des touches clavier

	SDL_WM_SetCaption( "JKT 2010", "C'est un jeu qui tue !!!" );	// Titre et icon de la fenêtre
	
	string iconeFichier = "@Icone/Icone.bmp";
	JKT_PACKAGE_UTILS::RessourcesLoader::getFileRessource(iconeFichier);
	SDL_WM_SetIcon( IMG_Load( iconeFichier.c_str() ), 0 );

	char txt1[50];
	cout << "\nDriver video :\t\t";
	if( SDL_VideoDriverName( txt1, 50 ) )
		cout << txt1;

	cout << "\nVideo memory :\t\t" << info->video_mem/1024 << " Mo" << endl;

TRACE().p( TRACE_OTHER, "init_SDL(config) end" );
}

void CCfg::CDisplay::InitOpenGL()
{
TRACE().p( TRACE_OTHER, "setup_opengl(width=%d,height=%d) begin", X, Y );
		// Informations openGL
	cout << "Version openGL :\t" << glGetString( GL_VERSION );
	cout << "\nModele de la carte graphique :\t" << glGetString( GL_RENDERER );
	cout << "\nFabricant de la carte graphique :\t" << glGetString( GL_VENDOR );
	cout << "\nExtensions openGL disponibles :\t" << glGetString( GL_EXTENSIONS );
	cout << "\nVersion GLU :\t\t" << gluGetString( GLU_VERSION );

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

	if(!glGenBuffers)
		exit(1);
	if(!glBindBuffer)
		exit(1);
	if(!glBufferData)
		exit(1);

TRACE().p( TRACE_OTHER, "setup_opengl() end" );
}

void CCfg::CDisplay::InitAgar()
{
TRACE().p( TRACE_OTHER, "setup Agar" );
		// Informations Agar
    AG_AgarVersion agarVersion;
    AG_GetVersion(&agarVersion);

    cout << "\nVersion Agar : [major=" << agarVersion.major;
    cout << ", minor=" << agarVersion.minor;
    cout << ", patch=" << agarVersion.patch;
    cout << ", release=" << agarVersion.release << "]";

	// Configuration Agar (librairie de gestion du menu)
	if(AG_InitCore("event1", 0) == -1 || AG_InitVideoSDL(screen, flags) == -1)
	{
		cerr << "\nERREUR d'initialisation Agar";
	}
}

bool CCfg::CDisplay::chargeGLExtension(const char* ext, string& extensions) {
		// Recherche de l'extension qui nous intéresse : GL_ARB_texture_compression
	if (extensions.find(ext) != std::string::npos)
	{
		cout << endl << "Extension supportée : " << ext ;
		return true;
	}
	else
	{
		cout << endl << "Extension non-supportée : " << ext ;
		return false;
	}
}

void CCfg::CDisplay::ChangeVideoSize(int x, int y)
{
TRACE().p( TRACE_OTHER, "changeVideoSize(config) begin" );
	if( !SDL_VideoModeOK(x, y, bpp, flags) ) {
		cout << "\nMode video invalide :\t\t" << endl;
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

void CCfg::CReseau::Init()
{
		if( SDLNet_Init() == -1 )
	{
TRACE().p( TRACE_ERROR, "SDLNet_Init() failed : %s", SDLNet_GetError() );
		cerr << endl << "Error : SDLNet_Init : %¨s\n" << SDLNet_GetError();
		exit( 1 );
	}
}
