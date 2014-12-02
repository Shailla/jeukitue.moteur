
mapDataTree = 0;

-- Position du sprite
posX = 0;
posY = 0;
posZ = 0;

-- Vitesse du sprite
vitX = 0;
vitY = 0;
vitZ = 0;

function onLoad()
	log("Version Lua : " .. _VERSION)
	
	mapDataTree = getDataTree();
	
	rootBrancheId = mapDataTree:createBranche({}, "root");
	
	mapBrancheId = mapDataTree:createBranche(rootBrancheId, "map");
	
	spriteBrancheId = mapDataTree:createBranche(mapBrancheId, "sprite1");

	vitXValeurId = mapDataTree:createValeur(spriteBrancheId, "float", "vitX");
	vitYValeurId = mapDataTree:createValeur(spriteBrancheId, "float", "vitY");
	vitZValeurId = mapDataTree:createValeur(spriteBrancheId, "float", "vitZ");

	posXValeurId = mapDataTree:createValeur(spriteBrancheId, "float", "posX");
	posYValeurId = mapDataTree:createValeur(spriteBrancheId, "float", "posY");
	posZValeurId = mapDataTree:createValeur(spriteBrancheId, "float", "posZ");	
	
	subscribeEvents("refresh");
end

function traceIds()
	traceId("root", rootBrancheId);
	traceId("mapBrancheId", mapBrancheId);
	traceId("spriteBrancheId", spriteBrancheId);
	traceId("vitXValeurId", vitXValeurId);
	traceId("vitYValeurId", vitYValeurId);
	traceId("vitZValeurId", vitZValeurId);
	traceId("posXValeurId", posXValeurId);
	traceId("posYValeurId", posYValeurId);
	traceId("posZValeurId", posZValeurId);
end

function traceId(object, id)
	msg = object .. " : ";
	for i = 1, #id do
		if i > 1 then
			msg = msg .. ",";
		end
		msg = msg .. id[i];
	end
	log(msg);
end

function eventManager(event)
	local type = event:getType();
		
	-- Action event
	if type == 1 then
		local actionId = event:getActionId();
		if actionId == 1100 then
			-- Vitesse du sprite
			if posX < -1 then
				vitX = 0.02;
			elseif posX > 1 then
				vitX = 0.02;
			end
			
			vitY = 0;
			vitZ = 0;
			
			-- Position du sprite
			posX = posX + vitX;
			posY = 0.5;
			posZ = 0.5;
		end
	end
end
