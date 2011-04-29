
#include <string>

using namespace std;

#include "Erreur.h"

namespace JktUtils
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

}	// JktUtils
