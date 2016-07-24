
#include <string>

using namespace std;

#include "Erreur.h"


namespace jkt
{

CErreur::CErreur(const stringstream &message) : JktException("CErreur", message) {
}

CErreur::CErreur(const string &message) : JktException("CErreur", message) {
}

}	// JktUtils
