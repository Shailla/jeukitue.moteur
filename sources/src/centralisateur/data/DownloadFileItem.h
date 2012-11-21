#ifndef __JKT__DOWNLOADFILEITEM_H
#define __JKT__DOWNLOADFILEITEM_H

#include <string>

using namespace std;

enum CategoryDonwloadFile {
	MAP = 1,			// Map de jeu
	MAP_PLAYER = 2		// Map joueur
};

struct DownloadFileItem 
{
	long _identifier;
	string _nom;
	string _description;
	enum CategoryDonwloadFile _category;
	int _taille;
};

#endif
