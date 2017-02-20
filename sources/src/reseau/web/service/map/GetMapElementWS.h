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
class MapObject;

class GetMapElementWS : public WebService {
	static string ID;
	static string NAME;
	static string TYPE;
	static string MAPS;
	static string ELEMENTS;
	static string MAPELEMENT;
	static string MAPELEMENTS;
	static string HIGHLIGHTED;
	static string HIDDEN;

	static std::regex RG_MAPS_SERVICE;
	static std::regex RG_MAP_SERVICE;
	static std::regex RG_MAP_ELEMENTS_SERVICE;
	static std::regex RG_MAP_ELEMENT_SERVICE;

	WebServiceResult getMapList();
	WebServiceResult getCurrentMap();
	WebServiceResult saveCurrentMap();
	void getElement(int elementId, JsonObject& mapElement, vector<string>& errors);
	WebServiceResult updateElements(HttpRequest& request);
	WebServiceResult updateElement(HttpRequest& request, int elementId);

	std::string updateElement(JsonObject* jsonObject, MapObject* object);

	void jisonifyMapGraphe(CMap* map, JsonObject& mapGraphe);

public:
	GetMapElementWS();
	virtual ~GetMapElementWS();

	WebServiceResult execute(HttpRequest& request, const std::string& baseEndpoint, const std::string& serviceEndpoint) throw(HttpException) override;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_SERVICE_GETMAPELEMENTWS_H_ */
