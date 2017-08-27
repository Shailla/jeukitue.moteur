
#include <string>

#include "Erreur.h"

using namespace std;

namespace jkt
{

CErreur::CErreur(const stringstream &message) : JktException("CErreur", message) {
}

CErreur::CErreur(const string &message) : JktException("CErreur", message) {
}

const char* CErreur::what() const throw() {
	return JktException::what();
}

}	// JktUtils
