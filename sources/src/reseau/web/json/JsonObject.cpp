/*
 * JsonString.cpp
 *
 *  Created on: 23 févr. 2016
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

JsonObject::JsonObject() {
}

JsonObject::~JsonObject() {
	for(JsonPair* pair : _pairs) {
		delete pair;
	}
}

JsonObject* JsonObject::fromJson(const string& json) {
	string var = json;

	unique_ptr<JsonObject> object = readObject(0, var);

	if(object) {
		return object.release();
	}
	else {
		return 0;
	}
}

unique_ptr<JsonObject> JsonObject::readObject(int depth, string& json) {
	smatch s;

	// Object
	cout << endl << indent(depth) << "READ OBJECT: " << json;

	bool result = regex_search(json, s, REGEX_OBJECT_BEGIN);

	if(result) {	// Si un objet a été trouvé
		cout << endl << indent(depth) << "Object trouvé";

		json = s.suffix();	// Confirm object begin was read

		unique_ptr<JsonObject> object(new JsonObject());

		// Lit toutes les paires clé/valeur
		unique_ptr<JsonPair> pair;
		bool first= true;

		do {
			pair = readPair(depth + 1, json);

			if(!pair) {
				if(first) {
					cout << endl << "pas de paire trouvé";
					break;
				}
				else {
					throw string("Apres un separateur on devrait avoir une paire");
				}
			}

			first = false;
			object->addPair(pair.release());

			result = regex_search(json, s, REGEX_PAIR_SEPARATOR);

			if(result) {
				cout << endl << indent(depth) << "Separateur trouvé";

				json = s.suffix();	// Confirm seperator was read
			}
			else {
				cout << endl << indent(depth) << "Fin des séparateurs";
				break;
			}
		} while(1);


		result = regex_search(json, s, REGEX_OBJECT_END);

		if(result) {	// Si un objet a été trouvé
			cout << endl << indent(depth) << "Fin objet";

			json = s.suffix();	// Confirm object end was read
		}
		else {
			throw string("La fin de l'objet est mauvaise");
		}

		return object;
	}
	else {
		cout << endl << indent(depth) << "Pas d'objet";
		return 0;
	}
}

unique_ptr<JsonList> JsonObject::readList(int depth, string& json) {
	smatch s;

	// Object
	cout << endl << indent(depth) << "READ LIST: " << json;

	bool result = regex_search(json, s, REGEX_LIST_BEGIN);

	if(result) {	// Si un objet a été trouvé
		cout << endl << indent(depth) << "Liste trouvée";

		unique_ptr<JsonList> list(new JsonList());
		json = s.suffix();	// Confirm list begin was read

		// Lit toutes les paires clé/valeur
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
					cout << endl << indent(depth) << "Pas de valeur trouvée";
					break;
				}
				else {
					throw string("Apres un separateur dans une liste on devrait avoir une valeur");
				}
			}

			first = false;
			list->addValue(value.release());

			result = regex_search(json, s, REGEX_PAIR_SEPARATOR);

			if(result) {
				cout << endl << indent(depth) << "Séparateur trouvé";

				json = s.suffix();	// Confirm list separator was read
			}
			else {
				break;
			}
		} while(1);

		result = regex_search(json, s, REGEX_LIST_END);

		if(result) {	// Si un objet a été trouvé
			cout << endl << indent(depth) << "Fin de liste";

			json = s.suffix();	// Confirm list end was read
		}
		else {
			throw string("La fin de liste est mauvaise");
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
		cout << endl << indent(depth) << "Paire trouvée";
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
			throw string("La paire n'a pas de valeur");
		}
	}
	else {
		cout << endl << indent(depth) << "pas de paire trouvée";
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
	_pairs.push_back(pair);
}

void JsonObject::addString(const string& name, const string& value) {
	JsonPair* pair = new JsonPair(name, (JsonValue*)new JsonString(value));
	_pairs.push_back(pair);
}

void JsonObject::addNumber(const string& name, const unsigned int value) {
	JsonPair* pair = new JsonPair(name, (JsonValue*)new JsonNumber(value));
	_pairs.push_back(pair);
}

void JsonObject::addNumber(const string& name, const int value) {
	JsonPair* pair = new JsonPair(name, (JsonValue*)new JsonNumber(value));
	_pairs.push_back(pair);
}

void JsonObject::addNumber(const string& name, const long value) {
	JsonPair* pair = new JsonPair(name, (JsonValue*)new JsonNumber(value));
	_pairs.push_back(pair);
}

void JsonObject::addBoolean(const string& name, const bool value) {
	JsonPair* pair = new JsonPair(name, (JsonValue*)new JsonBoolean(value));
	_pairs.push_back(pair);
}

JsonObject& JsonObject::addObject(const string& name) {
	JsonObject* var = new JsonObject();
	JsonPair* pair = new JsonPair(name, var);
	_pairs.push_back(pair);
	return *var;
}

JsonList& JsonObject::addList(const string& name) {
	JsonList* var = new JsonList();
	JsonPair* pair = new JsonPair(name, var);
	_pairs.push_back(pair);
	return *var;
}

vector<JsonPair*>& JsonObject::getPairs() {
	return _pairs;
}

JsonNumber* JsonObject::isJsonNumber() const {
	return 0;
}
JsonString* JsonObject::isJsonString() const {
	return 0;
}

JsonBoolean* JsonObject::isJsonBoolean() const {
	return 0;
}

void JsonObject::toJson(stringstream& buffer) {
	buffer << "{";

	bool first = true;

	for(JsonPair* pair : _pairs) {
		if(!first) {
			buffer << ",";
		}

		first = false;

		pair->toJson(buffer);
	}

	buffer << "}";
}

} /* namespace jkt */
