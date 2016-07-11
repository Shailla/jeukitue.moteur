
#include <string>
#include <iostream>
#ifdef WIN32
	#include <windows.h>
	#include <io.h>
	#include <direct.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>

using namespace std;

#include "photo.h"

CPhoto::CPhoto(int x, int y) {
	m_Ptr = new Uint8[x*y*3];
	m_X = x;
	m_Y = y;
}

CPhoto::~CPhoto() {
	if( m_Ptr )
		delete[] m_Ptr;		
}

bool CPhoto::Save( string &nomFichier ) {
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x00000000;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0x00000000;
#endif
	
	glReadPixels(  0, 0, m_X, m_Y, GL_RGB, GL_UNSIGNED_BYTE, m_Ptr );
	
		// Mirroir sur l'image pour la remettre dans le bon sens
	Uint8 a, b, c, d, e, f;
	for( int i=0 ; i< m_Y-i-1 ; i++ ) {
		for( int j=0 ; j < m_X ; j++ ) {
			a = m_Ptr[ (i*m_X + j)*3 ];
			b = m_Ptr[ (i*m_X + j)*3 + 1 ];
			c = m_Ptr[ (i*m_X + j)*3 + 2 ];
			
			d = m_Ptr[ ((m_Y-i-1)*m_X + j)*3 ];
			e = m_Ptr[ ((m_Y-i-1)*m_X + j)*3 + 1 ];
			f = m_Ptr[ ((m_Y-i-1)*m_X + j)*3 + 2 ];


			m_Ptr[ (i*m_X + j)*3 ] = d;
			m_Ptr[ (i*m_X + j)*3 + 1 ] = e;
			m_Ptr[ (i*m_X + j)*3 + 2 ] = f;
			
			m_Ptr[ ((m_Y-i-1)*m_X + j)*3 ] = a;
			m_Ptr[ ((m_Y-i-1)*m_X + j)*3 + 1 ] = b;
			m_Ptr[ ((m_Y-i-1)*m_X + j)*3 + 2 ] = c;
		}
	}
	
	SDL_Surface *surface = SDL_CreateRGBSurfaceFrom( m_Ptr, m_X, m_Y, 24, m_X*3, rmask, gmask, bmask, amask );	
	if( !surface ) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur (CPhoto::SDL_CreateRGBSurfaceFrom) : " << SDL_GetError();
		return false;
	}
	
	if( SDL_SaveBMP( surface, nomFichier.c_str() ) == -1 ) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur (CPhoto::SDL_SaveBMP) : " << SDL_GetError() << nomFichier;
		return false;
	}

	SDL_FreeSurface( surface );

	return true;	// Réussite
}
