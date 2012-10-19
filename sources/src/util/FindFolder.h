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

class CFindFolder
{
	char *m_Filter;			// Filtre de répertoire du style "c:/jeuquitue/"
	char *m_FilterOpt1;		// Filtre optinnnel de début du nom de fichier du style "fichier_"
	char *m_FilterOpt2;		// Filtre optionnel de fin du nom de fichier du style ".map"

#ifdef WIN32
	intptr_t m_hFile;
#elif defined(__linux__)
	DIR *m_Dir;
#endif

public:
		// Construteurs / destructeur
	CFindFolder(const char *filter, const char *optfilter1=NULL, const char *optfilter2=NULL);
	~CFindFolder();

	bool findNext(string &fichier);	// Trouve le prochain fichier du rép. correspondant aux filtres
	int nbr();				// Compte le nombre de fichier correspondant aux filtres dans le répertoire
	void reset();			// Redémarre la recherche à zéro

		// Interfaces inter-plateformes
	static int rmdir(const char *dir);		// Destruction d'un répertoire
	static bool isFolder(const string& directory);	// Vérifie si un élément est un répertoire
	static bool chmod(char const *path, bool read, bool write);	// Change le mode d'un fichier
	static int mkdir(const char*);
};

#endif
