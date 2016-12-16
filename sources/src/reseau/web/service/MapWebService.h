/*
 * MapWebService.h
 *
 *  Created on: 25 f�vr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_SERVICE_MAPWEBSERVICE_H_
#define SRC_RESEAU_WEB_SERVICE_MAPWEBSERVICE_H_

#include <regex>

#include "reseau/web/json/JsonObject.h"

#include "reseau/web/service/WebService.h"

namespace jkt {

class CMap;

class MapWebService : public WebService {
	static string ID;
	static string NAME;
	static string TYPE;
	static string SELECTED;

	static string MAP;
	static string GEO;
	static string LIGHT;
	static string ENTRYPOINT;

	static std::regex RG_GET_ELEMENT_SERVICE;

	WebServiceResult getMapList();
	WebServiceResult getCurrentMapGraphe();
	WebServiceResult getElement();

	void jisonifyMapGraphe(CMap* map, JsonObject& mapGraphe);

public:
	MapWebService();
	virtual ~MapWebService();

	WebServiceResult execute(HttpServer::HTTP_METHODS method, const std::string& fullEndpoint, const std::string& baseEndpoint, const std::string& serviceEndpoint, const std::string& params) override;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_SERVICE_MAPWEBSERVICE_H_ */
