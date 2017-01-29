/*
 * FileUtils.cpp
 *
 *  Created on: 29 janv. 2017
 *      Author: Erwin
 */

#include <string>
#include <sstream>
#include <time.h>

using namespace std;

#include "util/FileUtils.h"

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

} /* namespace jkt */
