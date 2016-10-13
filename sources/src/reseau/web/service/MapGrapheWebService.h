/*
 * MapGrapheWebService.h
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_SERVICE_MAPGRAPHEWEBSERVICE_H_
#define SRC_RESEAU_WEB_SERVICE_MAPGRAPHEWEBSERVICE_H_

#include "reseau/web/service/WebService.h"

namespace jkt {

class MapGrapheWebService : public WebService {
public:
	MapGrapheWebService();
	virtual ~MapGrapheWebService();

	WebServiceResult execute(const std::string& endpoint, const std::string& method);
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_SERVICE_MAPGRAPHEWEBSERVICE_H_ */
