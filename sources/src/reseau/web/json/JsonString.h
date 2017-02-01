/*
 * JsonString.h
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_JSON_JSONSTRING_H_
#define SRC_RESEAU_WEB_JSON_JSONSTRING_H_

#include <reseau/web/json/JsonValue.h>

namespace jkt {

class JsonString : public JsonValue {
	string _value;
public:
	JsonString(const string& value);
	virtual ~JsonString();

	const string& getValue() const;

	JsonNumber* isJsonNumber() override;
	JsonString* isJsonString() override;
	JsonBoolean* isJsonBoolean() override;
	JsonObject* isJsonObject() override;
	JsonList* isJsonList() override;

	void toJson(std::stringstream& buffer) override;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_JSON_JSONSTRING_H_ */
