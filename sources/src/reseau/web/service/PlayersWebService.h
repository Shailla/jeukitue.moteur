/*
 * PlayersWebService.h
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_SERVICE_PLAYERSWEBSERVICE_H_
#define SRC_RESEAU_WEB_SERVICE_PLAYERSWEBSERVICE_H_

#include "reseau/web/service/WebService.h"

namespace JktNet {

class PlayersWebService : public WebService {
public:
	PlayersWebService();
	virtual ~PlayersWebService();

	WebServiceResult execute(const std::string& endpoint, const std::string& method);
};

} /* namespace JktNet */

#endif /* SRC_RESEAU_WEB_SERVICE_PLAYERSWEBSERVICE_H_ */
