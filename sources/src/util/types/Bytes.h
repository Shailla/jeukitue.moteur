/*
 * Bytes.h
 *
 *  Created on: 25 juin 2013
 *      Author: vgdj7997
 */

#ifndef BYTES_H_
#define BYTES_H_

#include <string>

namespace jkt {

class Bytes {
	char* _bytes;
	int _size;
public:
	Bytes(char* bytes, int size);
	Bytes(const std::string& str);
	virtual ~Bytes();

	const char* getBytes() const;

	int size();
};

} /* namespace jkt */
#endif /* BYTES_H_ */
