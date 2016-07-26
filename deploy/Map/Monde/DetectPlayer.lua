
function onLoad()
	log("Version Lua A : " .. _VERSION)
	
	createPlayerZoneDetector("idCoucou", 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
end

function eventManager(event)
	local type = event:getType();
		
	-- Action event
	if type == 1 then
		local actionId = event:getActionId();
		if actionId == 1100 then
			for index = 1, nbrSprites do
				updateVitPos(index);
			end
		end
	end
end
