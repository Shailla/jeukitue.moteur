/*
 * IMessageStream.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef IMESSAGESTREAM_H_
#define IMESSAGESTREAM_H_

#include <sstream>
#include <vector>
#include <string>

#include "util/types/AnyData.h"

class StreamUtils {
public:
	/** Normal exception to use to signal the normal end of a stream */
	class EndOfStreamException {
	};

	/** Abnormal exception to use to signal an error reading while a stream */
	class ReadStreamError {
	};

	static JktUtils::AnyData readData(std::istringstream& in);
	static void write(std::ostringstream& out, const JktUtils::AnyData& data);
	static void writeHumanReadable(std::ostringstream& out, const JktUtils::AnyData& data);

	static void read(std::istringstream& in, float& data);
	static void write(std::ostringstream& out, const float& data);

	static void read(std::istringstream& in, int& data);
	static void write(std::ostringstream& out, const int& data);

	static void read(std::istringstream& in, std::vector<int>& data);
	static void write(std::ostringstream& out, const std::vector<int>& data);
	static void writeHumanReadable(std::ostringstream& out, const std::vector<int>& data);

	static void read(std::istringstream& in, std::string& data);
	static void write(std::ostringstream& out, const std::string& data);
};

#endif /* IMESSAGESTREAM_H_ */
