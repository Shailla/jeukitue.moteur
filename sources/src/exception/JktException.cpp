#include "exception/JktException.h"

#include <string>

using namespace std;

JktException::JktException(const char* clazz) {
}

JktException::JktException(const char* clazz, const string& message):_clazz(clazz),_message(message) {
}

JktException::JktException(const char* clazz, const stringstream& message):_clazz(clazz),_message(message.str()) {
}

JktException::~JktException(void) {
}

const char* JktException::what() const throw() {
	return (_clazz + ": " + _message).c_str();
}
