/*
 * JsonValue.h
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_JSON_JSONVALUE_H_
#define SRC_RESEAU_WEB_JSON_JSONVALUE_H_

#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace jkt {

class JsonBoolean;
class JsonNumber;
class JsonString;

class JsonValue {
public:
	JsonValue();
	virtual ~JsonValue();

	virtual const JsonNumber* isJsonNumber() const = 0;
	virtual const JsonString* isJsonString() const = 0;
	virtual const JsonBoolean* isJsonBoolean() const = 0;

	virtual void toJson(std::stringstream& buffer) = 0;
	std::string toString();
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_JSON_JSONVALUE_H_ */
