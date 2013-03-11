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

class IMessageStream : public std::istringstream {
public:
	IMessageStream(const std::string& str);

	IMessageStream& operator>>(std::vector<int>& data);
};

#endif /* IMESSAGESTREAM_H_ */
