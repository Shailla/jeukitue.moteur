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

namespace jkt {

class JsonValue;
class JsonString;
class JsonNumber;
class JsonObject;

class JsonList : public JsonValue {
	std::vector<JsonValue*> _values;
public:
	JsonList();
	virtual ~JsonList();

	void addValue(JsonValue* value);
	JsonString& addString(const string& value);
	JsonNumber& addNumber(unsigned int value);
	JsonNumber& addNumber(int value);
	JsonNumber& addNumber(long value);
	JsonBoolean& addBoolean(bool value);
	JsonObject& addObject();
	JsonList& addList();

	JsonNumber* isJsonNumber() override;
	JsonString* isJsonString() override;
	JsonBoolean* isJsonBoolean() override;
	JsonObject* isJsonObject() override;
	JsonList* isJsonList() override;

	void toJson(std::stringstream& buffer) override;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_JSON_JSONLIST_H_ */
