#ifndef __JKT__TCPUTILS_H
#define __JKT__TCPUTILS_H

#include "exception/centralisateur/CentralisateurTcpException.h"

namespace jkt
{

/**************************************************/
/* Write a 64 bits value to network packet buffer */
/**************************************************/

#if !SDL_DATA_ALIGNED
#define JktNet_Write64(value, areap) 	\
	*(Uint64 *)(areap) = SDL_SwapBE64(value);
#else
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define JktNet_Write64(value, areap) 	\
do					\
{					\
	Uint8 *area = (Uint8 *)(areap);	\
	area[0] = (value >> 56) & 0xFF;	\
	area[1] = (value >> 48) & 0xFF;	\
	area[2] = (value >> 40) & 0xFF;	\
	area[3] = (value >> 32) & 0xFF;	\
	area[4] = (value >> 24) & 0xFF;	\
	area[5] = (value >> 16) & 0xFF;	\
	area[6] = (value >>  8) & 0xFF;	\
	area[7] =  value        & 0xFF;	\
} while ( 0 )
#else
#define JktNet_Write64(value, areap) 	\
do					\
{					\
	Uint8 *area = (Uint8 *)(areap);	\
	area[7] = (value >> 56) & 0xFF;	\
	area[6] = (value >> 48) & 0xFF;	\
	area[5] = (value >> 40) & 0xFF;	\
	area[4] = (value >> 32) & 0xFF;	\
	area[3] = (value >> 24) & 0xFF;	\
	area[2] = (value >> 16) & 0xFF;	\
	area[1] = (value >>  8) & 0xFF;	\
	area[0] =  value        & 0xFF;	\
} while ( 0 )
#endif
#endif


/***************************************************/
/* Read a 64 bits value from network packet buffer */
/***************************************************/

#if !SDL_DATA_ALIGNED
#define JktNet_Read64(areap) 		\
	(SDL_SwapBE64(*(Uint64 *)(areap)))
#else
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define JktNet_Read64(areap) 		\
	((((Uint8 *)areap)[0] << 46) |  \
	(((Uint8 *)areap)[1] << 48)  |  \
	(((Uint8 *)areap)[2] <<  40) |  \
	(((Uint8 *)areap)[3] <<  32) |  \
	(((Uint8 *)areap)[4] <<  24) |  \
	(((Uint8 *)areap)[5] <<  16) |  \
	(((Uint8 *)areap)[6] <<  8)  |  \
	((Uint8 *)areap)[7] <<  0)
#else
#define JktNet_Read64(areap) 		\
	((((Uint8 *)areap)[7] << 56) |  \
	(((Uint8 *)areap)[6] << 48)  |  \
	(((Uint8 *)areap)[5] <<  40) |  \
	(((Uint8 *)areap)[4] <<  32) |  \
	(((Uint8 *)areap)[3] <<  24) |  \
	(((Uint8 *)areap)[2] <<  16) |  \
	(((Uint8 *)areap)[1] <<  8)  |  \
	((Uint8 *)areap)[0] <<  0)
#endif
#endif /* !SDL_DATA_ALIGNED */


class CTcpUtils
{
public:
	static int loadInteger(const TCPsocket socket) noexcept(false);
	static long loadLong(const TCPsocket socket) noexcept(false);
	static char* loadString(const TCPsocket socket) noexcept(false);
	static char* loadCharsBySize(const TCPsocket socket, const int size) noexcept(false);
};

}

#endif
