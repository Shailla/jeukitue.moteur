#ifndef __JKT__IFSTREAMWORK_H
#define __JKT__IFSTREAMWORK_H


class ifstreamWork
{
	ifstream current;	// Fichier associé
	ifstreamWork *next;	// Sous-fichier associé
	bool bFin;			// Indique que la fin du fichier est atteinte
	char *txt;			// DEBUG
public:
		// Constructeur
	ifstreamWork();
	~ifstreamWork();

	bool open( const string &nomFichier );
	//bool eof();	
	bool operator!();
	char *nameFichier();
	ifstreamWork& operator>>( string &mot );
	ifstreamWork& operator>>( float &val );
	ifstreamWork& operator>>( bool &val );
	ifstreamWork& operator>>( unsigned int &val );
	ifstreamWork& operator>>( int &val );
};

#endif
