#ifndef __JKT__RESSOURCESLOADER_H
#define __JKT__RESSOURCESLOADER_H

#include <string>
#include <vector>

extern const char* ASE_DIRECTORY;			// Répertoire des MAP de jeu
extern const char* ASE_EXTENSION;

extern const char* MAP_DIRECTORY;			// Répertoire des MAP de jeu
extern const char* MAP_EXTENSION;
extern const char* MAP_FILE_EXTENSION;		// Extention d'un fichier Map

extern const char* PLAYER_MAP_DIRECTORY;	// Répertoire des scripts des plugins
extern const char* PLUGINS_DIRECTORY;		// Répertoire des MAP de jeu
extern const char* TEMPORARY_DIRECTORY;	// Répertoire temporaire de travail

namespace jkt
{

class RessourcesLoader {
	static const char *elementsNamesAndFolders[];

public:
	static bool getFileRessource(std::string& file);
	static bool getRessource(const std::string& ressource, std::string& directory, std::string& filename);
	static bool getFileRessource(const std::string& rep, std::string& file);
	static std::vector<std::string> getMaps(void);
	static std::vector<std::string> getPlayerMaps(void);
};

}

#endif // __JKT__RESSOURCESLOADER_H
