/*
 * FileUtils.h
 *
 *  Created on: 29 janv. 2017
 *      Author: Erwin
 */

#ifndef SRC_UTIL_FILEUTILS_H_
#define SRC_UTIL_FILEUTILS_H_

#include <string>

namespace jkt {

class FileUtils {
	FileUtils();
public:
	virtual ~FileUtils();

	/** Génère un horodate sur date et heure courante, format year-month-day_hour-minute-second */
	static std::string horodatage();

	static bool checkFileExist(const std::string& name);
};

} /* namespace jkt */

#endif /* SRC_UTIL_FILEUTILS_H_ */
