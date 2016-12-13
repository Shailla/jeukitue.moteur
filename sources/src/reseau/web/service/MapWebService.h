/*
 * MapWebService.h
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_SERVICE_MAPWEBSERVICE_H_
#define SRC_RESEAU_WEB_SERVICE_MAPWEBSERVICE_H_

#include "reseau/web/service/WebService.h"

namespace jkt {

class MapWebService : public WebService {
public:
	MapWebService();
	virtual ~MapWebService();

	WebServiceResult execute(const std::string& method, const std::string& serviceEndpoint, const std::string& params);
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_SERVICE_MAPWEBSERVICE_H_ */
