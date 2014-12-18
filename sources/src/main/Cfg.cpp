
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


const char* CCfg::CST_GEN_PLAY_INTRO = 			"general.playIntro";

const char* CCfg::CST_VID_DISPLAY = 			"video.display";
const char* CCfg::CST_VID_FULLSCREEN =			"video.fullscreen";

const char* CCfg::CST_AUD_OUTPUT = 				"audio.output";
const char* CCfg::CST_AUD_MIXER = 				"audio.mixer";
const char* CCfg::CST_AUD_DRIVER = 				"audio.driver";
const char* CCfg::CST_AUD_DRIVERRECORD = 		"audio.driverRecord";

const char* CCfg::CST_COM_AVANCER =				"commande.avancer";
const char* CCfg::CST_COM_RECULER =				"commande.reculer";
const char* CCfg::CST_COM_GAUCHE =				"commande.gauche";
const char* CCfg::CST_COM_DROITE =				"commande.droite";
const char* CCfg::CST_COM_TIR1 =				"commande.tir1";
const char* CCfg::CST_COM_TIR2 =				"commande.tir2";
const char* CCfg::CST_COM_MONTER =				"commande.monter";
const char* CCfg::CST_COM_SELECTWEAPONUP =		"commande.selectWeaponUp";
const char* CCfg::CST_COM_SELECTWEAPONDOWN =	"commande.selectWeaponDown";

const char* CCfg::CST_CEN_IP =					"centralisateur.ip";
const char* CCfg::CST_CEN_PORT =				"centralisateur.port";

const char* CCfg::CST_NET_SERVEUR =				"reseau.serveur";
const char* CCfg::CST_NET_IP =					"reseau.serveur.ip";
const char* CCfg::CST_NET_PORT =				"reseau.serveur.port";
const char* CCfg::CST_NET_PORTTREE =			"reseau.serveur.portTree";

const char* CCfg::CST_JOU_NOM =					"joueur.nom";
const char* CCfg::CST_JOU_MAPNOM =				"joueur.mapNom";
const char* CCfg::CST_JOU_OUTLINEVISIBILITY =	"joueur.outlineVisibility";
const char* CCfg::CST_JOU_SKINVISIBILITY =		"joueur.skinVisibility";

const char* CCfg::CST_PLU_ACT_BEGIN =			"plugin.activate.begin";
const char* CCfg::CST_PLU_ACT_END =				"plugin.activate.end";

const char* CCfg::CST_DEB_SONPERFORMANCES =		"debug.sonPerformances";
const char* CCfg::CST_DEB_SONSPECTRE =			"debug.sonSpectre";
const char* CCfg::CST_DEB_AFFICHEFICHIER =		"debug.afficheFichier";
const char* CCfg::CST_DEB_AFFICHENORMAUX =		"debug.afficheNormaux";


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
	fichier.exceptions(ifstream::eofbit);

	if(!fichier) {
		quit_game("Fichier de configuration introuvable '" + nomFichierEntier + "'", 1);
	}

	try {

		/* ***************************************
		 * Générale
		 * **************************************/

		do fichier >> mot;	while(mot!=CST_GEN_PLAY_INTRO);		// Indique si l'intro du jeu doit être lancée au démarrage
		fichier >> General._playIntro;


		/* ***************************************
		 * Vidéo
		 * **************************************/

		do fichier >> mot;	while( mot!=CST_VID_DISPLAY );		// Propriétés d'affichage
		fichier >> Display.X >> Display.Y;

		do fichier >> mot;	while( mot!=CST_VID_FULLSCREEN );	// Propriétés d'affichage
		fichier >> Display.m_bFullscreen;


		// Audio
		do fichier >> mot;	while( mot!=CST_AUD_OUTPUT );
		fichier >> Audio.m_Output;

		do fichier >> mot;	while( mot!=CST_AUD_MIXER );
		fichier >> Audio.m_Mixer;

		do fichier >> mot;	while( mot!=CST_AUD_DRIVER );
		fichier >> Audio.m_Driver;

		do fichier >> mot; while( mot!=CST_AUD_DRIVERRECORD );
		fichier >> Audio.m_DriverRecord;


		/* ***************************************
		 * Commandes
		 * **************************************/

		int crotte;

		do fichier >> mot;	while( mot!=CST_COM_AVANCER );
		fichier >> crotte;
		Commandes.Avancer.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Avancer.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_RECULER );
		fichier >> crotte;
		Commandes.Reculer.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Reculer.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_GAUCHE );
		fichier >> crotte;
		Commandes.Gauche.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Gauche.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_DROITE );
		fichier >> crotte;
		Commandes.Droite.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Droite.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_TIR1 );
		fichier >> crotte;
		Commandes.Tir1.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Tir1.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_TIR2 );
		fichier >> crotte;
		Commandes.Tir2.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Tir2.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_MONTER );
		fichier >> crotte;
		Commandes.Monter.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Monter.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_SELECTWEAPONUP );
		fichier >> crotte;
		Commandes.SelectWeaponUp.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.SelectWeaponUp.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_SELECTWEAPONDOWN );
		fichier >> crotte;
		Commandes.SelectWeaponDown.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.SelectWeaponDown.mouse = crotte;


		/* ***************************************
		 * Centralisateur
		 * **************************************/

		do fichier >> mot;	while( mot!=CST_CEN_IP );
		fichier >> Centralisateur.m_IpServer;

		do fichier >> mot;	while( mot!=CST_CEN_PORT );
		fichier >> Centralisateur.m_Port;


		/* ***************************************
		 * Réseau
		 * **************************************/

		do fichier >> mot;	while( mot!=CST_NET_SERVEUR );
		fichier >> Reseau._serveur;

		do fichier >> mot;	while( mot!=CST_NET_IP );
		fichier >> Reseau._IpServer;

		do fichier >> mot;	while( mot!=CST_NET_PORT );
		fichier >> Reseau._Port;

		do fichier >> mot;	while( mot!=CST_NET_PORTTREE );
		fichier >> Reseau._portTree;


		/* ***************************************
		 * Joueur
		 * **************************************/

		do fichier >> mot;	while( mot!=CST_JOU_NOM );
		fichier >> Joueur.nom;

		do fichier >> mot;	while( mot!=CST_JOU_MAPNOM );
		fichier >> Joueur.mapName;

		do fichier >> mot;	while( mot!=CST_JOU_OUTLINEVISIBILITY );
		fichier >> Joueur.outlineVisibility;

		do fichier >> mot;	while( mot!=CST_JOU_SKINVISIBILITY );
		fichier >> Joueur.skinVisibility;

		/* ***************************************
		 * Plugin
		 * **************************************/

		do fichier >> mot;	while( mot!=CST_PLU_ACT_BEGIN );

		do {
			fichier >> mot;
			if(mot != CST_PLU_ACT_END) {
				Plugin._defaultPluging.push_back(mot);
			}
			else {
				break;
			}
		}
		while(true);

		/* ***************************************
		 * Debug
		 * **************************************/

		do fichier >> mot;	while( mot!=CST_DEB_SONPERFORMANCES );
		fichier >> Debug.bSonPerformances;

		do fichier >> mot;	while( mot!=CST_DEB_SONSPECTRE );
		fichier >> Debug.bSonSpectre;

		do fichier >> mot;	while( mot!=CST_DEB_AFFICHEFICHIER );
		fichier >> Debug.bAfficheFichier;

		do fichier >> mot;	while( mot!=CST_DEB_AFFICHENORMAUX );
		fichier >> Debug.bAfficheNormaux;

		fichier.close();
	}
	catch(istringstream::failure& finDuFlux) {
		quit_game("Fichier de configuration corrompu '" + nomFichierEntier + "'", 1);
	}
}

void CCfg::Ecrit() {
	string nomFichierEntier = "./" + nomFichierConfig + ".ini";
	ofstream fichier( nomFichierEntier.c_str() );

	fichier << "\n-------------------------GENERAL-------------------------\n";
	fichier << endl << CST_GEN_PLAY_INTRO << "\t\t" << General._playIntro;

	fichier << "\n\n\n-------------------------VIDEO-------------------------\n";
	fichier << endl << CST_VID_DISPLAY << "\t\t" << Display.X << "\t" << Display.Y;
	fichier << endl << CST_VID_FULLSCREEN << "\t" << Display.Fullscreen();

	fichier << "\n\n\n-------------------------AUDIO-------------------------\n";
	fichier << endl << CST_AUD_OUTPUT << "\t\t" << Audio.m_Output << "\t(" << resolveOutput(Audio.m_Output) << ")";
	fichier << endl << CST_AUD_MIXER << "\t\t\t" << Audio.m_Mixer << "\t(" << resolveMixer(Audio.m_Mixer) << ")";
	fichier << endl << CST_AUD_DRIVER << "\t\t" << Audio.m_Driver << "\t(" << resolveDriver(Audio.m_Driver) << ")";

	if( Audio.m_DriverRecord!=-1 ) {
		fichier << endl << CST_AUD_DRIVERRECORD << "\t" << Audio.m_DriverRecord << "\t(" << resolveDriverRecord(Audio.m_DriverRecord) << ")";
	}
	else {
		fichier << endl << CST_AUD_DRIVERRECORD << "\t" << Audio.m_DriverRecord << "\t(Pas de driver d'acquisition)";
	}

	fichier << "\n\n\n-----------------------COMMANDES-----------------------\n";
	fichier << endl << CST_COM_AVANCER << "\t\t\t" << Commandes.Avancer.key << "\t" << Commandes.Avancer.mouse << "\t(" << Commandes.resolve(Commandes.Avancer) << ")";
	fichier << endl << CST_COM_RECULER << "\t\t\t" << Commandes.Reculer.key << "\t" << Commandes.Reculer.mouse << "\t(" << Commandes.resolve(Commandes.Reculer) << ")";
	fichier << endl << CST_COM_GAUCHE << "\t\t\t\t" << Commandes.Gauche.key << "\t" << Commandes.Gauche.mouse << "\t(" << Commandes.resolve(Commandes.Gauche) << ")";
	fichier << endl << CST_COM_DROITE << "\t\t\t\t" << Commandes.Droite.key << "\t" << Commandes.Droite.mouse << "\t(" << Commandes.resolve(Commandes.Droite) << ")";
	fichier << endl << CST_COM_TIR1 << "\t\t\t\t" << Commandes.Tir1.key << "\t" << Commandes.Tir1.mouse << "\t(" << Commandes.resolve(Commandes.Tir1) << ")";
	fichier << endl << CST_COM_TIR2 << "\t\t\t\t" << Commandes.Tir2.key << "\t" << Commandes.Tir2.mouse << "\t(" << Commandes.resolve(Commandes.Tir2) << ")";
	fichier << endl << CST_COM_MONTER << "\t\t\t\t" << Commandes.Monter.key << "\t" << Commandes.Monter.mouse << "\t(" << Commandes.resolve(Commandes.Monter) << ")";
	fichier << endl << CST_COM_SELECTWEAPONUP << "\t\t" << Commandes.SelectWeaponUp.key << "\t" << Commandes.SelectWeaponUp.mouse << "\t(" << Commandes.resolve(Commandes.SelectWeaponUp) << ")";
	fichier << endl << CST_COM_SELECTWEAPONDOWN << "\t" << Commandes.SelectWeaponDown.key << "\t" << Commandes.SelectWeaponDown.mouse << "\t(" << Commandes.resolve(Commandes.SelectWeaponDown) << ")";

	fichier << "\n\n\n--------------------CENTRALISATEUR---------------------\n";
	fichier << endl << CST_CEN_IP << "\t\t" << Centralisateur.m_IpServer;
	fichier << endl << CST_CEN_PORT << "\t\t" << Centralisateur.m_Port;

	fichier << "\n\n\n------------------------RESEAU-------------------------\n";
	fichier << endl << CST_NET_SERVEUR << "\t\t" << Reseau._serveur;
	fichier << endl << CST_NET_IP << "\t\t\t" << Reseau._IpServer;
	fichier << endl << CST_NET_PORT << "\t\t\t" << Reseau._Port;
	fichier << endl << CST_NET_PORTTREE << "\t\t" << Reseau._portTree;

	fichier << "\n\n\n------------------------JOUEUR-------------------------\n";
	fichier << endl << CST_JOU_NOM << "\t\t\t\t\t" << Joueur.nom;
	fichier << endl << CST_JOU_MAPNOM << "\t\t\t\t" << Joueur.mapName;
	fichier << endl << CST_JOU_OUTLINEVISIBILITY << "\t" << Joueur.outlineVisibility;
	fichier << endl << CST_JOU_SKINVISIBILITY << "\t\t" << Joueur.skinVisibility;

	fichier << "\n\n\n------------------------PLUGIN-------------------------\n";
	vector<string>::iterator iter;
	fichier << CST_PLU_ACT_BEGIN << endl;
	for(iter = Plugin._defaultPluging.begin() ; iter != Plugin._defaultPluging.end() ; iter++) {
		fichier << *iter << endl;
	}
	fichier << CST_PLU_ACT_END;

	fichier << "\n\n\n------------------------DEBUG-------------------------\n";
	fichier << endl << CST_DEB_SONPERFORMANCES << "\t\t" << Debug.bSonPerformances;
	fichier << endl << CST_DEB_SONSPECTRE << "\t\t\t" << Debug.bSonSpectre;
	fichier << endl << CST_DEB_AFFICHEFICHIER << "\t\t" << Debug.bAfficheFichier;
	fichier << endl << CST_DEB_AFFICHENORMAUX << "\t\t" << Debug.bAfficheNormaux;

	fichier << "\n\n\n------------------------FIN-------------------------\n";

	fichier.close();
}

void CCfg::resolve(const SDL_Event *event, string& description) {
	Uint8 type = event->type;

	switch(type) {
	case SDL_NOEVENT: 			description.append("SDL_NOEVENT");			break;
	case SDL_ACTIVEEVENT:		description.append("SDL_ACTIVEEVENT");		break;
	case SDL_KEYDOWN: 			description.append("SDL_KEYDOWN");			break;
	case SDL_KEYUP: 			description.append("SDL_KEYUP");			break;
	case SDL_MOUSEMOTION: 		description.append("SDL_MOUSEMOTION");		break;
	case SDL_MOUSEBUTTONDOWN: 	description.append("SDL_MOUSEBUTTONDOWN");	break;
	case SDL_MOUSEBUTTONUP: 	description.append("SDL_MOUSEBUTTONUP");	break;
	case SDL_JOYAXISMOTION: 	description.append("SDL_JOYAXISMOTION");	break;
	case SDL_JOYBALLMOTION: 	description.append("SDL_JOYBALLMOTION");	break;
	case SDL_JOYHATMOTION: 		description.append("SDL_JOYHATMOTION");		break;
	case SDL_JOYBUTTONDOWN: 	description.append("SDL_JOYBUTTONDOWN");	break;
	case SDL_JOYBUTTONUP: 		description.append("SDL_JOYBUTTONUP");		break;
	case SDL_QUIT: 				description.append("SDL_QUIT");				break;
	case SDL_SYSWMEVENT: 		description.append("SDL_SYSWMEVENT");		break;
	case SDL_EVENT_RESERVEDA: 	description.append("SDL_EVENT_RESERVEDA");	break;
	case SDL_EVENT_RESERVEDB: 	description.append("SDL_EVENT_RESERVEDB");	break;
	case SDL_VIDEORESIZE: 		description.append("SDL_VIDEORESIZE");		break;
	case SDL_VIDEOEXPOSE: 		description.append("SDL_VIDEOEXPOSE");		break;
	case SDL_EVENT_RESERVED2: 	description.append("SDL_EVENT_RESERVED2");	break;
	case SDL_EVENT_RESERVED3: 	description.append("SDL_EVENT_RESERVED3");	break;
	case SDL_EVENT_RESERVED4: 	description.append("SDL_EVENT_RESERVED4");	break;
	case SDL_EVENT_RESERVED5: 	description.append("SDL_EVENT_RESERVED5");	break;
	case SDL_EVENT_RESERVED6: 	description.append("SDL_EVENT_RESERVED6");	break;
	case SDL_EVENT_RESERVED7: 	description.append("SDL_EVENT_RESERVED7");	break;
	case SDL_USEREVENT: 		description.append("SDL_USEREVENT");		break;
	case SDL_NUMEVENTS: 		description.append("SDL_NUMEVENTS");		break;
	default:
		stringstream str;
		str << "Event_type_inconnu(" << type << ")";
		description.append(str.str());
		break;
	}

	switch(type) {
	case SDL_KEYUP:
	case SDL_KEYDOWN:
		description.append(" (").append(resolve(event->key.keysym.sym)).append(")");
		break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		description.append(" (").append(resolve(event->button.button)).append(")");
		break;
	}
}

const char* CCfg::CCommandes::resolve(const CComID com) {
	if(com.key != 0)
		return CCfg::resolve( com.key );
	else
		return CCfg::resolve( com.mouse );
}

const char* CCfg::resolve(const Uint8 mouse) {
	switch(mouse)
	{
	case 1:		return "Souris_gauche";
	case 2:		return "Souris_centre";
	case 3:		return "Souris_droit";
	case 4:		return "Souris_centre_down";
	case 5:		return "Souris_centre_up";
	default:
		stringstream str;
		str << "Souris_inconnu(" << mouse << ")";
		return str.str().c_str();
	}
}

const char* CCfg::resolve(const SDLKey sym) {
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

	default:
		stringstream str;
		str << "Key_inconnu(" << sym << ")";
		result = str.str().c_str();
		break;
	}

	return result;
}

const int CCfg::CAudio::AVAILABLE_AUDIO_OUTPUTS_NBR = 4;

const char *CCfg::CAudio::AVAILABLE_AUDIO_OUTPUTS[] =
	{
	#ifdef _WIN32
		"Direct Sound",
		"Windows Multimedia Waveout",
		"ASIO",
		"NoSound",
	#elif defined(__linux__)
		"OSS - Open Sound System",
		"ESD - Elightment Sound Daemon",
		"ALSA 0.9 - Advanced Linux Sound Architecture",
		"NoSound",
	#endif
	};

/**
 * Initialise the audio drivers with the Jkt configuration parameters.
 */
void CCfg::CAudio::Init() {
	testInitAndSaveConfiguration(m_Driver, m_Output, m_Mixer, m_DriverRecord);
}

/**
 * Try to initialise the audio drivers with new parameters and save them in the configuration if they work correctly.
 */
bool CCfg::CAudio::testInitAndSaveConfiguration(int driver, int output, int mixer, int driverRecord) {
	TRACE().p( TRACE_SON, "CCfg::CAudio::Init()" );
	unsigned int caps = 0;

	// Initialisation audio de FMOD
	cout << "\nVersion de FMOD\t\t" << FSOUND_GetVersion() << endl;

	if(FSOUND_GetVersion() < FMOD_VERSION) {
		cout << "Error : You are using a wrong FMOD DLL version!  You should be using at least version " << FMOD_VERSION << endl;

		return false;
	}

	FSOUND_SetOutput( output );		// Sélection de l'output
	m_Output = output;

	int numDrivers = FSOUND_GetNumDrivers();
	remarquesDriver = new char*[numDrivers];
	string rem;

	for(int i = 0 ; i < numDrivers ; i++) {
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

		remarquesDriver[i] = new char[rem.size() + 1];
		strcpy( remarquesDriver[i], rem.c_str() );
	}

	if(numDrivers > 0) {	// Vérifie s'il y a un driver pour le son
		if(driver < numDrivers ) {
			FSOUND_SetDriver(driver);			// Sélection du driver configuré
			m_Driver = driver;
		}
		else {									// Le driver configuré ne peut pas fonctionner
			cerr << endl << __FILE__ << ":" << __LINE__ << " Driver son mal configuré, choix par défaut.";
			driver = 0;							// Modification du fichier de configuration
			FSOUND_SetDriver(0);				// Choix du driver par défaut
			m_Driver = 0;
		}
	}
	else {
		// Pas de driver son
		cerr << endl << __FILE__ << ":" << __LINE__ << " Aucun driver son détecté.";
	}

	FSOUND_SetMixer(mixer);			// Sélection du mixer
	m_Mixer = mixer;

	// Initialisation
	if(!FSOUND_Init(44100, 32, FSOUND_INIT_GLOBALFOCUS)) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur FSOUND_Init : "<< FMOD_ErrorString(FSOUND_GetError()) << endl;
		FSOUND_Close();
	}

	FSOUND_DSP_SetActive( FSOUND_DSP_GetFFTUnit(), true );

	if( FSOUND_Record_GetNumDrivers() > 0 ) {	// Vérifie s'il y a un driver d'acquisition du son
		if( driverRecord < FSOUND_Record_GetNumDrivers() ) {
			if( !FSOUND_Record_SetDriver(driverRecord)) {	// Sélection du driver				// d'entrée pour le micro
				cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur FSOUND_Record_SetDriver : " << FMOD_ErrorString(FSOUND_GetError()) << endl;
				cerr << endl << __FILE__ << ":" << __LINE__ << "\tAVIS AU PROGRAMMEUR" << endl
						<< "Le programme va planter s'il y a une tentative d'utilisation du micro !!!" << endl;
			}
			else {
				m_DriverRecord = driverRecord;
			}
		}
		else {		// Le driver de record ne peut pas fonctionner
			cerr << endl << __FILE__ << ":" << __LINE__ << "Driver de record son mal configuré, choix par défaut.";

			if( !FSOUND_Record_SetDriver(0) ) {		// Sélection du driver de record par défaut
				cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur FSOUND_Record_SetDriver : " << FMOD_ErrorString(FSOUND_GetError()) << endl;
				cerr << endl << __FILE__ << ":" << __LINE__ << "\tAVIS AU PROGRAMMEUR" << endl
						<< "Le programme va planter s'il y a une tentative d'utilisation du micro !!!" << endl;
			}
			else {
				m_DriverRecord = 0;
			}
		}
	}
	else {		// Aucun driver d'acquisition du son détecté
		cerr << endl << __FILE__ << ":" << __LINE__ << " Aucun driver d'acquisition du son détecté.";
	}

	// RESUME DE L'INITIALISATION
	cout << "\n\tRESUME DE L'INITIALISATION AUDIO\n";
	cout << "FSOUND Output Method : " << resolveOutput(FSOUND_GetOutput()) << endl;
	cout << "FSOUND Mixer         : " << resolveMixer(FSOUND_GetMixer()) << endl;
	cout << "FSOUND Driver        : " << resolveDriver(FSOUND_GetDriver()) << endl;

	FSOUND_GetDriverCaps(FSOUND_GetDriver(), &caps);

	if (!caps) {
		cout << "\tCe driver supporte seulement le mode software.\n\tIl ne supporte pas bien le son 3D.\n";
	}

	if (caps & FSOUND_CAPS_HARDWARE) {
		cout << "\t- Ce driver supporte le son 3D en hardware!\n";
	}

	if (caps & FSOUND_CAPS_EAX2) {
		cout << "\t- Ce driver supporte l'EAX 2 reverb!\n";
	}

	if (caps & FSOUND_CAPS_EAX3) {
		cout << "\t- Ce driver supporte l'EAX 3 reverb!\n";
	}

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
	//	_putenv("SDL_VIDEODRIVER=directx");	// A FAIRE A FAIRE A FAIRE : code compatible Linux
#endif

	if( SDL_Init( SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_NOPARACHUTE ) < 0 )	// First, initialize SDL's video subsystem
	{
		TRACE().p( TRACE_ERROR, "SDL_Init() failed : %s", SDLNet_GetError() );
		cerr << endl << __FILE__ << ":" << __LINE__ << " Error : Video initialization failed: " << SDL_GetError( ) << endl;
		exit( 1 );
	}

	// Icone et titre
	SDL_WM_SetCaption( "JKT 2010", "C'est un jeu qui tue !!!" );	// Titre et icon de la fenêtre

	string iconeFichier = "@Icone/Icone.bmp";
	JktUtils::RessourcesLoader::getFileRessource(iconeFichier);
	SDL_WM_SetIcon( IMG_Load( iconeFichier.c_str() ), 0 );


	const SDL_VideoInfo* info = SDL_GetVideoInfo( );	// Let's get some video information
	if( !info ) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Error : Video query failed: " << SDL_GetError( ) << endl;
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

	cout << endl << "hw_available=" << info->hw_available;
	cout << " ; wm_available=" << info->wm_available;
	cout << " ; blit_hw:1=" << info->blit_hw;
	cout << " ; blit_hw_CC:1=" << info->blit_hw_CC;
	cout << " ; blit_hw_A:1=" << info->blit_hw_A;
	cout << " ; blit_sw:1=" << info->blit_sw;
	cout << " ; blit_sw_CC:1=" << info->blit_sw_CC;
	cout << " ; blit_sw_A:1=" << info->blit_sw_A;
	cout << " ; blit_fill=" << info->blit_fill;
	cout << " ; video_mem=" << info->video_mem << endl;

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

	putenv("SDL_VIDEO_WINDOW_POS=center"); 	//pour centrer la fenêtre

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

	SDL_EnableUNICODE(SDL_ENABLE);

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

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );					// Spécifie la couleur de vidage du tampon chromatique
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glShadeModel( GL_SMOOTH );								// Mode dégradé pour le remplissage des polynomes
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glViewport( 0, 0, X, Y );

	// Récupération de la liste des extensions
	std::string extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));

	// Chargement des extensions
	if(!chargeGLExtension("GL_ARB_vertex_buffer_object",extensions))
		exit(1);

	glGenBuffers = (PFNGLGENBUFFERSARBPROC)SDL_GL_GetProcAddress("glGenBuffersARB");
	glBindBuffer = (PFNGLBINDBUFFERARBPROC)SDL_GL_GetProcAddress("glBindBufferARB");
	glBufferData = (PFNGLBUFFERDATAARBPROC)SDL_GL_GetProcAddress("glBufferDataARB");
	glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)SDL_GL_GetProcAddress("glDeleteBuffersARB");

	if(!glGenBuffers) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Error : OpenGL extension 'glGenBuffersARB' not available\n";
		exit(1);
	}

	if(!glBindBuffer) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Error : OpenGL extension 'glBindBufferARB' not available\n";
		exit(1);
	}

	if(!glBufferData) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Error : OpenGL extension 'glBufferDataARB' not available\n";
		exit(1);
	}

	if(!glDeleteBuffers) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Error : OpenGL extension 'glDeleteBuffersARB' not available\n";
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

	cout << endl << "Version Agar : " << agarVersion.major << "." << agarVersion.minor << "." << agarVersion.patch << " [" << agarVersion.release << "]";

	// Initialisation Agar (librairie de gestion du menu)
	if(AG_InitCore("JKT", 0) == -1 || AG_InitVideoSDL(screen, flags) == -1) {
		cerr << "\nERREUR d'initialisation Agar : '" << AG_GetError() << "'";
	}

	// Noms des drivers disponibles pour Agar
	char drvNames[256];
	AG_ListDriverNames(drvNames, sizeof(drvNames));

	cout << endl << "Available drivers : " << drvNames;

	AG_Driver *driver = (AG_Driver *)agDriverSw;
	cout << endl << "Driver Agar actif : " << driver->_inherit.name;
}

bool CCfg::CDisplay::chargeGLExtension(const char* ext, string& extensions) {
	// Recherche de l'extension qui nous intéresse : GL_ARB_texture_compression
	if (extensions.find(ext) != std::string::npos) {
		cout << endl << "Extension OpenGL supportée : " << ext ;
		return true;
	}
	else {
		cerr << endl << "Extension OpenGL non-supportée : " << ext ;
		return false;
	}
}

void CCfg::CDisplay::ChangeVideoSize(int x, int y) {
	TRACE().p( TRACE_OTHER, "changeVideoSize(config) begin" );
	if( !SDL_VideoModeOK(x, y, bpp, flags) ) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Mode video invalide :\t\t" << endl;
		return;
	}

	if( screen )
		SDL_FreeSurface( screen );							// Vide la surface

	screen = SDL_SetVideoMode( x, y, bpp, flags );	// Set the video mode
	if(screen == 0)
	{
		cerr << endl << __FILE__ << ":" << __LINE__ << " Video mode set failed: " << SDL_GetError() << endl;
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
		cerr << endl << __FILE__ << ":" << __LINE__ << " Error : SDLNet_Init : %¨s\n" << SDLNet_GetError();
		exit( 1 );
	}
}
