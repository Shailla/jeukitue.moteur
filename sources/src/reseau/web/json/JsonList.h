/*
 * JsonList.h
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_JSON_JSONLIST_H_
#define SRC_RESEAU_WEB_JSON_JSONLIST_H_

#include <vector>

#include "reseau/web/json/JsonValue.h"
#include "reseau/web/json/JsonPair.h"
#include "reseau/web/json/JsonObject.h"

namespace JktNet {

class JsonString;
class JsonObject;

class JsonList : public JsonValue {
	std::vector<JsonValue*> _values;
public:
	JsonList();
	virtual ~JsonList();

	JsonString& addString(const string& value);
	JsonObject& addObject();
	JsonList& addList();

	void toJson(std::stringstream& buffer) override;
};

} /* namespace JktNet */

#endif /* SRC_RESEAU_WEB_JSON_JSONLIST_H_ */
