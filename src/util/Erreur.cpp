
#include <string>

using namespace std;

#include "Erreur.h"

namespace JktUtils
{

CErreur::CErreur(int code, const string &msg) {
	_code = code;
	_msg = msg;
}

string CErreur::toString() const {
	string txt = "[";

	// Code
	txt += "code='";
	txt += _code;
	txt += "'";

	// Message
	if(_msg.length() > 0) {
		txt += ", msg='";
		txt += _msg + "'";
	}

	txt += "]";

	return txt;
}

}	// JktUtils
