#ifndef __JKT__IFSTREAMMAP_H
#define __JKT__IFSTREAMMAP_H

#include <string>
#include <vector>

#include "util/Tableau.h"

using namespace jkt;

namespace jkt
{
class CMaterial;

class CIfstreamMap:private std::ifstream {
	CIfstreamMap *next;	// Sous-fichier associé
	bool bFin;			// Indique que la fin du fichier est atteinte
									// (Pour que chaque matériau soit associé au bon objet géo)
	std::vector<CMaterial*> *tabMat;	// Permet de lire le nombre de matériaux pour m_OffsetMateriaux
	std::string m_NomFichier;	// Nom de la Map sans extension de nom de fichier
	std::string m_NomFichierWithExtension;
public:
	static int m_OffsetMateriaux;		// Offset pour la lecture des matériaux
		// Constructeur
	CIfstreamMap( std::vector<CMaterial*> *tab );
	~CIfstreamMap();

	bool open(const std::string &nomFichier);
	bool operator!();
	const std::string& getFileFullName();
	const std::string& getFileName();

	int OffsetMateriaux();	// Renvoie l'offset matériaux actuel

	CIfstreamMap &operator >>( std::string &mot );
	CIfstreamMap &operator>>( float &val );
	CIfstreamMap &operator>>( bool &val );
	CIfstreamMap &operator>>( unsigned int &val );
	CIfstreamMap &operator>>( int &val );
	CIfstreamMap &operator>>( char &val );

	CIfstreamMap& get( char &ch );
};

}	// JktMoteur

#endif

