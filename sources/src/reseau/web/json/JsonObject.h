/*
 * JsonObject.h
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_JSON_JSONOBJECT_H_
#define SRC_RESEAU_WEB_JSON_JSONOBJECT_H_

#include <vector>

#include "reseau/web/json/JsonValue.h"
#include "reseau/web/json/JsonPair.h"
#include "reseau/web/json/JsonList.h"

namespace JktNet {

class JsonString;
class JsonList;

class JsonObject : public JsonValue {
	std::vector<JsonPair*> _pairs;
public:
	JsonObject();
	virtual ~JsonObject();

	void addString(const std::string& name, const std::string& value);
	JsonObject& addObject(const std::string& name);
	JsonList& addList(const std::string& name);

	void toJson(std::stringstream& buffer) override;
};

} /* namespace JktNet */

#endif /* SRC_RESEAU_WEB_JSON_JSONSTRING_H_ */
