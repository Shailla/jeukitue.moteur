#ifndef __JKT__FINDFOLDER_H
#define __JKT__FINDFOLDER_H

#include <string>

#include <boost/filesystem/operations.hpp> // includes boost/filesystem/path.hpp
#include <boost/filesystem/fstream.hpp>

class CFindFolder {
public:
	static bool isFolder(const std::string& directory);		// V�rifie si un �l�ment est un r�pertoire
	static int mkdir(const std::string& dir);
};

#endif
