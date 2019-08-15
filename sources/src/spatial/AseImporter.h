
#ifndef __JKT__ASEIMPORTER_H
#define __JKT__ASEIMPORTER_H

#include <string>

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"

class ConsoleView;

namespace jkt
{

class CMaterialTexture;
class CMap;

class AseImporter {
	static int importAse(void* arg);
	static void copieTexture(const std::string& fromFile, CMap *pMapASE, const boost::filesystem::path& mapDir, ConsoleView* console ) noexcept(false);
	
public:
	static void lanceImportAse(const std::string& aseFilename, ConsoleView* console);
};

}	// JktMoteur

#endif

