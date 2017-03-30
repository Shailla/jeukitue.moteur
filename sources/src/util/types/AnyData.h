/*
 * AnyData.h
 *
 *  Created on: 17 mars 2013
 *      Author: Erwin
 */

#ifndef ANYDATA_H_
#define ANYDATA_H_

#include <string>
#include <sstream>

#include "util/exception/UnserializeException.h"

namespace jkt {

class AnyData {

protected:
	enum DataType {
		UNKNOWN_DATA_TYPE=0,
		INT_DATA_TYPE = 1,
		FLOAT_DATA_TYPE,
		STRING_DATA_TYPE
	};

private:
	DataType _type;

	int _int_value;
	float _float_value;
	std::string _string_value;

public:
	AnyData();
	AnyData(const int& value);
	AnyData(const float& value);
	AnyData(const std::string& value);
	AnyData(std::istringstream& in);
	virtual ~AnyData();

	static void unserialize(AnyData& data, std::istringstream& in) throw(UnserializeException);
	void serialize(std::ostringstream& out) const;

	/** Obtenir une description de la donnée */
	std::string toString() const;

	/** Obtenir une description de la donnée */
	void toString(std::ostringstream& out) const;

	AnyData& operator=(const AnyData& data);

	void update(const int& value);
	void update(const float& value);
	void update(const std::string& value);

	bool isInt() const;
	bool isFloat() const;
	bool isString() const;

	int getValueInt() const;
	float getValueFloat() const;
	std::string getValueString() const;
};

};

#endif /* ANYDATA_H_ */
