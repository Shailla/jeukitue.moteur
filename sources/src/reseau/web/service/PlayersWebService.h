/*
 * PlayersWebService.h
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_SERVICE_PLAYERSWEBSERVICE_H_
#define SRC_RESEAU_WEB_SERVICE_PLAYERSWEBSERVICE_H_

#include "reseau/web/service/WebService.h"

namespace jkt {

class PlayersWebService : public WebService {
	WebServiceResult getPlayersInCurrentMap();

public:
	PlayersWebService();
	virtual ~PlayersWebService();

	WebServiceResult execute(HttpServer::HTTP_METHODS method, const std::string& fullEndpoint, const std::string& baseEndpoint, const std::string& serviceEndpoint, const std::string& params) override;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_SERVICE_PLAYERSWEBSERVICE_H_ */
