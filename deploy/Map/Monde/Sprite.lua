
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
	
	brancheId = {11, 15};
	
	posX = mapDataTree:createValeur(brancheId, "float", "posX");
	posY = mapDataTree:createValeur(brancheId, "float", "posY");
	posZ = mapDataTree:createValeur(brancheId, "float", "posZ");
	
	vitX = mapDataTree:createValeur(brancheId, "float", "vitX");
	vitY = mapDataTree:createValeur(brancheId, "float", "vitY");
	vitZ = mapDataTree:createValeur(brancheId, "float", "vitZ");
	
	subscribeEvents("refresh");
end

function eventManager(event)
	local type = event:getType();
		
	-- Action event
	if type == 1 then
		local actionId = event:getActionId();
		if actionId == 1100 then
			log("Refresh");
		end
	end
end
