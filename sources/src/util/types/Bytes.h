/*
 * Bytes.h
 *
 *  Created on: 25 juin 2013
 *      Author: vgdj7997
 */

#ifndef BYTES_H_
#define BYTES_H_

namespace JktUtils {

class Bytes {
	char* _bytes;
	int _size;
public:
	Bytes(char* bytes, int size);
	virtual ~Bytes();

	const char* getBytes() const;

	int size();
};

} /* namespace JktUtils */
#endif /* BYTES_H_ */
