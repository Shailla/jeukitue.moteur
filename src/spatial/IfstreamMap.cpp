
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "ressource/RessourcesLoader.h"
#include "util/Erreur.h"
#include "util/Tableau.cpp"
#include "spatial/materiau/Material.h"

#include "spatial/IfstreamMap.h"

namespace JKT_PACKAGE_MOTEUR3D
{
int CIfstreamMap::m_OffsetMateriaux = 0;

CIfstreamMap::CIfstreamMap( vector<CMaterial*> *tab )
{
	bFin = false;	// Fin de fichier non atteinte
	next = 0;		// Pas de sous-fichier par défaut

	tabMat = tab;
}

CIfstreamMap::~CIfstreamMap(void)
{
	if( next )			// Si il y a un sous-fichier
		delete next;
	ifstream::close();
}

int CIfstreamMap::OffsetMateriaux()	// Renvoie l'offset matériaux actuel
{
	return m_OffsetMateriaux;
}
const string& CIfstreamMap::getFileFullName() {
	return m_NomFichierWithExtension;
}
const string& CIfstreamMap::getFileName() {
	return m_NomFichier;
}
bool CIfstreamMap::open(const string &nomFichier)
{
		// Construction du nom complet du fichier Map
	m_NomFichier = nomFichier;

	if(!RessourcesLoader::getFileRessource(m_NomFichier)) {
		m_NomFichier = string(".\\map\\") + m_NomFichier;
	}

		// Sauve le nom de fichier en enlevant l'extension
	int nbr = (int)m_NomFichier.find_last_of('.');
	m_NomFichier.replace(nbr,string::npos,"");
	m_NomFichierWithExtension = m_NomFichier + ".map.xml";

		// Ouverture du fichier
	ifstream::open( m_NomFichierWithExtension.c_str() );	// Ouverture du fichier associé
	if( ifstream::operator!() )		// L'ouverture de fichier s'est-elle bien passée
	{
		cerr << "\nErreur ouverture fichier 2 (" << m_NomFichierWithExtension << ")";
		return false;
	}
	else
	{
		return true;
	}
}

inline bool CIfstreamMap::operator !() {	return bFin;	}

CIfstreamMap& CIfstreamMap::operator >>( string &mot )
{
	if( next )		// Si on est dans un sous-fichier
	{		// Cas du sous-fichier
		if( !(next->operator >>( mot )) )
		{
			cout << "\nFIN DE SOUS FICHIER (" << next->getFileFullName() << ") DETECTEE DANS (" << getFileFullName() << ")";
			delete next;
			next = 0;
			if( !(CIfstreamMap::operator >>(mot)) )
				bFin = true;		// Fin de fichier atteinte
		}
	}
	else
	{		// Cas du fichier 'current'
		if( !((*(static_cast<ifstream *>(this))) >> mot) )
			bFin = true;			// Fin de fichier atteinte

		if( mot=="#include" )	// Si inclusion de fichier
		{
			string nomInclude;
			if( !((*(static_cast<ifstream *>(this))) >> nomInclude) )
				bFin = true;		// Fin de fichier atteinte

			cout << "\n#include " << nomInclude;

			next = new CIfstreamMap( tabMat );		// Ouverture du sous-fichier
			if( !next->open( nomInclude ) )
			{
				string erreur;
				erreur = "\nErreur : #include " + nomInclude + " -> Impossible";
				throw CErreur( 0, erreur );
			}

			if( !(next->operator >>( mot )) )
			{
				cout << "\nFIN DE SOUS FICHIER (" << next->getFileFullName() << ") DANS (" << getFileFullName() << ")";
				delete next;
				next = 0;
				if( !(operator >>(mot)) )
					bFin = true;		// Fin de fichier atteinte
			}
		}
		else if( mot=="Start" )
		{
			cout << " ..Start";
			m_OffsetMateriaux = (unsigned int)tabMat->size();	// Mise à jour de l'offset matériaux
			operator >>( mot );
		}
	}

	return *this;
}

CIfstreamMap& CIfstreamMap::operator>>( float &val )
{
	if( next )		// Si on n'est pas dans un sous-fichier
	{
		if( !(next->operator >>( val )) )
		{
			cout << "\nFIN DE SOUS FICHIER (" << next->getFileFullName() << ") DETECTEE DANS " << getFileFullName();
			delete next;
			next = 0;
			if( !(*static_cast<ifstream *> (this) >> val) )
				bFin = true;		// Fin de fichier atteinte
		}
	}
	else
	{
		if( !(*static_cast<ifstream *> (this) >> val) )
			bFin = true;
	}

	return *this;
}

CIfstreamMap& CIfstreamMap::operator>>( bool &val )
{
	if( next )		// Si on n'est pas dans un sous-fichier
	{
		if( !(next->operator >>( val )) )
		{
			cout << "\nFIN DE SOUS FICHIER (" << next->getFileFullName() << ") DETECTEE DANS " << getFileFullName();
			delete next;
			next = 0;
			if( !(*static_cast<ifstream *> (this) >> val) )
				bFin = true;		// Fin de fichier atteinte
		}
	}
	else
	{
		if( !(*static_cast<ifstream *> (this) >> val) )
			bFin = true;
	}

	return *this;
}

CIfstreamMap& CIfstreamMap::operator>>( unsigned int &val )
{
	if( next )		// Si on n'est pas dans un sous-fichier
	{
		if( !(next->operator >>( val )) )
		{
			cout << "\nFIN DE SOUS FICHIER (" << next->getFileFullName() << ") DETECTEE DANS " << getFileFullName();
			delete next;
			next = 0;
			if( !(*static_cast<ifstream *> (this) >> val) )
				bFin = true;		// Fin de fichier atteinte
		}
	}
	else
	{
		if( !(*static_cast<ifstream *> (this) >> val) )
			bFin = true;
	}

	return *this;
}

CIfstreamMap& CIfstreamMap::operator>>( int &val )
{
	if( next )		// Si on n'est pas dans un sous-fichier
	{
		if( !(next->operator >>( val )) )
		{
			cout << "\nFIN DE SOUS FICHIER (" << next->getFileFullName() << ") DETECTEE DANS " << getFileFullName();
			delete next;
			next = 0;
			if( !(*static_cast<ifstream *> (this) >> val) )
				bFin = true;		// Fin de fichier atteinte
		}
	}
	else
	{
		if( !(*static_cast<ifstream *> (this) >> val) )
			bFin = true;
	}

	return *this;
}

CIfstreamMap& CIfstreamMap::operator>>( char &val )
{
	if( next )		// Si on n'est pas dans un sous-fichier
	{
		if( !(next->operator >>( val )) )
		{
			cout << "\nFIN DE SOUS FICHIER (" << next->getFileFullName() << ") DETECTEE DANS " << getFileFullName();
			delete next;
			next = 0;
			if( !(*static_cast<ifstream *> (this) >> val) )
				bFin = true;		// Fin de fichier atteinte
		}
	}
	else
	{
		if( !(*static_cast<ifstream *> (this) >> val) )
			bFin = true;
	}

	return *this;
}

CIfstreamMap& CIfstreamMap::get( char &ch )
{
	if( next )		// Si on n'est pas dans un sous-fichier
	{
		if( !(next->get( ch )) )
		{
			cout << "\nFIN DE SOUS FICHIER (" << next->getFileFullName() << ") DETECTEE DANS " << getFileFullName();
			delete next;
			next = 0;
			if( !(static_cast<ifstream *>(this)->get( ch ) ) )
				bFin = true;		// Fin de fichier atteinte
		}
	}
	else
	{
		if( !(static_cast<ifstream *>(this)->get( ch ) ) )
			bFin = true;
	}

	return *this;
}

}	// JKT_PACKAGE_MOTEUR3D
