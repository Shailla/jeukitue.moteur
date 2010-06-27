
#include <string>
#include <vector>

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
	static std::vector<std::string> getMaps(void);
	static std::vector<std::string> getPlayerMaps(void);
};
}
