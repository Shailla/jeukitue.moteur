#ifndef __JKT__IFSTREAMMAP_H
#define __JKT__IFSTREAMMAP_H

#include <string>
#include <vector>

using namespace std;

#include "util/Tableau.h"

using namespace JKT_PACKAGE_UTILS;

namespace JKT_PACKAGE_MOTEUR3D
{
class CMaterial;

class CIfstreamMap:private ifstream
{
	CIfstreamMap *next;	// Sous-fichier associé
	bool bFin;			// Indique que la fin du fichier est atteinte
									// (Pour que chaque matériau soit associé au bon objet géo)
	vector<CMaterial*> *tabMat;	// Permet de lire le nombre de matériaux pour m_OffsetMateriaux
	string m_NomFichier;	// Nom de la Map sans extension de nom de fichier
	string m_NomFichierWithExtension;
public:
	static int m_OffsetMateriaux;		// Offset pour la lecture des matériaux
		// Constructeur
	CIfstreamMap( vector<CMaterial*> *tab );
	~CIfstreamMap();

	bool open(const string &nomFichier);
	bool operator!();
	const string& getFileFullName();
	const string& getFileName();

	int OffsetMateriaux();	// Renvoie l'offset matériaux actuel

	CIfstreamMap &operator >>( string &mot );
	CIfstreamMap &operator>>( float &val );
	CIfstreamMap &operator>>( bool &val );
	CIfstreamMap &operator>>( unsigned int &val );
	CIfstreamMap &operator>>( int &val );
	CIfstreamMap &operator>>( char &val );

	CIfstreamMap& get( char &ch );
};

}	// JKT_PACKAGE_MOTEUR3D

#endif

