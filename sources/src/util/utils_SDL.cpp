

#include <SDL.h>

#include "utils_SDL.h"

Uint32 util_SDL_getPixel( SDL_Surface *surface, int x, int y ) {	// Renvoie pixel positionn� x,y de surface
    int bpp = surface->format->BytesPerPixel;
		/* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch( bpp ) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

Uint8* util_SDL_SurfaceToArray( SDL_Surface *surface ) {
	Uint8 *array = new Uint8[surface->w * surface->h*4];

	SDL_PixelFormat *fmt = surface->format;
	Uint32 pixel;
	Uint8 red, green, blue;
	
	for(int i=0 ; i<surface->w ; i++) {	//mets les dimensions de l'image � un format compatible
		for(int j=0 ; j<surface->h ; j++) {
			pixel = util_SDL_getPixel(surface, i, (surface->h-j-1));
			SDL_GetRGB( pixel, fmt, &red, &green, &blue );
			array[(j*surface->w*4) + (i*4) + 0] = red;
			array[(j*surface->w*4) + (i*4) + 1] = green;
			array[(j*surface->w*4) + (i*4) + 2] = blue;
			array[(j*surface->w*4) + (i*4) + 3] = (Uint8)0;
		}
	}

	return array;
}
