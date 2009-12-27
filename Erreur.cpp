
#include <string>

using namespace std;

#include "Erreur.h"

namespace JKT_PACKAGE_UTILS
{

CErreur::CErreur( int c, const string &m )
{
	code = c;
	msg = m;
}

string CErreur::toString() const {
	string txt;
	if(msg.length() > 0) {
		txt += "msg='";
		txt += msg + "', ";
	}

	txt += "code=";
	txt += code;

	return txt;
}

}	// JKT_PACKAGE_UTILS
