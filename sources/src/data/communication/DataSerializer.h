/*
 * DataSerializer.h
 *
 *  Created on: 11 mars 2013
 *      Author: vgdj7997
 */

#ifndef DATASERIALIZER_H_
#define DATASERIALIZER_H_

#include <vector>
#include <sstream>

#include "data/communication/message/Changement.h"
#include "data/exception/DataCommunicationException.h"

class DataSerializer {
public:
	DataSerializer();
	virtual ~DataSerializer();

	static void toStream(std::vector<Changement*>& changements, std::ostringstream& out);
	static void fromStream(std::vector<Changement*>& changements, std::istringstream& out) throw(DataCommunicationException);
};

#endif /* DATASERIALIZER_H_ */
