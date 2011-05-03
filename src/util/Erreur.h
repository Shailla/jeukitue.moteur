#ifndef __JKT__ERREUR_H
#define __JKT__ERREUR_H

#include <string>

namespace JktUtils
{

class CErreur		// Classe pour l'émission d'erreurs par exception
{
public:
	std::string _msg;	// Message d'erreur
	int _code;			// Code de l'erreur

		// Constructeur
	CErreur(const int code, const std::string &msg);

	std::string toString() const;
};

}	// JktUtils

#endif

