#ifndef __JKT__ERREUR_H
#define __JKT__ERREUR_H

#include <sstream>
#include <string>

#include "exception/JktException.h"

namespace JktUtils
{

class CErreur : public JktException {		// Classe pour l'émission d'erreurs par exception
public:
	CErreur(const std::stringstream &message);
	CErreur(const std::string &message);
};

}	// JktUtils

#endif

