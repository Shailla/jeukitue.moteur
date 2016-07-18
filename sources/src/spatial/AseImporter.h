
#ifndef __JKT__ASEIMPORTER_H
#define __JKT__ASEIMPORTER_H

#include <string>

using namespace std;

class ConsoleAvancementView;

namespace JktMoteur
{

class CMaterialTexture;
class CMap;

class AseImporter
{
	static int importAse(void* arg);
	static bool copieTexture( CMaterialTexture *mat, CMap *pMapASE, string &nomRep, ConsoleAvancementView* console );
	
public:
	static bool supprimeRepertoire(const string& repName);
	static void lanceImportAse(const string& aseFilename, ConsoleAvancementView* console);
};

}	// JktMoteur

#endif

