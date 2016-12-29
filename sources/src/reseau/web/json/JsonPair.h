/*
 * JsonPair.h
 *
 *  Created on: 24 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_JSON_JSONPAIR_H_
#define SRC_RESEAU_WEB_JSON_JSONPAIR_H_

#include <string>
#include <sstream>

#include "reseau/web/json/JsonValue.h"

namespace jkt {

class JsonString;
class JsonObject;
class JsonList;

class JsonPair {
	std::string _name;
	JsonValue* _value;
public:
	JsonPair(const std::string& name, JsonValue* value);
	virtual ~JsonPair();

	const std::string& getName() const;
	const JsonValue* getValue() const;

	void toJson(std::stringstream& buffer);
	std::string toJson();
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_JSON_JSONPAIR_H_ */
