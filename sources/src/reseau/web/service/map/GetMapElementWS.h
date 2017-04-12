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
	static std::string ID;
	static std::string NAME;
	static std::string TYPE;
	static std::string MAPS;
	static std::string ELEMENTS;
	static std::string MAPELEMENT;
	static std::string MAPELEMENTS;
	static std::string HIGHLIGHTED;
	static std::string HIDDEN;

	static std::string CARAC;

	static std::string CARAC_UPDATABLE;
	static std::string CARAC_NAME;
	static std::string CARAC_VALUE;

	static std::string CARAC_TYPE;
	static std::string CARAC_TYPE_STRING;
	static std::string CARAC_TYPE_BOOL;

	static std::string CARAC_GROUP;
	static std::string CARAC_GROUP_MAIN;

	static std::regex RG_MAPS_SERVICE;
	static std::regex RG_MAP_SERVICE;
	static std::regex RG_MAP_ELEMENTS_SERVICE;
	static std::regex RG_MAP_ELEMENT_SERVICE;

	WebServiceResult getMapList();
	WebServiceResult getCurrentMap();
	WebServiceResult saveCurrentMap();
	void getElement(int elementId, JsonObject& mapElement, std::vector<std::string>& errors);
	WebServiceResult updateElements(HttpRequest& request);
	WebServiceResult updateElement(HttpRequest& request, int elementId);

	std::string updateElement(JsonObject* jsonObject, MapObject* object);
	JsonObject& addCharisticString(JsonList& jsonCharistics, const std::string& name, const std::string& group, bool updatable, const std::string& value);
	JsonObject& addCharisticBoolean(JsonList& jsonCharistics, const std::string& name, const std::string& group, bool updatable, const bool value);
	JsonObject& addCharisticNumber(JsonList& jsonCharistics, const std::string& name, const std::string& group, bool updatable, long value);

	void jisonifyMapGraphe(CMap* map, JsonObject& mapGraphe);

public:
	GetMapElementWS();
	virtual ~GetMapElementWS();

	WebServiceResult execute(HttpRequest& request, const std::string& baseEndpoint, const std::string& serviceEndpoint) throw(HttpException) override;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_SERVICE_GETMAPELEMENTWS_H_ */
