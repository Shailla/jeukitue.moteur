#ifndef __JKT__IFSTREAMWORK_H
#define __JKT__IFSTREAMWORK_H

#include <string>

class ifstreamWork
{
	std::ifstream current;	// Fichier associé
	ifstreamWork *next;	// Sous-fichier associé
	bool bFin;			// Indique que la fin du fichier est atteinte
	char *txt;			// DEBUG
public:
		// Constructeur
	ifstreamWork();
	~ifstreamWork();

	bool open( const std::string &nomFichier );
	//bool eof();	
	bool operator!();
	char *nameFichier();
	ifstreamWork& operator>>( std::string &mot );
	ifstreamWork& operator>>( float &val );
	ifstreamWork& operator>>( bool &val );
	ifstreamWork& operator>>( unsigned int &val );
	ifstreamWork& operator>>( int &val );
};

#endif
