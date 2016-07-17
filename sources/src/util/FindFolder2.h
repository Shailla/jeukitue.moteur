#ifndef __JKT__FINDFOLDER_H
#define __JKT__FINDFOLDER_H

#include <string>

#ifdef WIN32
#elif defined(__linux__)
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <dirent.h>
#endif

using namespace std;

class CFindFolder2
{
	char *m_Filter;			// Filtre de r�pertoire du style "c:/jeuquitue/"
	char *m_FilterOpt1;		// Filtre optinnnel de d�but du nom de fichier du style "fichier_"
	char *m_FilterOpt2;		// Filtre optionnel de fin du nom de fichier du style ".map"

#ifdef WIN32
	intptr_t m_hFile;
#elif defined(__linux__)
	DIR *m_Dir;
#endif

public:
		// Construteurs / destructeur
	CFindFolder2(const char *filter, const char *optfilter1=NULL, const char *optfilter2=NULL);
	~CFindFolder2();

	bool findNext(string &fichier);	// Trouve le prochain fichier du r�p. correspondant aux filtres
	int nbr();				// Compte le nombre de fichier correspondant aux filtres dans le r�pertoire
	void reset();			// Red�marre la recherche � z�ro

		// Interfaces inter-plateformes
	static int rmdir(const char *dir);		// Destruction d'un r�pertoire
	static bool isFolder(const string& directory);	// V�rifie si un �l�ment est un r�pertoire
	static bool chmod(char const *path, bool read, bool write);	// Change le mode d'un fichier
	static int mkdir(const char*);
};

#endif
