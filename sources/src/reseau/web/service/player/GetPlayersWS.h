/*
 * GetPlayersWS.h
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_SERVICE_PLAYERSWEBSERVICE_H_
#define SRC_RESEAU_WEB_SERVICE_PLAYERSWEBSERVICE_H_

#include "reseau/web/service/WebService.h"

namespace jkt {

class GetPlayersWS : public WebService {
	WebServiceResult getPlayersInCurrentMap();

public:
	GetPlayersWS();
	virtual ~GetPlayersWS();

	WebServiceResult execute(HttpRequest& request, const std::string& baseEndpoint, const std::string& serviceEndpoint) noexcept(false) override;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_SERVICE_PLAYERSWEBSERVICE_H_ */
