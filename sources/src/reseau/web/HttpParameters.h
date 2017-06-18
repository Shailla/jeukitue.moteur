/*
 * HttpParameters.h
 *
 *  Created on: 11 juin 2017
 *      Author: Erwin
 */

#ifndef SRC_RESEAU_WEB_SERVICE_HTTPPARAMETERS_H_
#define SRC_RESEAU_WEB_SERVICE_HTTPPARAMETERS_H_

#include <map>

namespace jkt {

class HttpParameters {
	std::map<std::string, std::string> _parameters;
	std::string _head;
	bool _modified;

public:
	HttpParameters();
	HttpParameters(const HttpParameters& basicParameters);
	virtual ~HttpParameters();

	void addParameter(const char* name, const char* value);
	void addParameter(const char* name, const std::string& value);
	void addParameter(const char* name, int value);
	void addParameter(const char* name, long value);
	void addParameter(const char* name, size_t value);

	void clear();
	bool isEmpty() const;

	const std::string& str();
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_SERVICE_HTTPPARAMETERS_H_ */
