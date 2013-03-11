/*
 * DataSerializer.h
 *
 *  Created on: 11 mars 2013
 *      Author: vgdj7997
 */

#ifndef DATASERIALIZER_H_
#define DATASERIALIZER_H_

#include <vector>

#include "data/Changement.h"

class DataSerializer {
public:
	DataSerializer();
	virtual ~DataSerializer();

	static char* toBytes(std::vector<Changement*>& changements);
};

#endif /* DATASERIALIZER_H_ */
