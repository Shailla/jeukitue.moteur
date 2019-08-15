/*
 * JsonString.cpp
 *
 *  Created on: 23 f�vr. 2016
 *      Author: VGDJ7997
 */

#include <regex>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "reseau/web/json/JsonPair.h"
#include "reseau/web/json/JsonBoolean.h"
#include "reseau/web/json/JsonString.h"
#include "reseau/web/json/JsonNumber.h"
#include "reseau/web/json/JsonList.h"
#include "reseau/web/json/MalformedJsonException.h"
#include "reseau/web/json/BadFormatJsonException.h"

#include "reseau/web/json/JsonObject.h"

using namespace std;

namespace jkt {

regex JsonObject::REGEX_OBJECT_BEGIN("^\\s*\\{");
regex JsonObject::REGEX_OBJECT_END("^\\s*\\}");
regex JsonObject::REGEX_PAIR_KEY("^\\s*\\\"([^\\\"]+)\\\"\\s*\\:");
regex JsonObject::REGEX_PAIR_SEPARATOR("^\\s*,\\s*");
regex JsonObject::REGEX_STRING("^\\s*\\\"([^\\\"]+)\\\"");
regex JsonObject::REGEX_NUMBER("^\\s*(\\d+)");
regex JsonObject::REGEX_BOOLEAN("^\\s*(true|false)");
regex JsonObject::REGEX_LIST_BEGIN("^\\s*\\[");
regex JsonObject::REGEX_LIST_END("^\\s*\\]");
regex JsonObject::REGEX_BLANK("^\\s*$");

JsonObject::JsonObject() {
}

JsonObject::~JsonObject() {
}

JsonObject* JsonObject::fromJson(const string& json) {
	string var = json;

	cout << "BODY:'" << json << "'";


	unique_ptr<JsonObject> object = readObject(0, var);
	smatch s;

	if(!regex_search(var, s, REGEX_BLANK)) {		// S'il reste des caracr�res non-convertis dans le Json c'est qu'il �tait malform�
		throw MalformedJsonException("Tous les caract�res du Json n'ont pas pu �tre convertis");
	}
	else if(object) {
		return object.release();
	}
	else {
		return 0;
	}
}

JsonBoolean* JsonObject::getBoolean(const string& name, bool mandatory, bool strict) {
	JsonBoolean* object = 0;
	JsonValue* value;

	try {
		value = _pairs.at(name);
	}
	catch(out_of_range& exception) {
		value = 0;
	}

	if(value) {
		object = value->isJsonBoolean();

		if(strict && !object) {
			stringstream msg;
			msg << "Bad format, expected boolean for value named '" << name << "'";
			throw BadFormatJsonException(name);
		}
	}

	if(mandatory && !object) {
		stringstream msg;
		msg << "Missing mandatory value boolean named '" << name << "'";
		throw BadFormatJsonException(name);
	}

	return object;
}

JsonNumber* JsonObject::getNumber(const std::string& name, bool mandatory, bool strict) {
	JsonNumber* object = 0;
	JsonValue* value;

	try {
		value = _pairs.at(name);
	}
	catch(out_of_range& exception) {
		value = 0;
	}

	if(value) {
		object = value->isJsonNumber();

		if(strict && !object) {
			stringstream msg;
			msg << "Bad format, expected number for value named '" << name << "'";
			throw BadFormatJsonException(name);
		}
	}

	if(mandatory && !object) {
		stringstream msg;
		msg << "Missing mandatory value number named '" << name << "'";
		throw BadFormatJsonException(name);
	}

	return object;
}

JsonString* JsonObject::getString(const std::string& name, bool mandatory, bool strict) {
	JsonString* object = 0;
	JsonValue* value;

	try {
		value = _pairs.at(name);
	}
	catch(out_of_range& exception) {
		value = 0;
	}

	if(value) {
		object = value->isJsonString();

		if(strict && !object) {
			stringstream msg;
			msg << "Bad format, expected string for value named '" << name << "'";
			throw BadFormatJsonException(name);
		}
	}

	if(mandatory && !object) {
		stringstream msg;
		msg << "Missing mandatory value string named '" << name << "'";
		throw BadFormatJsonException(name);
	}

	return object;
}

JsonList* JsonObject::getList(const std::string& name, bool mandatory, bool strict) {
	JsonList* object = 0;
	JsonValue* value;

	try {
		value = _pairs.at(name);
	}
	catch(out_of_range& exception) {
		value = 0;
	}

	if(value) {
		object = value->isJsonList();

		if(strict && !object) {
			stringstream msg;
			msg << "Bad format, expected list for value named '" << name << "'";
			throw BadFormatJsonException(name);
		}
	}

	if(mandatory && !object) {
		stringstream msg;
		msg << "Missing mandatory value list named '" << name << "'";
		throw BadFormatJsonException(name);
	}

	return object;
}

JsonObject* JsonObject::getObject(const std::string& name, bool mandatory, bool strict) {
	JsonObject* object = 0;
	JsonValue* value;

	try {
		value = _pairs.at(name);
	}
	catch(out_of_range& exception) {
		value = 0;
	}

	if(value) {
		object = value->isJsonObject();

		if(strict && !object) {
			stringstream msg;
			msg << "Bad format, expected object for value named '" << name << "'";
			throw BadFormatJsonException(name);
		}
	}

	if(mandatory && !object) {
		stringstream msg;
		msg << "Missing mandatory value object named '" << name << "'";
		throw BadFormatJsonException(name);
	}

	return object;
}

JsonValue* JsonObject::getValue(const std::string& name, bool mandatory) {
	JsonValue* value;

	try {
		value = _pairs.at(name);
	}
	catch(out_of_range& exception) {
		value = 0;
	}

	if(mandatory && !value) {
		stringstream msg;
		msg << "Missing mandatory value named '" << name << "'";
		throw BadFormatJsonException(name);
	}

	return value;
}

unique_ptr<JsonObject> JsonObject::readObject(int depth, string& json) {
	smatch s;

	// Object
	cout << endl << indent(depth) << "READ OBJECT: '" << json << "'" << flush;

	bool result = regex_search(json, s, REGEX_OBJECT_BEGIN);

	if(result) {	// Si un objet a �t� trouv�
		cout << endl << indent(depth) << "Object trouv�";

		json = s.suffix();	// Confirm object begin was read

		unique_ptr<JsonObject> object(new JsonObject());

		// Lit toutes les paires cl�/valeur
		unique_ptr<JsonPair> pair;
		bool first= true;

		do {
			pair = readPair(depth + 1, json);

			if(!pair) {
				if(first) {
					cout << endl << "pas de paire trouv�";
					break;
				}
				else {
					throw MalformedJsonException("Apres un separateur on devrait avoir une paire");
				}
			}

			first = false;
			object->addPair(pair.release());

			result = regex_search(json, s, REGEX_PAIR_SEPARATOR);

			if(result) {
				cout << endl << indent(depth) << "Separateur trouv�";

				json = s.suffix();	// Confirm seperator was read
			}
			else {
				cout << endl << indent(depth) << "Fin des s�parateurs";
				break;
			}
		} while(1);


		result = regex_search(json, s, REGEX_OBJECT_END);

		if(result) {	// Si un objet a �t� trouv�
			cout << endl << indent(depth) << "Fin objet";

			json = s.suffix();	// Confirm object end was read
		}
		else {
			throw MalformedJsonException("La fin de l'objet est mauvaise");
		}

		return object;
	}
	else {
		cout << endl << indent(depth) << "Pas d'objet" << flush;
		return 0;
	}
}

unique_ptr<JsonList> JsonObject::readList(int depth, string& json) {
	smatch s;

	// Object
	cout << endl << indent(depth) << "READ LIST: " << json;

	bool result = regex_search(json, s, REGEX_LIST_BEGIN);

	if(result) {	// Si un objet a �t� trouv�
		cout << endl << indent(depth) << "Liste trouv�e";

		unique_ptr<JsonList> list(new JsonList());
		json = s.suffix();	// Confirm list begin was read

		// Lit toutes les paires cl�/valeur
		unique_ptr<JsonValue> value;
		bool first = true;

		do {
			value = readObject(depth + 1, json);

			if(!value) {
				value = readList(depth + 1, json);
			}

			if(!value) {
				value = readString(depth + 1, json);
			}

			if(!value) {
				value = readNumber(depth + 1, json);
			}

			if(!value) {
				value = readBoolean(depth + 1, json);
			}

			if(!value) {
				if(first) {
					cout << endl << indent(depth) << "Pas de valeur trouv�e";
					break;
				}
				else {
					throw MalformedJsonException("Apres un separateur dans une liste on devrait avoir une valeur");
				}
			}

			first = false;
			list->addValue(value.release());

			result = regex_search(json, s, REGEX_PAIR_SEPARATOR);

			if(result) {
				cout << endl << indent(depth) << "S�parateur trouv�";

				json = s.suffix();	// Confirm list separator was read
			}
			else {
				break;
			}
		} while(1);

		result = regex_search(json, s, REGEX_LIST_END);

		if(result) {	// Si un objet a �t� trouv�
			cout << endl << indent(depth) << "Fin de liste";

			json = s.suffix();	// Confirm list end was read
		}
		else {
			throw MalformedJsonException("La fin de liste est mauvaise");
		}

		return list;
	}
	else {
		cout << endl << indent(depth) << "Pas de liste";
		return 0;
	}
}

unique_ptr<JsonPair> JsonObject::readPair(int depth, string& json) {
	smatch s;

	// Object
	cout << endl << indent(depth) << "READ PAIR: " << json;

	bool result = regex_search(json, s, REGEX_PAIR_KEY);

	if(result) {
		cout << endl << indent(depth) << "Paire trouv�e";
		string pairName = s[1];

		json = s.suffix();	// Confirm pair name was read

		cout << " => " << pairName;

		unique_ptr<JsonValue> value;

		value = readObject(depth + 1, json);

		if(!value) {
			value = readList(depth + 1, json);
		}

		if(!value) {
			value = readString(depth + 1, json);
		}

		if(!value) {
			value = readNumber(depth + 1, json);
		}

		if(!value) {
			value = readBoolean(depth + 1, json);
		}

		if(value) {
			return unique_ptr<JsonPair>(new JsonPair(pairName, value.release()));
		}
		else {
			throw MalformedJsonException("La paire n'a pas de valeur");
		}
	}
	else {
		cout << endl << indent(depth) << "pas de paire trouv�e";
		return 0;
	}
}

unique_ptr<JsonString> JsonObject::readString(int depth, string& json) {
	string var = json;
	smatch s;

	// Object
	cout << endl << indent(depth) << "READ STRING: " << json;

	bool result = regex_search(json, s, REGEX_STRING);

	if(result) {
		string value = s[1];
		cout << endl << indent(depth) << "String lue : " << value;

		json = s.suffix();	// Confirm string value was read

		return unique_ptr<JsonString>(new JsonString(value));
	}
	else {
		cout << endl << indent(depth) << "Pas de string";
		return 0;
	}
}

unique_ptr<JsonNumber> JsonObject::readNumber(int depth, string& json) {
	string var = json;
	smatch s;

	// Object
	cout << endl << indent(depth) << "READ NUMBER: " << json;

	bool result = regex_search(json, s, REGEX_NUMBER);

	if(result) {
		int value = boost::lexical_cast<int>(s[1]);
		cout << endl << indent(depth) << "Nombre lue : " << value;

		json = s.suffix();	// Confirm number value was read

		return unique_ptr<JsonNumber>(new JsonNumber(value));
	}
	else {
		cout << endl << indent(depth) << "Pas de nombre";
		return 0;
	}
}

unique_ptr<JsonBoolean> JsonObject::readBoolean(int depth, string& json) {
	string var = json;
	smatch s;

	// Object
	cout << endl << indent(depth) << "READ BOOLEAN: " << json;

	bool result = regex_search(json, s, REGEX_BOOLEAN);

	if(result) {
		bool value;

		if(s[1] == "true") {
			value = true;
		}
		else {
			value = false;
		}

		cout << endl << indent(depth) << "Oui/non lu : " << value;

		json = s.suffix();	// Confirm number value was read

		return unique_ptr<JsonBoolean>(new JsonBoolean(value));
	}
	else {
		cout << endl << indent(depth) << "Pas de nombre";
		return 0;
	}
}

string JsonObject::indent(int depth) {
	string str;

	for(int i = 0 ; i < depth ; i++) {
		str += "   ";
	}

	return str;
}

void JsonObject::addPair(JsonPair* pair) {
	_pairs[pair->getName()] = pair->getValue();
}

void JsonObject::addString(const string& name, const string& value) {
	_pairs[name] = (JsonValue*)new JsonString(value);
}

void JsonObject::addNumber(const string& name, const unsigned int value) {
	_pairs[name] = (JsonValue*)new JsonNumber(value);
}

void JsonObject::addNumber(const string& name, const int value) {
	_pairs[name] = (JsonValue*)new JsonNumber(value);
}

void JsonObject::addNumber(const string& name, const long value) {
	_pairs[name] = (JsonValue*)new JsonNumber(value);
}

void JsonObject::addBoolean(const string& name, const bool value) {
	_pairs[name] = (JsonValue*)new JsonBoolean(value);
}

JsonObject& JsonObject::addObject(const string& name) {
	JsonObject* var = new JsonObject();
	_pairs[name] = (JsonValue*)var;

	return *var;
}

JsonList& JsonObject::addList(const string& name) {
	JsonList* var = new JsonList();
	_pairs[name] = (JsonValue*)var;

	return *var;
}

JsonNumber* JsonObject::isJsonNumber() {
	return 0;
}
JsonString* JsonObject::isJsonString() {
	return 0;
}

JsonBoolean* JsonObject::isJsonBoolean() {
	return 0;
}

JsonObject* JsonObject::isJsonObject() {
	return this;
}

JsonList* JsonObject::isJsonList() {
	return 0;
}

void JsonObject::toJson(stringstream& buffer) {
	buffer << "{";

	bool first = true;

	for(auto& var : _pairs) {
		if(!first) {
			buffer << ",";
		}

		first = false;

		buffer << "\"" << var.first << "\":";
		var.second->toJson(buffer);
	}

	buffer << "}";
}

} /* namespace jkt */
