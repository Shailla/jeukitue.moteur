
/*************************************************************************************************

	Cette classe est une interface permettant la lecture des fichiers d'un r�pertoire
	avec le m�me syst�me sous Windows comme sous Linux

**************************************************************************************************/

#include <string>
#include <string.h>
#include <iostream>
#ifdef WIN32
	#include <sys/stat.h>
	#include <io.h>
	#include <direct.h>
#elif defined(__linux__)
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <dirent.h>
#endif

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"
using namespace boost::filesystem;

#include "FindFolder.h"

// Pour rechercher c:/rep/fich*.map on aura par exemple :
//	CFindFolder( "c:/rep", "fich", ".map" );
//	ou
//	CFindFolder( "c:/rep", "fich, ".map" );
CFindFolder::CFindFolder(const char *filter, const char *optfilter1, const char *optfilter2) {
	if( !filter ) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " CFindFolder::CFindFolder() filtre null" << endl;
		m_Filter = new char[strlen("") + 1];
		strcpy( m_Filter, "" );
	}

#ifdef WIN32
	m_hFile = -2;	// Indique que la recherche est vierge
#elif defined(__linux__)
	m_Dir = 0;		// Indique que la recherche est vierge
#endif

	m_Filter = new char[ strlen(filter) +1 ];
	strcpy( m_Filter, filter );

	if( optfilter1 ) {
		m_FilterOpt1 = new char[ strlen(optfilter1) +1 ];
		strcpy( m_FilterOpt1, optfilter1 );
	}
	else {
		m_FilterOpt1 = NULL;
	}

	if( optfilter2 ) {
		m_FilterOpt2 = new char[ strlen(optfilter2) +1 ];
		strcpy( m_FilterOpt2, optfilter2 );
	}
	else {
		m_FilterOpt2 = NULL;
	}
}

void CFindFolder::reset()
{
#ifdef WIN32
	_findclose( m_hFile );
	m_hFile = -2;
#elif defined(__linux__)
	if( m_Dir != 0 )
		closedir( m_Dir );
	m_Dir = 0;
#endif
}

int CFindFolder::nbr() {	// Nombre de fichiers du r�pertoire correspondant aux filtres
	string name;
	int num = 0;

	while(findNext(name))
		num++;

	return num;
}

// Trouve le fichier suivant correspondant aux filtres du r�pertoire
bool CFindFolder::findNext(string &fichier)
{
#ifdef WIN32
	struct _finddata_t fileinfo;
	bool bTrouve1;		// Indique si un fichier correspondant au filtre 1 a �t� trouv�
	bool bTrouve2;		// Indique si un fichier correspondant au filtre 2 a �t� trouv�
	bool bTrouve;		// Indique si un fichier correspondant � tous les filtres a �t� trouv�
	bool bResult;

	do {
		bTrouve = false;
		bTrouve1 = false;
		bTrouve2 = false;
		bResult = false;

		if( m_hFile == -2 ) {
			string filter = m_Filter;
			if( string( filter.end()-1, filter.end() ) != "/" )
				filter += "/";

			filter += "*";

			m_hFile = _findfirst( filter.c_str(), &fileinfo );
			if( m_hFile != -1L )
				bResult = true;		// Un fichier a �t� trouv�
		}
		else {
			if( _findnext( m_hFile, &fileinfo ) != -1L )
				bResult = true;		// Un fichier a �t� trouv�
		}

		if( bResult )	// Si un fichier a �t� trouv�
		{
			fichier = fileinfo.name;

			if( m_FilterOpt1 )	// S'il y a un filtre optionnel
			{
					// Ex�cution du FILTRE 1
				if( fichier.size() >= strlen(m_FilterOpt1) )
					if( string( fichier.begin(), fichier.begin()+strlen(m_FilterOpt1) ) == m_FilterOpt1 )
						bTrouve1 = true;
			}

			if( bTrouve1 || !m_FilterOpt1 )	// S'il n'y a pas de 1er filtre ou que que celui-ci a trouv� quelque chose qui lui convient
			{
				if( m_FilterOpt2 )	// S'il y a un filtre optionnel
				{
						// Ex�cution du FILTRE 2
					if( fichier.size() >= strlen(m_FilterOpt2))
						if( string( fichier.end()-strlen(m_FilterOpt2), fichier.end() ) == m_FilterOpt2 )
							bTrouve2 = true;
				}
			}

			if( m_FilterOpt1 && m_FilterOpt2 )
			{
				if( bTrouve1 && bTrouve2 )
					bTrouve = true;
			}
			else if( m_FilterOpt1 )
			{
				if( bTrouve1 )
					bTrouve = true;
			}
			else if( m_FilterOpt2 )
			{
				if( bTrouve2 )
					bTrouve = true;
			}
			else
			{
				if( bResult )
					bTrouve = true;
			}
		}
	} while( bResult && !bTrouve );

	if( bTrouve )
	{
		return true;
	}
	else
	{
		fichier = "";
		return false;
	}

#elif defined(__linux__)

	struct dirent *dp;
	bool bTrouve1;		// Indique si un fichier correspondant au filtre 1 a �t� trouv�
	bool bTrouve2;		// Indique si un fichier correspondant au filtre 2 a �t� trouv�
	bool bTrouve;		// Indique si un fichier correspondant � tous les filtres a �t� trouv�
	bool bResult;

	do {
		bTrouve = false;
		bTrouve1 = false;
		bTrouve2 = false;
		bResult = false;

		if( m_Dir == 0 ) {
			string filter = m_Filter;
			if( string( filter.end()-1, filter.end() ) != "/" )
				filter += "/";

			m_Dir = opendir( filter.c_str() );
		}

		if( m_Dir ) {
			dp = readdir( m_Dir );
			if( dp )
				bResult = true;		// Un fichier a �t� trouv�
		}

		if( bResult )	// Si un fichier a �t� trouv�
		{
			fichier = dp->d_name;

			if( m_FilterOpt1 )	// S'il y a un filtre optionnel
			{
					// Ex�cution du FILTRE 1
				if( fichier.size() >= strlen(m_FilterOpt1) )
					if( string( fichier.begin(), fichier.begin()+strlen(m_FilterOpt1) ) == m_FilterOpt1 )
						bTrouve1 = true;
			}

			if( bTrouve1 || !m_FilterOpt1 )	// S'il n'y a pas de 1er filtre ou que que celui-ci a trouv� quelque chose qui lui convient
			{
				if( m_FilterOpt2 )	// S'il y a un filtre optionnel
				{
						// Ex�cution du FILTRE 2
					if( fichier.size() >= strlen(m_FilterOpt2))
						if( string( fichier.end()-strlen(m_FilterOpt2), fichier.end() ) == m_FilterOpt2 )
							bTrouve2 = true;
				}
			}

			if( m_FilterOpt1 && m_FilterOpt2 ) {
				if( bTrouve1 && bTrouve2 )
					bTrouve = true;
			}
			else if( m_FilterOpt1 ) {
				if( bTrouve1 )
					bTrouve = true;
			}
			else if( m_FilterOpt2 ) {
				if( bTrouve2 )
					bTrouve = true;
			}
			else {
				if( bResult )
					bTrouve = true;
			}
		}
	} while( bResult && !bTrouve );

	if( bTrouve ) {
		return true;
	}
	else {
		fichier = "";
		return false;
	}
#endif
}

CFindFolder::~CFindFolder()
{
	delete[] m_Filter;

	if( m_FilterOpt1 )
		delete[] m_FilterOpt1;

	if( m_FilterOpt2 )
		delete[] m_FilterOpt2;

#ifdef WIN32
	_findclose( m_hFile );
#elif defined(__linux__)
	if( m_Dir )
		closedir( m_Dir );
#endif
}

// Est-ce un r�pertoire
bool CFindFolder::isFolder(const string& directory) {
path dir(directory);
return is_directory(dir);
}

// Supprimer un r�pertoire
bool CFindFolder::rmdir(const string& dirToRemove) {
	path dir(dirToRemove);
	remove_all(dirToRemove);
	return !exists(dir);
}

// Cr�er un r�pertoire
int CFindFolder::mkdir(const char *dir) {
	return create_directory(dir);
}
