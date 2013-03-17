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

const string& JktException::getMessage() const {
	return _message;
}

