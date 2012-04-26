#ifndef __JKT__RESSOURCESLOADER_H
#define __JKT__RESSOURCESLOADER_H

#include <string>
#include <vector>
using namespace std;

namespace JktUtils
{

class RessourcesLoader
{
	static int nbrElements;
	static const char *elementsNamesAndFolders[];

public:
	static bool getFileRessource(string& file);
	static bool getFileRessource(const string& rep, string& file);
	static vector<string> getMaps(void);
	static vector<string> getPlayerMaps(void);
};
}

#endif
