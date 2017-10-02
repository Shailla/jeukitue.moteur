/*
 * MapLogger.h
 *
 *  Created on: 7 mars 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_SPATIAL_MAPLOGGER_H_
#define SRC_SPATIAL_MAPLOGGER_H_

#include <fstream>
#include <string>
#include <sstream>

namespace jkt {

class MapLogger {
	static const char* LOG_INFO_PREFIX;
	static const char* LOG_ERROR_PREFIX;

	std::ofstream _logFile;

public:
	MapLogger(const std::string& mapFile);
	virtual ~MapLogger();

	void logInfo(const std::string& trace);
	void logInfo(const std::stringstream& trace);

	void logError(const std::string& trace);
	void logError(const std::stringstream& trace);
};

} /* namespace jkt */

#endif /* SRC_SPATIAL_MAPLOGGER_H_ */
