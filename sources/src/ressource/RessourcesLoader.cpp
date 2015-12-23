
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>

#include "util/Trace.h"
#include "ressource/RessourceConstantes.h"
#include "util/FindFolder.h"
#include "RessourcesLoader.h"

using namespace std;

const char* MAP_DIRECTORY =			"./Map/";						// R�pertoire des MAP de jeu
const char* PLAYER_MAP_DIRECTORY =	"./Ressources/Maps/Joueurs/";	// R�pertoire des scripts des plugins

const char* PLUGINS_DIRECTORY =		"./plugins/";					// R�pertoire des MAP de jeu

const char* TEMPORARY_DIRECTORY =	"./Tmp/";						// R�pertoire temporaire de travail

namespace JktUtils
{
int RessourcesLoader::nbrElements = 9;
const char* RessourcesLoader::elementsNamesAndFolders[] =
{
	"@Fond",	"./Ressources/Images/Fonds",		// Images de fond
	"@Fonte",	"./Ressources/Images/Fontes",		// Polices / fontes
	"@Icone",	"./Ressources/Images/Icones",		// Icones des menus
	"@Texture",	"./Ressources/Images/Textures",		// Images de textures par d�faut

	"@Joueur",	"./Ressources/Maps/Joueurs",		// Maps des joueurs par d�faut
	"@Arme",	"./Ressources/Maps/Armes",			// Map des armes par d�faut

	"@Bruit",	"./Ressources/Sons/Bruits",			// Bruits par d�faut des Map
	"@Jingle",	"./Ressources/Sons/Jingles",		// Jingles des menus
	"@Musique",	"./Ressources/Sons/Musiques",		// Musiques par d�faut
};

bool RessourcesLoader::getFileRessource(const string& rep, string& file) {
	if(!getFileRessource(file)) {	// Cherche s'il correspond � une ressource par d�faut
									// Sinon, cherche s'il correspond � une ressource du r�pertoire
		if((int)file.find_first_of('$') == 0) {
			int nbr1 = (int)file.find_first_of('\\');
			int nbr2 = (int)file.find_first_of('/');
			int nbr;

			if(nbr1 > nbr2) {
				nbr = nbr1;
			}
			else {
				nbr = nbr2;
			}

			if(nbr == 1) {
				file.replace(0, nbr, rep);
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		return true;
	}
}

bool RessourcesLoader::getFileRessource(string& file) {
	// R�cup�ration du premier �l�ment du nom du fichier
	// par exemple, dans --Ressource\Textures\... on r�cup�re --Ressource
	bool bTrouve = false;
	string before = file;

	replace(file.begin(), file.end(), '\\', '/');

	if(file.find_first_of('@') == 0) {
		unsigned int nbr = file.find_first_of('/');

		if(nbr != string::npos) {
			string resourceType = file.substr(0, nbr);

			for(int i=0; i<nbrElements; i++) {
				if(!resourceType.compare(elementsNamesAndFolders[i*2 + 0])) {
					file.replace(0, nbr, elementsNamesAndFolders[i*2 + 1]);
					LOGERROR(("La ressource '%s' est '%s')", before.c_str(), file.c_str()));
					bTrouve = true;
					break;
				}
			}

			if(!bTrouve) {
				LOGERROR(("Ressource de type inconnu '%s' (type identifi� '%s')", before.c_str(), resourceType.c_str()));
			}
		}
		else {
			LOGERROR(("Format de ressource non pris en compte : '%s'", before.c_str()));
		}
	}

	// Indique si une ressource a �t� trouv�e
	return bTrouve;
}

vector<string> RessourcesLoader::getMaps() {
	std::vector<std::string> mapNames;
	string mapName;
	CFindFolder folder( MAP_DIRECTORY, 0, ".map.xml" );
	folder.nbr();   // TODO : Cette ligne ne sert � rien, mais lorsqu'elle n'est pas pr�sente il y a un bug
	folder.reset();

	while(folder.findNext(mapName)) {
		mapName.erase( mapName.find_last_of( "." ) );
		mapName.erase( mapName.find_last_of( "." ) );
		
		mapNames.push_back(mapName);
	}

	return mapNames;
}

vector<string> RessourcesLoader::getPlayerMaps() {
	std::vector<std::string> mapNames;
	string mapName;
	CFindFolder folder( PLAYER_MAP_DIRECTORY, 0, ".map.xml" );
	folder.nbr();   // TODO : Cette ligne ne sert � rien, mais lorsqu'elle n'est pas pr�sente il y a un bug
	folder.reset();

	while( folder.findNext( mapName ) )
	{
		mapName.erase( mapName.find_last_of( "." ) );
		mapName.erase( mapName.find_last_of( "." ) );
		
		mapNames.push_back(mapName);
	}

	return mapNames;
}

};
