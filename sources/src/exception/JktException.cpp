#include "exception/JktException.h"

#include <string>

using namespace std;

JktException::JktException(void) {
}

JktException::JktException(const string& message) {
	_message = message;
}

JktException::~JktException(void) {
}

const char* JktException::what() const throw() {
	return _message.c_str();
}
