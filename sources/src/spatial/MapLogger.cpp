/*
 * MapLogger.cpp
 *
 *  Created on: 7 mars 2016
 *      Author: VGDJ7997
 */

#include "util/Trace.h"

#include "spatial/MapLogger.h"

using namespace std;

namespace JktMoteur {

const char* MapLogger::LOG_INFO_PREFIX = 		"INFO   : ";
const char* MapLogger::LOG_ERROR_PREFIX = 		"ERROR  : ";

MapLogger::MapLogger(const string& mapFile) {
	string mapLogFile = string(mapFile).append(".log");

	_logFile.exceptions(std::ofstream::failbit|std::ofstream::badbit);

	try {
		_logFile.open(mapLogFile.c_str());
	}
	catch(std::ios_base::failure& exception) {
		LOGERROR(("Cannot open Map log file '%s'", mapLogFile.c_str()));
	}
}

void MapLogger::logInfo(const string& trace) {
	LOGINFO(("[Log lecture Map] %s", trace.c_str()));
	_logFile << endl << LOG_INFO_PREFIX << trace.c_str() << flush;
}

void MapLogger::logError(const string& trace) {
	LOGWARN(("[Log lecture Map] %s", trace.c_str()));
	_logFile << endl << LOG_INFO_PREFIX << trace.c_str() << flush;
}

MapLogger::~MapLogger() {
	// Fermeture propre du fichier de logs
	_logFile.flush();
	_logFile.close();
}

} /* namespace JktNet */
