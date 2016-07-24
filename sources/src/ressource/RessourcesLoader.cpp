
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>

#include "util/Trace.h"
#include "ressource/RessourceConstantes.h"
#include "util/FindFolder.h"
#include "service/dto/MapInformationDto.h"
#include "service/MapService.h"

#include "RessourcesLoader.h"

using namespace std;

const char* MAP_ALIAS =				"$";							// Repr�sente le r�pertoire de la Map courante dans une ressource

const char* ASE_DIRECTORY =			"./ase/";						// R�pertoire des MAP de jeu
const char* ASE_EXTENSION =			".ASE";

const char* MAP_DIRECTORY =			"./Map/";						// R�pertoire des MAP de jeu
const char* MAP_EXTENSION =			".map.xml";

const char* PLAYER_MAP_DIRECTORY =	"./Ressources/Maps/Joueurs/";	// R�pertoire Map des skins de joueur
const char* PLUGINS_DIRECTORY =		"./plugins/";					// R�pertoire des plugins
const char* TEMPORARY_DIRECTORY =	"./Tmp/";						// R�pertoire temporaire de travail

namespace JktUtils
{

const char* RessourcesLoader::elementsNamesAndFolders[] = {
	"@Ase",		"./ase",							// R�pertoire des fichiers ASE importables
	"@Map",		"./map",							// R�pertoire des Map

	"@Fonte",	"./Ressources/Fontes",				// Polices / fontes

	"@Fond",	"./Ressources/Images/Fonds",		// Images de fond
	"@Icone",	"./Ressources/Images/Icones",		// Icones des menus
	"@Texture",	"./Ressources/Images/Textures",		// Images de textures par d�faut

	"@Joueur",	"./Ressources/Maps/Joueurs",		// Maps des joueurs par d�faut
	"@Arme",	"./Ressources/Maps/Armes",			// Map des armes par d�faut

	"@Bruit",	"./Ressources/Sons/Bruits",			// Bruits par d�faut des Map
	"@Jingle",	"./Ressources/Sons/Jingles",		// Jingles des menus
	"@Musique",	"./Ressources/Sons/Musiques",		// Musiques par d�faut
	NULL
};

bool RessourcesLoader::getFileRessource(const string& rep, string& file) {
	if(!getFileRessource(file)) {	// Cherche s'il correspond � une ressource par d�faut
									// Sinon, cherche s'il correspond � une ressource du r�pertoire
		if((int)file.find_first_of( MAP_ALIAS ) == 0) {
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

			for(int i=0; elementsNamesAndFolders[i*2 + 0] != 0 ; i++) {
				if(!resourceType.compare(elementsNamesAndFolders[i*2 + 0])) {
					file.replace(0, nbr, elementsNamesAndFolders[i*2 + 1]);
					LOGDEBUG(("La ressource '%s' est '%s')", before.c_str(), file.c_str()));
					bTrouve = true;
					break;
				}
			}

			if(!bTrouve) {
				// TODO Est-ce vraiment un warning ?
				LOGWARN(("Ressource de type inconnu '%s' (type identifi� '%s')", before.c_str(), resourceType.c_str()));
			}
		}
		else {
			// TODO Est-ce vraiment un warning ?
			LOGWARN(("Format de ressource non pris en compte : '%s'", before.c_str()));
		}
	}

	// Indique si une ressource a �t� trouv�e
	return bTrouve;
}

vector<string> RessourcesLoader::getMaps() {
	vector<MapInformationDto> content;
	MapService::loadMapDirectoryContent(content);

	std::vector<std::string> mapNames;

	for(MapInformationDto& dto : content) {
		mapNames.push_back(dto.getMapFileMinimalName());
	}

	return mapNames;
}

vector<string> RessourcesLoader::getPlayerMaps() {
	vector<MapInformationDto> content;
	MapService::loadPlayerMapDirectoryContent(content);

	std::vector<std::string> mapNames;

	for(MapInformationDto& dto : content) {
		mapNames.push_back(dto.getMapFileMinimalName());
	}

	return mapNames;
}

};
