
#include <string>

using namespace std;

namespace JKT_PACKAGE_UTILS
{

class RessourcesLoader
{
	static int nbrElements;
	static char *elementsNamesAndFolders[];

public:
	static bool getFileRessource(string& file);
	static bool getFileRessource(const string& rep,string& file);
};
}