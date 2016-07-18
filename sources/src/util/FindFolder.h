#ifndef __JKT__FINDFOLDER_H
#define __JKT__FINDFOLDER_H

#include <string>

class CFindFolder {
public:
	static bool rmdir(const std::string& dir);				// Destruction d'un r�pertoire
	static bool isFolder(const std::string& directory);		// V�rifie si un �l�ment est un r�pertoire
	static int mkdir(const char*);
};

#endif
