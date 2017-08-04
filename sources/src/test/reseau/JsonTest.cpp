/*
 * JsonTest.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <string>
#include <sstream>

#include "test/reseau/JsonTest.h"

#include "reseau/web/json/JsonPair.h"
#include "reseau/web/json/JsonValue.h"
#include "reseau/web/json/JsonString.h"
#include "reseau/web/json/JsonObject.h"
#include "reseau/web/json/JsonList.h"

using namespace std;
using namespace jkt;

namespace jkt {

JsonTest::JsonTest() : Test("JsonTest") {
}

JsonTest::~JsonTest() {
}

string JsonTest::getDescription() {
	return "Tests de l'API de conversion en JSON";
}

void JsonTest::test() {
	toJson();
	fromJson();
}

void JsonTest::toJson() {
	// Empty JSON object
	{
		log("Empty JSON object", __LINE__);
		JsonObject object;
		ASSERT_EQUAL("{}", object.toString(),  "");
	}

	// JSON string
	{
		log("JSON String", __LINE__);
		JsonObject object = JsonObject();
		object.addString("string", "coucou");
		ASSERT_EQUAL("{\"string\":\"coucou\"}", object.toString(),  "");
	}

	// Empty JSON list
	{
		log("JSON empty List", __LINE__);
		JsonObject object;
		object.addList("emptyList");
		ASSERT_EQUAL("{\"emptyList\":[]}", object.toString(),  "");
	}

	// JSON list with strings
	{
		log("JSON List with strings", __LINE__);
		JsonObject object;
		JsonList& list = object.addList("list");
		list.addString("str01");
		list.addString("str02");
		ASSERT_EQUAL("{\"list\":[\"str01\",\"str02\"]}", object.toString(),  "");
	}

	// Complex JSON data
	{
		log("JSON complex data", __LINE__);

		JsonObject root;

		JsonObject& valueObject1 = root.addObject("object1");
		valueObject1.addString("string01","str01");
		valueObject1.addString("string02","str02");

		JsonList& valueList = valueObject1.addList("list");
		valueList.addString("str11");
		valueList.addString("str12");

		JsonObject& object2 = valueList.addObject();
		object2.addString("string20", "str20");

		// En Json un objet n'est pas ordonn�, mais dans jkt la std::map utilis�e dans JsonObject impose l'ordre alphab�tique des cl�s
		ASSERT_EQUAL("{\"object1\":{\"list\":[\"str11\",\"str12\",{\"string20\":\"str20\"}],\"string01\":\"str01\",\"string02\":\"str02\"}}", root.toString(),  "");
	}
}

void JsonTest::fromJson() {
	log("From Json test", __LINE__);

	string json = "{\"object1\":{\"list\":[\"str11\",\"str12\",{\"string20\":true,\"string22\":false}],\"string01\":\"str01\",\"string02\":123}}";

	JsonObject* object = JsonObject::fromJson(json);

	ASSERT_EQUAL(json, object->toString(), "json => JsonObject => json should be identity operator");
}

} // JktTest
