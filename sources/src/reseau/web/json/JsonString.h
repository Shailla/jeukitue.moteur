/*
 * JsonString.h
 *
 *  Created on: 23 f�vr. 2016
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

	void toJson(std::stringstream& buffer) override;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_JSON_JSONSTRING_H_ */
