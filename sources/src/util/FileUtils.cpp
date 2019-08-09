/*
 * FileUtils.cpp
 *
 *  Created on: 29 janv. 2017
 *      Author: Erwin
 */

#include <string>
#include <sstream>
#include <sys/stat.h>
#include <time.h>

#include "util/FileUtils.h"

using namespace std;

namespace jkt {

FileUtils::FileUtils() {
}

FileUtils::~FileUtils() {
}

string FileUtils::horodatage() {
	// Nom du fichier avec horodatage
	time_t t = time(0);
	struct tm* now = localtime(&t);

	ostringstream horodatage;
	horodatage << (now->tm_year+1900) << "-" << (now->tm_mon+1) << "-"<< now->tm_mday		// year-month-day
			<< "_"
			<< now->tm_hour << "-" << now->tm_min << "-" << now->tm_sec;				// hour-min-sec

	return horodatage.str();
}

bool FileUtils::checkFileExist(const std::string& name) {
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

} /* namespace jkt */
