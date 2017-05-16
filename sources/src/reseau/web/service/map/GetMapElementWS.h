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
#include "reseau/web/json/BadFormatJsonException.h"

#include "reseau/web/service/WebService.h"

namespace jkt {

class CMap;
class MapObject;

class GetMapElementWS : public WebService {
public:
	class Caracteristic {
	public:
		const std::string _name;
		const std::string _type;
		const std::string _group;
		const bool _updatable;

		Caracteristic(const std::string& name, const std::string& type, const std::string& group, const bool updatable);
	};

	static const std::string ID;
	static const std::string NAME;
	static const std::string TYPE;
	static const std::string MAPS;
	static const std::string ELEMENTS;
	static const std::string MAPELEMENT;
	static const std::string MAPELEMENTS;

	static const std::string CARACS;
	static const std::string CARAC_VALUE;
	static const std::string CARAC_UPDATABLE;

	static const std::string CARAC_TYPE;
	static const std::string CARAC_TYPE_LONG;
	static const std::string CARAC_TYPE_BOOL;
	static const std::string CARAC_TYPE_STRING;

	static const std::string CARAC_GROUP;
	static const std::string CARAC_GROUP_MAIN;

	static const Caracteristic HIGHLIGHTED;
	static const Caracteristic HIDDEN;

private:
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

	void updateElement(JsonObject* jsonObject, MapObject* object) throw(BadFormatJsonException);

	JsonObject& addCharisticBoolean(JsonObject& jsonCharistics, const Caracteristic& carac, const bool value);
	JsonObject& addCharisticLong(JsonObject& jsonCharistics, const Caracteristic& carac, const bool value);
	JsonObject& addCharisticString(JsonObject& jsonCharistics, const Caracteristic& carac, const bool value);

	JsonObject& addCharisticBoolean(JsonObject& jsonCharistics, const std::string& name, const std::string& group, bool updatable, const bool value);
	JsonObject& addCharisticLong(JsonObject& jsonCharistics, const std::string& name, const std::string& group, bool updatable, long value);
	JsonObject& addCharisticString(JsonObject& jsonCharistics, const std::string& name, const std::string& group, bool updatable, const std::string& value);

	bool updateCharisticBoolean(JsonObject& jsonCharistics, const Caracteristic& carac, bool& newValue);
	bool updateCharisticLong(JsonObject& jsonCharistics, const Caracteristic& carac, long& newValue);
	bool updateCharisticString(JsonObject& jsonCharistics, const Caracteristic& carac, std::string& newValue);

	void jisonifyMapGraphe(CMap* map, JsonObject& mapGraphe);

public:
	GetMapElementWS();
	virtual ~GetMapElementWS();

	WebServiceResult execute(HttpRequest& request, const std::string& baseEndpoint, const std::string& serviceEndpoint) throw(HttpException) override;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_SERVICE_GETMAPELEMENTWS_H_ */
