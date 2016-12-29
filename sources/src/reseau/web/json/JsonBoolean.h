/*
 * JsonBoolean.h
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_JSON_JSONBOOLEAN_H_
#define SRC_RESEAU_WEB_JSON_JSONBOOLEAN_H_

#include "reseau/web/json/JsonValue.h"

namespace jkt {

class JsonBoolean : public JsonValue {
	bool _value;
public:
	JsonBoolean(bool value);
	virtual ~JsonBoolean();

	bool getValue() const;

	const JsonNumber* isJsonNumber() const override;
	const JsonString* isJsonString() const override;
	const JsonBoolean* isJsonBoolean() const override;

	void toJson(std::stringstream& buffer) override;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_JSON_JSONBOOLEAN_H_ */
