
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>

#include "ressource/RessourceConstantes.h"
#include "util/FindFolder.h"
#include "RessourcesLoader.h"

using namespace std;

const char* MAP_DIRECTORY =			"./Map/";						// Répertoire des MAP de jeu
const char* PLAYER_MAP_DIRECTORY =	"./Ressources/Maps/Joueurs/";	// Répertoire des scripts des plugins

const char* PLUGINS_DIRECTORY =		"./plugins/";					// Répertoire des MAP de jeu

const char* TEMPORARY_DIRECTORY =	"./Tmp/";						// Répertoire temporaire de travail

namespace JktUtils
{
int RessourcesLoader::nbrElements = 9;
const char* RessourcesLoader::elementsNamesAndFolders[] =
{
	"@Fond",	".\\Ressources\\Images\\Fonds",		// Images de fond
	"@Fonte",	".\\Ressources\\Images\\Fontes",	// Polices / fontes
	"@Icone",	".\\Ressources\\Images\\Icones",	// Icones des menus
	"@Texture",	".\\Ressources\\Images\\Textures",	// Images de textures par défaut

	"@Joueur",	".\\Ressources\\Maps\\Joueurs",		// Maps des joueurs par défaut
	"@Arme",	".\\Ressources\\Maps\\Armes",		// Map des armes par défaut

	"@Bruit",	".\\Ressources\\Sons\\Bruits",		// Bruits par défaut des Map
	"@Jingle",	".\\Ressources\\Sons\\Jingles",		// Jingles des menus
	"@Musique",	".\\Ressources\\Sons\\Musiques",	// Musiques par défaut
};

bool RessourcesLoader::getFileRessource(const string& rep, string& file)
{
	if(!getFileRessource(file)) {	// Chercher s'il correspond à une ressource par défaut, sinon
			// Cherche s'il correspond à une ressource du répertoire
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
				file.replace(0,nbr,rep);
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

bool RessourcesLoader::getFileRessource(string& file)
{
	// Récupération du premier élément du nom du fichier
	// par exemple, dans --Ressource\Textures\... on récupère --Ressource
	bool bTrouve = false;
	string before = file;

	if((int)file.find_first_of('@') == 0) {
		int nbr1 = (int)file.find_first_of('\\');
		int nbr2 = (int)file.find_first_of('/');
		int nbr;

		if(nbr1 > nbr2) {
			nbr = nbr1;
		}
		else {
			nbr = nbr2;
		}

		if(nbr != string::npos) {
			string resourceType = file.substr(0, nbr);

			for(int i=0; i<nbrElements; i++) {
				if(!resourceType.compare(elementsNamesAndFolders[i*2 + 0])) {
					file.replace(0, nbr, elementsNamesAndFolders[i*2 + 1]);

					cout << endl << "La ressource '" << before << "' est '" << file << "'";
					bTrouve = true;
					break;
				}
			}

			if(!bTrouve) {
				std::cerr << endl << "Ressource de type inconnu : '" << before << "' (type identifie '" << resourceType << "'";
			}
		}
		else {
			std::cerr << endl << "Format de ressource non pris en compte : '" << before << "'";
		}
	}

	// Indique si une ressource a été trouvée
	return bTrouve;
}

vector<string> RessourcesLoader::getMaps() {
	std::vector<std::string> mapNames;
	string mapName;
	CFindFolder folder( MAP_DIRECTORY, 0, ".map.xml" );
	folder.nbr();   // TODO : Cette ligne ne sert à rien, mais lorsqu'elle n'est pas présente il y a un bug
	folder.reset();

	int mapNumber = 0;
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
	folder.nbr();   // TODO : Cette ligne ne sert à rien, mais lorsqu'elle n'est pas présente il y a un bug
	folder.reset();

	int mapNumber = 0;
	while( folder.findNext( mapName ) )
	{
		mapName.erase( mapName.find_last_of( "." ) );
		mapName.erase( mapName.find_last_of( "." ) );
		
		mapNames.push_back(mapName);
	}

	return mapNames;
}

};
