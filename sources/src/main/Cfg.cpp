
#include <string>
#include <fstream>

#include <GL/glew.h>

#include <agar/config/have_opengl.h>
#include <agar/config/have_sdl.h>
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/opengl.h>

#include <boost/filesystem/operations.hpp> // includes boost/filesystem/path.hpp
#include <boost/filesystem/fstream.hpp>

#include <SDL.h>
#include <SDL_net.h>
#include <SDL_image.h>


#include <fmod.h>
#include <fmod_errors.h>

#include "main/divers.h"
#include "util/StringUtils.h"
#include "util/Trace.h"
#include "util/StringUtils.h"
#include "son/audio.h"
#include "ressource/RessourcesLoader.h"


#include "main/Cfg.h"

using namespace std;

char **remarquesDriver;
int bpp, flags;
AG_Window* agarWindows;

SDL_Surface *screen;

const char* CCfg::CST_GEN_PLAY_INTRO = 			"general.playIntro";

const char* CCfg::CST_VID_DISPLAY_POS =			"video.display.position";
const char* CCfg::CST_VID_DISPLAY_SIZE = 		"video.display.size";
const char* CCfg::CST_VID_FULLSCREEN =			"video.fullscreen";

const char* CCfg::CST_AUD_OUTPUT = 				"audio.output";
const char* CCfg::CST_AUD_MIXER = 				"audio.mixer";
const char* CCfg::CST_AUD_DRIVER = 				"audio.driver";
const char* CCfg::CST_AUD_DRIVERRECORD = 		"audio.driverRecord";

const char* CCfg::CST_COM_AVANCER =				"commande.avancer";
const char* CCfg::CST_COM_RECULER =				"commande.reculer";
const char* CCfg::CST_COM_GAUCHE =				"commande.gauche";
const char* CCfg::CST_COM_DROITE =				"commande.droite";
const char* CCfg::CST_COM_CAMERA_MONTER =		"commande.cameraMonter";
const char* CCfg::CST_COM_CAMERA_DESCENDRE =	"commande.cameraDescendre";
const char* CCfg::CST_COM_CAMERA_DROITE =		"commande.cameraDroite";
const char* CCfg::CST_COM_CAMERA_GAUCHE =		"commande.cameraGauche";
const char* CCfg::CST_COM_CAMERA_RECULER =		"commande.cameraReculer";
const char* CCfg::CST_COM_CAMERA_AVANCER =		"commande.cameraAvancer";
const char* CCfg::CST_COM_VUE_RECULER =			"commande.vueReculer";
const char* CCfg::CST_COM_VUE_AVANCER =			"commande.vueAvancer";


const char* CCfg::CST_COM_TIR1 =					"commande.tir1";
const char* CCfg::CST_COM_TIR2 =					"commande.tir2";
const char* CCfg::CST_COM_MONTER =					"commande.monter";
const char* CCfg::CST_COM_SELECTWEAPONUP =			"commande.selectWeaponUp";
const char* CCfg::CST_COM_SELECTWEAPONDOWN =		"commande.selectWeaponDown";
const char* CCfg::CST_COM_GRAVITY =					"commande.gravity";
const char* CCfg::CST_COM_TEXTURES =				"commande.textures";

const char* CCfg::CST_CEN_IP =						"centralisateur.ip";
const char* CCfg::CST_CEN_PORT =					"centralisateur.port";

const char* CCfg::CST_NET_IP =						"reseau.serveur.ip";
const char* CCfg::CST_NET_PORT =					"reseau.serveur.port";
const char* CCfg::CST_NET_PORTTREE =				"reseau.serveur.portTree";
const char* CCfg::CST_TREE_UPDATE_CLIENTTOSERVER_DELAY =	"reseau.tree.update.client-to-server.delay";
const char* CCfg::CST_TREE_UPDATE_SERVERTOCLIENT_DELAY =	"reseau.tree.update.server-to-client.delay";

const char* CCfg::CST_JOU_NOM =						"joueur.nom";
const char* CCfg::CST_JOU_MAPNOM =					"joueur.mapNom";
const char* CCfg::CST_JOU_HITBOX_VISIBILITY =		"joueur.hitboxVisibility";
const char* CCfg::CST_JOU_SOLIDBOX_VISIBILITY =		"joueur.solidboxVisibility";
const char* CCfg::CST_JOU_SKIN_VISIBILITY =			"joueur.skinVisibility";

const char* CCfg::CST_PLU_ACT_BEGIN =				"plugin.activate.begin";
const char* CCfg::CST_PLU_ACT_END =					"plugin.activate.end";

const char* CCfg::CST_WEB_TCP_SERVER_TIMEOUT =		"web.tcp.server.timeout";
const char* CCfg::CST_WEB_TCP_SERVER_CLIENTS_SIZE =	"web.tcp.server.clients.size";
const char* CCfg::CST_WEB_HTML_PORT =				"web.html.server.port";

const char* CCfg::CST_DEB_SONPERFORMANCES =			"debug.sonPerformances";
const char* CCfg::CST_DEB_SONSPECTRE =				"debug.sonSpectre";
const char* CCfg::CST_DEB_AFFICHEFICHIER =			"debug.afficheFichier";
const char* CCfg::CST_DEB_AFFICHENORMAUX =			"debug.afficheNormaux";
const char* CCfg::CST_DEB_AXES_METER_VISIBILITY = 	"debug.afficheAxesMeter";
const char* CCfg::CST_DEB_CUBIC_METER_VISIBILITY =	"debug.afficheCubeMeter";


CCfg::CCfg() {
	LOGDEBUG(("Cfg::Cfg()%T", this));
}

void CCfg::AfficheDateCompilation() {
	LOGINFO(("Date de compilation : %s", __DATE__));
}

void CCfg::NommeConfig(const string &configFileBaseName, const string& configFileSuffixName) {

	string candidat = "./" + configFileBaseName + "-" + configFileSuffixName + ".ini";	// config-<suffix>.ini
	string defaut = "./" + configFileBaseName + ".ini";				// config.ini

	if(jkt::StringUtils::isNotBlank(configFileSuffixName) && boost::filesystem::exists(candidat)) {
		configFile = candidat;
	}
	else {
		LOGINFO(("Pas de fichier de configuration sp�cifique ('%s')", candidat.c_str()));
		configFile = defaut;				// config.ini
	}

	LOGINFO(("Choix du fichier de configuration : %s", configFile.c_str()));
}

void CCfg::Lit() {
	LOGDEBUG(("Cfg::Lit()%T", this));
	string mot;

	ifstream fichier(configFile.c_str());
	fichier.exceptions(ifstream::eofbit);

	if(!fichier) {
		quit_game("Fichier de configuration introuvable '" + configFile + "'", 1);
	}

	try {

		/* ***************************************
		 * Générale
		 * **************************************/

		do fichier >> mot;	while(mot!=CST_GEN_PLAY_INTRO);		// Indique si l'intro du jeu doit �tre lanc�e au d�marrage
		fichier >> General._playIntro;


		/* ***************************************
		 * Vidéo
		 * **************************************/

		do fichier >> mot;	while( mot!=CST_VID_DISPLAY_POS );
		fichier >> Display.position;
		jkt::StringUtils::trim(Display.position);

		do fichier >> mot;	while( mot!=CST_VID_DISPLAY_SIZE );
		fichier >> Display.X >> Display.Y;

		do fichier >> mot;	while( mot!=CST_VID_FULLSCREEN );
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

		do fichier >> mot;	while( mot!=CST_COM_CAMERA_MONTER );
		fichier >> crotte;
		Commandes.CameraMonter.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.CameraMonter.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_CAMERA_DESCENDRE );
		fichier >> crotte;
		Commandes.CameraDescendre.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.CameraDescendre.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_CAMERA_DROITE );
		fichier >> crotte;
		Commandes.CameraDroite.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.CameraDroite.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_CAMERA_GAUCHE );
		fichier >> crotte;
		Commandes.CameraGauche.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.CameraGauche.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_CAMERA_RECULER );
		fichier >> crotte;
		Commandes.CameraReculer.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.CameraReculer.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_CAMERA_AVANCER );
		fichier >> crotte;
		Commandes.CameraAvancer.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.CameraAvancer.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_VUE_RECULER );
		fichier >> crotte;
		Commandes.VueReculer.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.VueReculer.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_VUE_AVANCER );
		fichier >> crotte;
		Commandes.VueAvancer.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.VueAvancer.mouse = crotte;

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

		do fichier >> mot;	while( mot!=CST_COM_GRAVITY );
		fichier >> crotte;
		Commandes.Gravity.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Gravity.mouse = crotte;

		do fichier >> mot;	while( mot!=CST_COM_TEXTURES );
		fichier >> crotte;
		Commandes.Textures.key = (SDLKey)crotte;
		fichier >> crotte >> mot;
		Commandes.Textures.mouse = crotte;


		/* ***************************************
		 * Centralisateur
		 * **************************************/

		do fichier >> mot;	while( mot!=CST_CEN_IP );
		fichier >> Centralisateur.m_IpServer;

		do fichier >> mot;	while( mot!=CST_CEN_PORT );
		fichier >> Centralisateur.m_Port;


		/* ***************************************
		 * R�seau
		 * **************************************/

		do fichier >> mot;	while( mot!=CST_NET_IP );
		fichier >> Reseau._IpServer;

		do fichier >> mot;	while( mot!=CST_NET_PORT );
		fichier >> Reseau._Port;

		do fichier >> mot;	while( mot!=CST_NET_PORTTREE );
		fichier >> Reseau._portTree;

		do fichier >> mot;	while( mot!=CST_TREE_UPDATE_CLIENTTOSERVER_DELAY );
		fichier >> Reseau._treeUpdateClientToServerDelay;

		do fichier >> mot;	while( mot!=CST_TREE_UPDATE_SERVERTOCLIENT_DELAY );
		fichier >> Reseau._treeUpdateServerToClientDelay;


		/* ***************************************
		 * Joueur
		 * **************************************/

		do fichier >> mot;	while( mot!=CST_JOU_NOM );
		fichier >> Joueur.nom;

		do fichier >> mot;	while( mot!=CST_JOU_MAPNOM );
		fichier >> Joueur.mapName;

		do fichier >> mot;	while( mot!=CST_JOU_HITBOX_VISIBILITY );
		fichier >> Joueur.hitboxVisibility;

		do fichier >> mot;	while( mot!=CST_JOU_SOLIDBOX_VISIBILITY );
		fichier >> Joueur.solidboxVisibility;

		do fichier >> mot;	while( mot!=CST_JOU_SKIN_VISIBILITY );
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
		 * Web
		 * **************************************/

		do fichier >> mot;	while( mot!=CST_WEB_TCP_SERVER_TIMEOUT );
		fichier >> Web._htmlTcpTimeout;

		do fichier >> mot;	while( mot!=CST_WEB_TCP_SERVER_CLIENTS_SIZE );
		fichier >> Web._htmlTcpClientsSize;

		do fichier >> mot;	while( mot!=CST_WEB_HTML_PORT );
		fichier >> Web._htmlServerPort;


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

		do fichier >> mot;	while( mot!=CST_DEB_AXES_METER_VISIBILITY);
		fichier >> Debug.axesMeterVisibility;

		do fichier >> mot;	while( mot!=CST_DEB_CUBIC_METER_VISIBILITY);
		fichier >> Debug.cubicMeterVisibility;

		fichier.close();
	}
	catch(istringstream::failure& finDuFlux) {
		quit_game("Fichier de configuration corrompu '" + configFile + "'", 1);
	}
}

void CCfg::Ecrit() {
	ofstream fichier( configFile.c_str() );

	fichier << "\n-------------------------GENERAL-------------------------";
	fichier << endl << CST_GEN_PLAY_INTRO << "\t\t" << General._playIntro;

	fichier << "\n\n-------------------------VIDEO-------------------------";
	fichier << endl << CST_VID_DISPLAY_POS << "\t\t" << Display.position;
	fichier << endl << CST_VID_DISPLAY_SIZE << "\t\t" << Display.X << "\t" << Display.Y;
	fichier << endl << CST_VID_FULLSCREEN << "\t" << Display.Fullscreen();

	fichier << "\n\n-------------------------AUDIO-------------------------";
	fichier << endl << CST_AUD_OUTPUT << "\t\t" << Audio.m_Output << "\t(" << resolveOutput(Audio.m_Output) << ")";
	fichier << endl << CST_AUD_MIXER << "\t\t\t" << Audio.m_Mixer << "\t(" << resolveMixer(Audio.m_Mixer) << ")";
	fichier << endl << CST_AUD_DRIVER << "\t\t" << Audio.m_Driver << "\t(" << resolveDriver(Audio.m_Driver) << ")";

	if( Audio.m_DriverRecord!=-1 ) {
		fichier << endl << CST_AUD_DRIVERRECORD << "\t" << Audio.m_DriverRecord << "\t(" << resolveDriverRecord(Audio.m_DriverRecord) << ")";
	}
	else {
		fichier << endl << CST_AUD_DRIVERRECORD << "\t" << Audio.m_DriverRecord << "\t(Pas de driver d'acquisition)";
	}

	fichier << "\n\n-----------------------COMMANDES-----------------------";
	fichier << endl << CST_COM_AVANCER << "\t\t\t" 		<< Commandes.Avancer.key << "\t" << Commandes.Avancer.mouse << "\t(" << Commandes.resolve(Commandes.Avancer) << ")";
	fichier << endl << CST_COM_RECULER << "\t\t\t" 		<< Commandes.Reculer.key << "\t" << Commandes.Reculer.mouse << "\t(" << Commandes.resolve(Commandes.Reculer) << ")";
	fichier << endl << CST_COM_GAUCHE << "\t\t\t\t" 	<< Commandes.Gauche.key << "\t" << Commandes.Gauche.mouse << "\t(" << Commandes.resolve(Commandes.Gauche) << ")";
	fichier << endl << CST_COM_DROITE << "\t\t\t\t" 	<< Commandes.Droite.key << "\t" << Commandes.Droite.mouse << "\t(" << Commandes.resolve(Commandes.Droite) << ")";

	fichier << endl << CST_COM_CAMERA_MONTER << "\t\t\t\t" 		<< Commandes.CameraMonter.key << "\t" << Commandes.CameraMonter.mouse << "\t(" << Commandes.resolve(Commandes.CameraMonter) << ")";
	fichier << endl << CST_COM_CAMERA_DESCENDRE << "\t\t\t\t"	<< Commandes.CameraDescendre.key << "\t" << Commandes.CameraDescendre.mouse << "\t(" << Commandes.resolve(Commandes.CameraDescendre) << ")";
	fichier << endl << CST_COM_CAMERA_DROITE << "\t\t\t\t" 		<< Commandes.CameraDroite.key << "\t" << Commandes.CameraDroite.mouse << "\t(" << Commandes.resolve(Commandes.CameraDroite) << ")";
	fichier << endl << CST_COM_CAMERA_GAUCHE << "\t\t\t\t" 		<< Commandes.CameraGauche.key << "\t" << Commandes.CameraGauche.mouse << "\t(" << Commandes.resolve(Commandes.CameraGauche) << ")";
	fichier << endl << CST_COM_CAMERA_RECULER << "\t\t\t\t" 	<< Commandes.CameraReculer.key << "\t" << Commandes.CameraReculer.mouse << "\t(" << Commandes.resolve(Commandes.CameraReculer) << ")";
	fichier << endl << CST_COM_CAMERA_AVANCER << "\t\t\t\t" 	<< Commandes.CameraAvancer.key << "\t" << Commandes.CameraAvancer.mouse << "\t(" << Commandes.resolve(Commandes.CameraAvancer) << ")";
	fichier << endl << CST_COM_VUE_RECULER << "\t\t\t\t" 		<< Commandes.VueReculer.key << "\t" << Commandes.VueReculer.mouse << "\t(" << Commandes.resolve(Commandes.VueReculer) << ")";
	fichier << endl << CST_COM_VUE_AVANCER << "\t\t\t\t" 		<< Commandes.VueAvancer.key << "\t" << Commandes.VueAvancer.mouse << "\t(" << Commandes.resolve(Commandes.VueAvancer) << ")";


	fichier << endl << CST_COM_TIR1 << "\t\t\t\t" 		<< Commandes.Tir1.key << "\t" << Commandes.Tir1.mouse << "\t(" << Commandes.resolve(Commandes.Tir1) << ")";
	fichier << endl << CST_COM_TIR2 << "\t\t\t\t" 		<< Commandes.Tir2.key << "\t" << Commandes.Tir2.mouse << "\t(" << Commandes.resolve(Commandes.Tir2) << ")";
	fichier << endl << CST_COM_MONTER << "\t\t\t\t" 	<< Commandes.Monter.key << "\t" << Commandes.Monter.mouse << "\t(" << Commandes.resolve(Commandes.Monter) << ")";
	fichier << endl << CST_COM_SELECTWEAPONUP << "\t\t" << Commandes.SelectWeaponUp.key << "\t" << Commandes.SelectWeaponUp.mouse << "\t(" << Commandes.resolve(Commandes.SelectWeaponUp) << ")";
	fichier << endl << CST_COM_SELECTWEAPONDOWN << "\t" << Commandes.SelectWeaponDown.key << "\t" << Commandes.SelectWeaponDown.mouse << "\t(" << Commandes.resolve(Commandes.SelectWeaponDown) << ")";
	fichier << endl << CST_COM_GRAVITY << "\t\t\t" 		<< Commandes.Gravity.key << "\t" << Commandes.Gravity.mouse << "\t(" << Commandes.resolve(Commandes.Gravity) << ")";
	fichier << endl << CST_COM_TEXTURES << "\t\t\t" 	<< Commandes.Textures.key << "\t" << Commandes.Textures.mouse << "\t(" << Commandes.resolve(Commandes.Textures) << ")";

	fichier << "\n\n--------------------CENTRALISATEUR---------------------";
	fichier << endl << CST_CEN_IP << "\t\t" << Centralisateur.m_IpServer;
	fichier << endl << CST_CEN_PORT << "\t\t" << Centralisateur.m_Port;

	fichier << "\n\n------------------------RESEAU-------------------------";
	fichier << endl << CST_NET_IP << "\t\t\t" << Reseau._IpServer;
	fichier << endl << CST_NET_PORT << "\t\t\t" << Reseau._Port;
	fichier << endl << CST_NET_PORTTREE << "\t\t" << Reseau._portTree;
	fichier << endl << CST_TREE_UPDATE_CLIENTTOSERVER_DELAY << "\t\t" << Reseau._treeUpdateClientToServerDelay;
	fichier << endl << CST_TREE_UPDATE_SERVERTOCLIENT_DELAY << "\t\t" << Reseau._treeUpdateServerToClientDelay;

	fichier << "\n\n------------------------JOUEUR-------------------------";
	fichier << endl << CST_JOU_NOM << "\t\t\t\t\t" << Joueur.nom;
	fichier << endl << CST_JOU_MAPNOM << "\t\t\t\t" << Joueur.mapName;
	fichier << endl << CST_JOU_HITBOX_VISIBILITY << "\t\t" << Joueur.hitboxVisibility;
	fichier << endl << CST_JOU_SOLIDBOX_VISIBILITY << "\t" << Joueur.solidboxVisibility;
	fichier << endl << CST_JOU_SKIN_VISIBILITY << "\t\t" << Joueur.skinVisibility;

	fichier << "\n\n------------------------PLUGIN-------------------------";
	fichier << endl << CST_PLU_ACT_BEGIN << endl;
	for(string& plugin : Plugin._defaultPluging) {
		fichier << "\t" << plugin << endl;
	}
	fichier << CST_PLU_ACT_END;

	fichier << "\n\n-------------------------WEB---------------------------";
	fichier << endl << CST_WEB_TCP_SERVER_TIMEOUT << "\t\t" << Web._htmlTcpTimeout;
	fichier << endl << CST_WEB_TCP_SERVER_CLIENTS_SIZE << "\t\t" << Web._htmlTcpClientsSize;
	fichier << endl << CST_WEB_HTML_PORT << "\t\t" << Web._htmlServerPort;

	fichier << "\n\n------------------------DEBUG-------------------------";
	fichier << endl << CST_DEB_SONPERFORMANCES << "\t\t" << Debug.bSonPerformances;
	fichier << endl << CST_DEB_SONSPECTRE << "\t\t\t" << Debug.bSonSpectre;
	fichier << endl << CST_DEB_AFFICHEFICHIER << "\t\t" << Debug.bAfficheFichier;
	fichier << endl << CST_DEB_AFFICHENORMAUX << "\t\t" << Debug.bAfficheNormaux;
	fichier << endl << CST_DEB_AXES_METER_VISIBILITY << "\t\t" << Debug.axesMeterVisibility;
	fichier << endl << CST_DEB_CUBIC_METER_VISIBILITY << "\t\t" << Debug.cubicMeterVisibility;

	fichier << "\n\n------------------------FIN-------------------------\n";

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
	LOGDEBUG(("CCfg::CAudio::Init()"));
	unsigned int caps = 0;

	// Initialisation audio de FMOD
	LOGINFO(("Version de FMOD : %f", FSOUND_GetVersion()));

	if(FSOUND_GetVersion() < FMOD_VERSION) {
		LOGINFO(("Error : You are using a wrong FMOD DLL version!  You should be using at least version %f", FMOD_VERSION));

		return false;
	}

	FSOUND_SetOutput( output );		// S�lection de l'output
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

	if(numDrivers > 0) {	// V�rifie s'il y a un driver pour le son
		if(driver < numDrivers ) {
			FSOUND_SetDriver(driver);			// S�lection du driver configur�
			m_Driver = driver;
		}
		else {									// Le driver configur� ne peut pas fonctionner
			LOGERROR(("Driver son mal configur�, choix par d�faut."));
			driver = 0;							// Modification du fichier de configuration
			FSOUND_SetDriver(0);				// Choix du driver par d�faut
			m_Driver = 0;
		}
	}
	else {
		// Pas de driver son
		LOGERROR((" Aucun driver son d�tect�."));
	}

	FSOUND_SetMixer(mixer);			// S�lection du mixer
	m_Mixer = mixer;

	// Initialisation
	if(!FSOUND_Init(44100, 32, FSOUND_INIT_GLOBALFOCUS)) {
		LOGERROR(("Erreur FSOUND_Init : %s", FMOD_ErrorString(FSOUND_GetError())));
		FSOUND_Close();
	}

	FSOUND_DSP_SetActive( FSOUND_DSP_GetFFTUnit(), true );

	if( FSOUND_Record_GetNumDrivers() > 0 ) {	// V�rifie s'il y a un driver d'acquisition du son
		if( driverRecord < FSOUND_Record_GetNumDrivers() ) {
			if( !FSOUND_Record_SetDriver(driverRecord)) {	// S�lection du driver				// d'entr�e pour le micro
				LOGERROR((" Erreur FSOUND_Record_SetDriver : %s", FMOD_ErrorString(FSOUND_GetError())));
				LOGERROR(("\tAVIS AU PROGRAMMEUR Le programme va planter s'il y a une tentative d'utilisation du micro !!!"));
			}
			else {
				m_DriverRecord = driverRecord;
			}
		}
		else {		// Le driver de record ne peut pas fonctionner
			LOGERROR(("Driver de record son mal configur�, choix par d�faut."));

			if( !FSOUND_Record_SetDriver(0) ) {		// S�lection du driver de record par d�faut
				LOGERROR(("Erreur FSOUND_Record_SetDriver : ", FMOD_ErrorString(FSOUND_GetError())));
				LOGERROR(("\tAVIS AU PROGRAMMEUR Le programme va planter s'il y a une tentative d'utilisation du micro !!!"));
			}
			else {
				m_DriverRecord = 0;
			}
		}
	}
	else {		// Aucun driver d'acquisition du son d�tect�
		LOGERROR((" Aucun driver d'acquisition du son d�tect�."));
	}

	// RESUME DE L'INITIALISATION
	LOGINFO(("RESUME DE L'INITIALISATION AUDIO"));
	LOGINFO(("FSOUND Output Method : '%s'", resolveOutput(FSOUND_GetOutput())));
	LOGINFO(("FSOUND Mixer         : '%s'", resolveMixer(FSOUND_GetMixer())));
	LOGINFO(("FSOUND Driver        : '%s'", resolveDriver(FSOUND_GetDriver())));

	FSOUND_GetDriverCaps(FSOUND_GetDriver(), &caps);

	if (!caps) {
		LOGINFO(("\tCe driver supporte seulement le mode software.\n\tIl ne supporte pas bien le son 3D."));
	}

	if (caps & FSOUND_CAPS_HARDWARE) {
		LOGINFO(("\t- Ce driver supporte le son 3D en hardware!"));
	}

	if (caps & FSOUND_CAPS_EAX2) {
		LOGINFO(("\t- Ce driver supporte l'EAX 2 reverb!"));
	}

	if (caps & FSOUND_CAPS_EAX3) {
		LOGINFO(("\t- Ce driver supporte l'EAX 3 reverb!"));
	}

	LOGINFO(("FSOUND Record Driver : %s", resolveDriverRecord(FSOUND_Record_GetDriver())));

	int num2d, num3d, total;
	FSOUND_GetNumHWChannels( &num2d, &num3d, &total );

	LOGINFO(("Hardware 2D channels : %d", num2d));
	LOGINFO(("Hardware 3D channels : %d", num3d));
	LOGINFO(("Total hardware channels : %d", total));

	return true;
}

void CCfg::CDisplay::Init() {
	InitSDL();
	InitOpenGL();
	InitAgar();
}

void CCfg::CDisplay::InitSDL() {
	LOGDEBUG(("init_SDL(config) begin"));

#ifdef WIN32
	//	_putenv("SDL_VIDEODRIVER=directx");	// A FAIRE A FAIRE A FAIRE : code compatible Linux
#endif

	if( SDL_Init( SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_NOPARACHUTE ) < 0 ) {	// First, initialize SDL's video subsystem
		LOGERROR(("Video initialization failed : %s", SDLNet_GetError()));
		exit( 1 );
	}

	// Icone et titre
	SDL_WM_SetCaption( "JKT 2010", "C'est un jeu qui tue !!!" );	// Titre et icon de la fen�tre

	string iconeFichier = "@Icone/Icone.bmp";
	jkt::RessourcesLoader::getFileRessource(iconeFichier);
	SDL_WM_SetIcon( IMG_Load( iconeFichier.c_str() ), 0 );


	const SDL_VideoInfo* info = SDL_GetVideoInfo( );	// Let's get some video information
	if( !info ) {
		LOGERROR(("Video query failed : %s", SDL_GetError()));
		exit( 1 );
	}

	bpp = info->vfmt->BitsPerPixel;

	if( SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 )==-1 ) {
		LOGERROR(("Erreur init_SDL : SDL_GL_RED_SIZE"));
		exit( 1 );
	}

	if( SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 )==-1 ) {
		LOGERROR(("Erreur init_SDL : SDL_GL_GREEN_SIZE"));
		exit( 1 );
	}

	if( SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 )==-1 ) {
		LOGERROR(("Erreur init_SDL : SDL_GL_BLUE_SIZE"));
		exit( 1 );
	}

	if( SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 )==-1 ) {
		LOGERROR(("Erreur init_SDL : SDL_GL_ALPHE_SIZE"));
		exit( 1 );
	}

	if( SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 )==-1 ) {
		LOGERROR(("Erreur init_SDL : SDL_GL_DEPTH_SIZE"));
		exit( 1 );
	}

	if( SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 )==-1 ) {
		LOGERROR(("Erreur init_SDL : SDL_GL_DOUBLEBUFFER"));
		exit( 1 );
	}

	stringstream var;
	var << "hw_available=" << info->hw_available;
	var << " ; wm_available=" << info->wm_available;
	var << " ; blit_hw:1=" << info->blit_hw;
	var << " ; blit_hw_CC:1=" << info->blit_hw_CC;
	var << " ; blit_hw_A:1=" << info->blit_hw_A;
	var << " ; blit_sw:1=" << info->blit_sw;
	var << " ; blit_sw_CC:1=" << info->blit_sw_CC;
	var << " ; blit_sw_A:1=" << info->blit_sw_A;
	var << " ; blit_fill=" << info->blit_fill;
	var << " ; video_mem=" << info->video_mem;
	LOGINFO((var.str().c_str()));

	flags = SDL_OPENGL | SDL_RESIZABLE;

	if( Fullscreen() )
		flags |= SDL_FULLSCREEN;

	// Get available fullscreen/hardware modes
	SDL_Rect **modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);

	// Check if there are any modes available
	if( modes == (SDL_Rect **)0 ) {
		LOGERROR(("Aucun mode video n'est disponible"));
		exit( 1 );
	}

	/* Check if our resolution is restricted */
	if(modes == (SDL_Rect **)-1) {
		LOGINFO(("Tous les modes video sont disponibles"));
	}
	else {
		/* Print valid modes */
		stringstream str;

		for( int i=0 ; modes[i] ; ++i ) {
			str << "[" << modes[i]->w << "*" << modes[i]->h << "] ";
		}

		LOGINFO(("Modes video disponibles : %s", str.str().c_str()));
	}

	if( !SDL_VideoModeOK(X, Y, bpp, flags) ) {
		LOGWARN(("WARNING : Mode video indisponible => mode choisi par defaut, corrigez la configuration"));
		X = modes[0]->w;
		Y = modes[0]->h;
	}

	string windowPosition("SDL_VIDEO_WINDOW_POS=");
	windowPosition = windowPosition + position;
	putenv(windowPosition.c_str()); 	//pour centrer la fen�tre

	screen = SDL_SetVideoMode(X, Y, bpp, flags);		// Set the video mode

	if( screen == 0) {
		LOGERROR(("Error : Video mode set failed: ", SDL_GetError()));
		exit( 1 );
	}

	SDL_WarpMouse( 250, 250 );			// Positionne le curseur de la souris
	SDL_ShowCursor( SDL_DISABLE );		// Cache le curseur de la souris
	SDL_WM_GrabInput( SDL_GRAB_ON );	// Active le mode d�placement relatif de la souris

	SDL_EnableKeyRepeat( 500, 500 );	// R�p�tition des touches clavier

	char txt1[50];
	LOGINFO(("Driver video :"));
	if( SDL_VideoDriverName(txt1, 50) ) {
		LOGINFO((txt1));
	}

	SDL_EnableUNICODE(SDL_ENABLE);

	LOGINFO(("Video memory : %d Mo", info->video_mem/1024));

	// Purge les �v�nements SDL, si on ne le fait pas deux fen�tres du jeu s'affichent et les �v�nements sont mal g�r�s
	SDL_PumpEvents();

	LOGDEBUG(("init_SDL(config) end"));
}

void CCfg::CDisplay::InitOpenGL() {
	LOGDEBUG(("setup_opengl(width=%d,height=%d) begin", X, Y));

	// Informations openGL
	LOGINFO(("Version openGL : %s", glGetString(GL_VERSION)));
	LOGINFO(("Version GLU : %s", gluGetString(GLU_VERSION)));
	LOGINFO(("Version GLEW : %s", glewGetString(GLEW_VERSION)));
	LOGINFO(("Version GLSL support�s : %s", glGetString(GL_SHADING_LANGUAGE_VERSION)));
	LOGINFO(("Extensions openGL disponibles : %s", glGetString(GL_EXTENSIONS)));
	LOGINFO(("Modele de la carte graphique : %s", glGetString(GL_RENDERER)));
	LOGINFO(("Fabricant de la carte graphique : %s", glGetString(GL_VENDOR)));



	if(glewInit() != GLEW_OK) {
		LOGINFO(("Echec d'initialisation de GLEW"));
		exit(1);
	}

	glewInit();

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );					// Sp�cifie la couleur de vidage du tampon chromatique
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glShadeModel( GL_SMOOTH );								// Mode d�grad� pour le remplissage des polynomes
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glViewport( 0, 0, X, Y );

	LOGINFO(("Erreur OpenGL : %s", gluErrorString(glGetError())));

	LOGDEBUG(("setup_opengl() end"));
}

void CCfg::CDisplay::InitAgar() {
	LOGDEBUG(("setup Agar"));
	// Informations Agar
	AG_AgarVersion agarVersion;
	AG_GetVersion(&agarVersion);

	LOGINFO(("Version Agar : %d.%d.%d (%s)", agarVersion.major, agarVersion.minor, agarVersion.patch, agarVersion.release));

	// Initialisation Agar (librairie de gestion du menu)
	if(AG_InitCore("JKT", 0) == -1 || AG_InitVideoSDL(screen, flags) == -1) {
		LOGERROR(("Erreur d'initialisation Agar : '%s'", AG_GetError()));
	}

	// Noms des drivers disponibles pour Agar
	char drvNames[256];
	AG_ListDriverNames(drvNames, sizeof(drvNames));

	LOGINFO(("Available drivers : %s", drvNames));

	LOGINFO(("Driver Agar actif :"));
	AG_Driver* driver = (AG_Driver *)agDriverSw;
	LOGINFO((" - Identification : %s", driver->_inherit.name));
	LOGINFO((" - Compatibilit� OpenGL   : %d", AG_UsingGL(driver)));
	LOGINFO((" - Compatibilit� SDL      : %d", AG_UsingSDL(driver)));

	unsigned int x, y;
	AG_GetDisplaySize(driver, &x, &y);
	LOGINFO((" - Available display size : %d * %d", x, y));

}

void CCfg::CDisplay::ChangeVideoSize(int x, int y) {
	LOGDEBUG(("changeVideoSize(config) begin"));
	if( !SDL_VideoModeOK(x, y, bpp, flags) ) {
		LOGINFO(("Mode video invalide"));
		return;
	}

	if( screen )
		SDL_FreeSurface( screen );							// Vide la surface

	screen = SDL_SetVideoMode( x, y, bpp, flags );	// Set the video mode
	if(screen == 0) {
		LOGERROR(("Video mode set failed : ", SDL_GetError()));
		exit( 1 );
	}

	X = x;
	Y = y;

	InitOpenGL();

	LOGDEBUG(("changeVideoSize() end"));
}

void CCfg::CReseau::Init() {
	if(SDLNet_Init() == -1) {
		LOGERROR(("SDLNet_Init() failed : %s", SDLNet_GetError()));
		exit( 1 );
	}
}
