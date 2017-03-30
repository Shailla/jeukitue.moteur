#ifndef __JKT__DOWNLOADFILEITEM_H
#define __JKT__DOWNLOADFILEITEM_H

#include <string>

enum CategoryDonwloadFile {
	MAP = 1,			// Map de jeu
	MAP_PLAYER = 2		// Map joueur
};

struct DownloadFileItem 
{
	long _identifier;
	std::string _nom;
	std::string _description;
	enum CategoryDonwloadFile _category;
	int _taille;
};

#endif
