#ifndef __JKT__ERREUR_H
#define __JKT__ERREUR_H

#include <string>

namespace JKT_PACKAGE_UTILS
{

class CErreur		// Classe pour l'émission d'erreurs par exception
{
public:
	std::string msg;	// Message d'erreur
	int code;			// Code de l'erreur

		// Constructeur
	CErreur(int c, const std::string &m);

	std::string toString() const;
};

}	// JKT_PACKAGE_UTILS

#endif

