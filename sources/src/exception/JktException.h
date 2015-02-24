#ifndef __JKT__JKTEXCEPTION_H
#define __JKT__JKTEXCEPTION_H

#include <exception>
#include <string>

class JktException : public std::exception {
	std::string _message;
public:
    JktException(void);
    JktException(const std::string& message);
    ~JktException(void);

    const char* what() const throw();
};

#endif
