
/*************************************************************************************************

	Cette classe est une interface permettant la lecture des fichiers d'un répertoire
	avec le même système sous Windows comme sous Linux

 **************************************************************************************************/

#include <string>
#include <iostream>

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"

using namespace boost::filesystem;

#include "FindFolder.h"

using namespace std;

// Est-ce un répertoire
bool CFindFolder::isFolder(const string& directory) {
	path dir(directory);
	return is_directory(dir);
}

// Créer un répertoire
int CFindFolder::mkdir(const string& dir) {
	return create_directory(dir);
}
