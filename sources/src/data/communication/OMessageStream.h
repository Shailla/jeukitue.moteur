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

std::ostringstream& operator<<(std::ostringstream& out, const std::vector<int>& data);

#endif /* OMESSAGESTREAM_H_ */
