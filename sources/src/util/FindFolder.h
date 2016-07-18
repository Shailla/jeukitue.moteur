#ifndef __JKT__FINDFOLDER_H
#define __JKT__FINDFOLDER_H

#include <string>

class CFindFolder {
public:
	static bool rmdir(const std::string& dir);				// Destruction d'un répertoire
	static bool isFolder(const std::string& directory);		// Vérifie si un élément est un répertoire
	static int mkdir(const char*);
};

#endif
