
#ifndef __JKT__PHOTO_H
#define __JKT__PHOTO_H

#include <string>

#include "sdl.h"

class CPhoto
{
	Uint8	*m_Ptr;
	int		m_X, m_Y;

public:
	CPhoto( int x, int y );
	~CPhoto();
	bool Save( std::string &nomFichier );
};

#endif