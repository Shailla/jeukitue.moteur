/*
 * MapWebService.h
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_SERVICE_GETMAPELEMENTWS_H_
#define SRC_RESEAU_WEB_SERVICE_GETMAPELEMENTWS_H_

#include <regex>

#include "reseau/web/json/JsonObject.h"

#include "reseau/web/service/WebService.h"

namespace jkt {

class CMap;

class GetMapElementWS : public WebService {
	static string ID;
	static string NAME;
	static string TYPE;
	static string SELECTED;

	static std::regex RG_GET_MAPS;
	static std::regex RG_GET_MAP_GRAPHE;
	static std::regex RG_GET_ELEMENT_SERVICE;
	static std::regex RG_UPDATE_ELEMENT_SERVICE;

	WebServiceResult getMapList();
	WebServiceResult getCurrentMapGraphe();
	WebServiceResult getElement(int elementId);
	WebServiceResult updateElement(HttpRequest& request, int elementId);

	void jisonifyMapGraphe(CMap* map, JsonObject& mapGraphe);

public:
	GetMapElementWS();
	virtual ~GetMapElementWS();

	WebServiceResult execute(HttpRequest& request, const std::string& baseEndpoint, const std::string& serviceEndpoint) throw(HttpException) override;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_SERVICE_GETMAPELEMENTWS_H_ */
