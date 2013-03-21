
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

#include "util/Erreur.h"

#include "ifstreamWork.h"

ifstreamWork::ifstreamWork()
{
	bFin = false;	// Fin de fichier non atteinte
	next = 0;		// Pas de sous-fichier par défaut
	txt = NULL;
}

bool ifstreamWork::open( const string &nomFichier )
{
	string nomComplet( "./map/" );
	if( string( nomFichier.end()-4,nomFichier.end() ) == ".map" )
	{		// Inclusion d'un fichier map
		nomComplet += nomFichier;

		txt = new char[ nomComplet.length() ];
		strcpy( txt, nomComplet.c_str() );

		current.open( nomComplet.c_str() );		// Ouverture du fichier associé
		if( !current )
		{
			cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur ouverture fichier (" << nomComplet << ")";
			return false;
		}
		else
			return true;
	}

	cerr << endl << __FILE__ << ":" << __LINE__ << " Inclusion d'un fichier de format iconnu dans le fichier map (" + nomFichier + ")";
	return false;
}

ifstreamWork::~ifstreamWork()
{
	if( next )			// Si il y a un sous-fichier
		delete next;
	current.close();
}

char *ifstreamWork::nameFichier()
{	return txt;		}

inline bool ifstreamWork::operator !() {	return bFin;	}

/*	if( next )	// Si un (ou plusieurs) sous-fichier est ouvert
	{
		if( next->eof() )	// Si ce(s) sous-fichier(s) est fermé
			return bFin;	// Indique si on est aussi à la fin du fichier courrant
		else
			return false;	// S'il(s) est pas fermé indique qu'on a pas fini de lire les données
	}
	else
	{
		return bFin;
	}
}*/

ifstreamWork& ifstreamWork::operator>>( string &mot )
{
	if( next )		// Si on est dans un sous-fichier
	{		// Cas du sous-fichier
		if( !(next->operator >>( mot )) )
		{
			cout << "\nFIN DE SOUS FICHIER (" << next->txt << ") DETECTEE DANS " << txt;
			delete next;
			next = 0;
			if( !(operator>>(mot)) )
				bFin = true;		// Fin de fichier atteinte
		}
	}
	else
	{
			// Cas du fichier 'current'
		if( !(current >>(mot)) )
			bFin = true;			// Fin de fichier atteinte

		if( mot=="#include" )	// Si inclusion de fichier
		{
			string nomInclude;
			if( !(current>>(nomInclude)) )
			{
				bFin = true;		// Fin de fichier atteinte
				return *this;
			}

			cout << "\n#include " << nomInclude;

			next = new ifstreamWork();		// Ouverture du sous-fichier
			if( !next->open( nomInclude ) )
			{
				string erreur;
				erreur = "\nErreur : #include " + nomInclude + " -> Impossible";
				throw JktUtils::CErreur( 0, erreur );
			}

			if( !(next->operator >>( mot )) )
			{
				cout << "\nFIN DE SOUS FICHIER (" << next->txt << ") DETECTEE DANS " << txt;
				delete next;
				next = 0;
				if( !(operator>>(mot)) )
					bFin = true;		// Fin de fichier atteinte
			}
		}
	}

	return *this;
}

ifstreamWork& ifstreamWork::operator>>( float &val )
{
	if( next )		// Si on n'est pas dans un sous-fichier
	{
		if( !(next->operator >>( val )) )
		{
			cout << "\nFIN DE SOUS FICHIER (" << next->txt << ") DETECTEE DANS " << txt;
			delete next;
			next = 0;
			if( !(current>>val) )
				bFin = true;		// Fin de fichier atteinte
		}
	}
	else
	{
		if( !(current>>val) )
			bFin = true;
	}

	return *this;
}

ifstreamWork& ifstreamWork::operator>>( bool &val )
{
	if( next )		// Si on n'est pas dans un sous-fichier
	{
		if( !(next->operator >>( val )) )
		{
			cout << "\nFIN DE SOUS FICHIER (" << next->txt << ") DETECTEE DANS " << txt;
			delete next;
			next = 0;
			if( !(current>>(val)) )
				bFin = true;		// Fin de fichier atteinte
		}
	}
	else
	{
		if( !(current>>val) )
			bFin = true;
	}

	return *this;
}

ifstreamWork& ifstreamWork::operator>>( unsigned int &val )
{
	if( next )		// Si on n'est pas dans un sous-fichier
	{
		if( !(next->operator >>( val )) )
		{
			cout << "\nFIN DE SOUS FICHIER (" << next->txt << ") DETECTEE DANS " << txt;
			delete next;
			next = 0;
			if( !(current>>val) )
				bFin = true;		// Fin de fichier atteinte
		}
	}
	else
	{
		if( !(current>>val) )
			bFin = true;		// Fin de fichier atteinte
	}

	return *this;
}

ifstreamWork& ifstreamWork::operator>>( int &val )
{
	if( !next )		// Si on n'est pas dans un sous-fichier
	{
		if( !(next->operator >>( val )) )
		{
			cout << "\nFIN DE SOUS FICHIER (" << next->txt << ") DETECTEE DANS " << txt;
			delete next;
			next = 0;
			if( !(current>>val) )
				bFin = true;		// Fin de fichier atteinte
		}
	}
	else
	{
		if( !(current>>val) )
			bFin = true;		// Fin de fichier atteinte
	}

	return *this;
}
