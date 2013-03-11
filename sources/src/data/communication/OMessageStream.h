/*
 * OMessageStream.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef OMESSAGESTREAM_H_
#define OMESSAGESTREAM_H_

#include <sstream>
#include <vector>

class OMessageStream : public std::ostringstream {
public:
	OMessageStream& operator<<(std::vector<int>& data);
};

#endif /* OMESSAGESTREAM_H_ */
