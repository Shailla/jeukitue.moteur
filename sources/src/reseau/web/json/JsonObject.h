/*
 * JsonObject.h
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_JSON_JSONOBJECT_H_
#define SRC_RESEAU_WEB_JSON_JSONOBJECT_H_

#include <regex>
#include <vector>
#include <map>
#include <string>

#include "reseau/web/json/JsonValue.h"
#include "reseau/web/json/JsonPair.h"
#include "reseau/web/json/JsonList.h"
#include "reseau/web/json/MalformedJsonException.h"

namespace jkt {

class JsonString;
class JsonList;
class JsonNumber;

class JsonObject : public JsonValue {
	static std::regex REGEX_OBJECT_BEGIN;
	static std::regex REGEX_OBJECT_END;
	static std::regex REGEX_PAIR_KEY;
	static std::regex REGEX_PAIR_SEPARATOR;
	static std::regex REGEX_STRING;
	static std::regex REGEX_NUMBER;
	static std::regex REGEX_BOOLEAN;
	static std::regex REGEX_LIST_BEGIN;
	static std::regex REGEX_LIST_END;
	static std::regex REGEX_BLANK;

	static std::string indent(int depth);
	static std::unique_ptr<JsonObject> readObject(int depth, std::string& json) throw(MalformedJsonException);
	static std::unique_ptr<JsonPair> readPair(int depth, std::string& json) throw(MalformedJsonException);
	static std::unique_ptr<JsonString> readString(int depth, std::string& json) throw(MalformedJsonException);
	static std::unique_ptr<JsonNumber> readNumber(int depth, std::string& json) throw(MalformedJsonException);
	static std::unique_ptr<JsonBoolean> readBoolean(int depth, std::string& json) throw(MalformedJsonException);
	static std::unique_ptr<JsonList> readList(int depth, std::string& json) throw(MalformedJsonException);

	std::map<std::string, JsonValue*> _pairs;

public:
	JsonObject();
	virtual ~JsonObject();

	void addPair(JsonPair* pair);
	void addString(const std::string& name, const std::string& value);
	void addBoolean(const std::string& name, const bool value);
	void addNumber(const std::string& name, const unsigned int value);
	void addNumber(const std::string& name, const int value);
	void addNumber(const std::string& name, const long value);
	JsonObject& addObject(const std::string& name);
	JsonList& addList(const std::string& name);

	JsonNumber* isJsonNumber() override;
	JsonString* isJsonString() override;
	JsonBoolean* isJsonBoolean() override;
	JsonObject* isJsonObject() override;
	JsonList* isJsonList() override;

	void toJson(std::stringstream& buffer) override;
	static JsonObject* fromJson(const std::string& json) throw(MalformedJsonException);

	JsonValue* getValue(const std::string& name);
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_JSON_JSONSTRING_H_ */
