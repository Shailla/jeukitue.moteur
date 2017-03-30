
/*************************************************************************************************

	Cette classe est une interface permettant la lecture des fichiers d'un r�pertoire
	avec le m�me syst�me sous Windows comme sous Linux

 **************************************************************************************************/

#include <string>
#include <iostream>

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"

using namespace boost::filesystem;

#include "FindFolder.h"

using namespace std;

// Est-ce un r�pertoire
bool CFindFolder::isFolder(const string& directory) {
	path dir(directory);
	return is_directory(dir);
}

// Cr�er un r�pertoire
int CFindFolder::mkdir(const string& dir) {
	return create_directory(dir);
}
