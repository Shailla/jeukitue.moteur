/*
 * MapWebService.cpp
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#include <stdlib.h>

#include "reseau/web/json/JsonObject.h"
#include "reseau/web/HtmlServer.h"
#include "service/MapService.h"
#include "service/dto/MapInformationDto.h"

#include "reseau/web/service/MapWebService.h"

namespace jkt {

MapWebService::MapWebService() {
}

MapWebService::~MapWebService() {
}

WebServiceResult MapWebService::execute(const std::string& endpoint, const std::string& method) {
	JsonObject root;
	JsonList& maps = root.addList("maps");

	vector<MapInformationDto> dtos;
	MapService::loadMapDirectoryContent(dtos);

	for(MapInformationDto dto : dtos) {
		JsonObject& jsonMap = maps.addObject();
		jsonMap.addString("name", dto.getMapFileMinimalName());
	}

	string json = root.toString();

	WebServiceResult result;
	result._contentSize = json.size();
	result._content = malloc(json.size());
	json.copy((char*)result._content, json.size());
	result._contentType = HtmlServer::HTTP_CONTENT_TYPE_JSON;
	result._status = HtmlServer::HTTP_RESPONSE_200;

	return result;
}

} /* namespace jkt */
