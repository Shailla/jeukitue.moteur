#ifndef __JKT__JKTEXCEPTION_H
#define __JKT__JKTEXCEPTION_H

#include <exception>
#include <string>

class JktException : public std::exception {
	std::string _clazz;
	std::string _message;
public:
    JktException(const char* clazz);
    JktException(const char* clazz, const std::string& message);
    ~JktException(void);

    const char* what() const throw();
};

#endif
