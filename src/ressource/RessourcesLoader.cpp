
#include <string>
#include <iostream>

#include "RessourcesLoader.h"

using namespace std;

namespace JKT_PACKAGE_UTILS
{
int RessourcesLoader::nbrElements = 9;
char* RessourcesLoader::elementsNamesAndFolders[] =
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

		if(nbr != string::npos)
		{
			string element = file.substr(0,nbr);

			for(int i=0; i<nbrElements; i++) {
				if(!element.compare(elementsNamesAndFolders[i*2]))
				{
					std::cout << std::endl << "Ressource trouvee pour le fichier : " << file << std::endl;
					file.replace(0,nbr,elementsNamesAndFolders[i*2+1]);
					std::cout << "Qui devient : " << file << std::endl;
					bTrouve = true;
					break;
				}
			}
		}

		if(!bTrouve)
		{
			std::cerr << std::endl << "Ressource de type inconnu : " << file << std::endl;
			exit(1);
		}
	}

	return bTrouve;	// Indique si une ressource a été trouvée
}
};
