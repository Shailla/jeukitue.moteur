
#ifndef __JKT__ASEIMPORTER_H
#define __JKT__ASEIMPORTER_H

#include <string>

using namespace std;

class ConsoleAvancementView;

namespace JKT_PACKAGE_MOTEUR3D
{

class CMaterialTexture;
class CMap;

class AseImporter
{
	static int importAse(void* arg);
	static bool copieTexture( CMaterialTexture *mat, CMap *pMapASE, string &nomRep, ConsoleAvancementView* console );
	
	static bool delDirectory( const string &path );
	static bool deleteOnlyFiles( string &fichier, const string &path );
public:
	static bool supprimeRepertoire(const string& repName);
	static void lanceImportAse(const string& aseFilename, ConsoleAvancementView* console);
};

}	// JKT_PACKAGE_MOTEUR3D

#endif

