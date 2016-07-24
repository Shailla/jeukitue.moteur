/*
 * IpUtils.h
 *
 *  Created on: 16 août 2013
 *      Author: Erwin
 */

#ifndef IPUTILS_H_
#define IPUTILS_H_

#include <string>

#include "SDL.h"
#include "SDL_net.h"

bool operator < (const IPaddress& adr1, const IPaddress& adr2);

namespace jkt {

class IpUtils {
public:
	IpUtils();
	virtual ~IpUtils();

	static std::string translateIp(IPaddress address);
	static std::string translateAddress(IPaddress address);
};

} /* namespace jkt */
#endif /* IPUTILS_H_ */
