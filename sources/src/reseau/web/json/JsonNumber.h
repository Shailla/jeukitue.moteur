/*
 * JsonString.h
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_JSON_JSONNUMBER_H_
#define SRC_RESEAU_WEB_JSON_JSONNUMBER_H_

#include <reseau/web/json/JsonValue.h>

namespace jkt {

class JsonNumber : public JsonValue {
	long _value;
public:
	JsonNumber(const unsigned int& value);
	JsonNumber(const int& value);
	JsonNumber(const long& value);
	virtual ~JsonNumber();

	void toJson(std::stringstream& buffer) override;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_JSON_JSONNUMBER_H_ */
