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

std::istringstream& operator>>(std::istringstream& in, std::vector<int>& data);
std::ostringstream& operator<<(std::ostringstream& out, std::vector<int>& data);

#endif /* IMESSAGESTREAM_H_ */
