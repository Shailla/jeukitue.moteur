#ifndef __JKT__JKTEXCEPTION_H
#define __JKT__JKTEXCEPTION_H

#include <exception>
#include <sstream>
#include <string>

class JktException : public std::exception {
	std::string _clazz;
	std::string _message;
public:
    JktException(const char* clazz);
    JktException(const char* clazz, const std::string& message);
    JktException(const char* clazz, const std::stringstream& message);
    ~JktException(void);

    const char* getMessage() const;
    const char* what() const throw();
};

#endif
