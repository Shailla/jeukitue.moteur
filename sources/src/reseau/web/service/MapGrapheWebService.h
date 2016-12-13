/*
 * MapGrapheWebService.h
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_SERVICE_MAPGRAPHEWEBSERVICE_H_
#define SRC_RESEAU_WEB_SERVICE_MAPGRAPHEWEBSERVICE_H_

#include "reseau/web/service/WebService.h"
#include "reseau/web/json/JsonObject.h"

namespace jkt {

class CMap;

class MapGrapheWebService : public WebService {
	static string ID;
	static string NAME;
	static string TYPE;
	static string SELECTED;

	static string MAP;
	static string GEO;
	static string LIGHT;
	static string ENTRYPOINT;

public:
	MapGrapheWebService();
	virtual ~MapGrapheWebService();

	void jisonifyMap(CMap* map, JsonObject& mapGraphe);
	WebServiceResult execute(const std::string& method, const std::string& serviceEndpoint, const std::string& params);
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_SERVICE_MAPGRAPHEWEBSERVICE_H_ */
