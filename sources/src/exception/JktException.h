#ifndef __JKT__JKTEXCEPTION_H
#define __JKT__JKTEXCEPTION_H

#include <string>

class JktException {
	std::string _message;
public:
    JktException(void);
    JktException(const std::string& message);
    ~JktException(void);

    const std::string& getMessage() const;
};

#endif
