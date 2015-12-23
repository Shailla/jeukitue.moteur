
mapDataTree = 0
nbrSprites = 50
vitessesX = {}
vitessesY = {}
vitessesZ = {}
nextVitessesX = {}
nextVitessesY = {}
nextVitessesZ = {}
positionsX = {}
positionsY = {}
positionsZ = {}
colorsR = {}
colorsG = {}
colorsB = {}
nextColorsR = {}
nextColorsG = {}
nextColorsB = {}

function onLoad()
	log("Version Lua A : " .. _VERSION)
	
	if (isModeServer() or isModeLocal()) then
		log("Jeu en mode serveur ou local => initialisation des sprites");
		
		mapDataTree = getDataTree();

		dirigeablesBrId = mapDataTree:getBranche( {"map", "sprites", "dirigeables"} );
		traceId("ID DIRIGEABLE", dirigeablesBrId);
		
		for index = 1, nbrSprites do
			createSprite(dirigeablesBrId, index);
		end
			
		subscribeEvents("refresh");
	else
		log("Jeu en mode client => pas initialisation des sprites (tâche laissée au serveur)");
	end
end

function createSprite(dirigeablesBrId, index)
	monSpriteBrId = mapDataTree:createBranche(dirigeablesBrId, "monSprite"..index);
	
	-- Position du sprite
	posX = mapDataTree:createValeur(monSpriteBrId, "float", "posX");
	posY = mapDataTree:createValeur(monSpriteBrId, "float", "posY");
	posZ = mapDataTree:createValeur(monSpriteBrId, "float", "posZ");

	-- Couleur du sprite
	colorR = mapDataTree:createValeur(monSpriteBrId, "float", "colorRed");
	colorG = mapDataTree:createValeur(monSpriteBrId, "float", "colorGreen");
	colorB = mapDataTree:createValeur(monSpriteBrId, "float", "colorBlue");	

	-- Vitesse du sprite
	vitX = mapDataTree:createValeur(monSpriteBrId, "float", "vitX");
	vitY = mapDataTree:createValeur(monSpriteBrId, "float", "vitY");
	vitZ = mapDataTree:createValeur(monSpriteBrId, "float", "vitZ");
	
	-- Prochaine vitesse du sprite (optimisation pour éviter des latences côté client)
	nextVitX = mapDataTree:createValeur(monSpriteBrId, "float", "nextVitX");
	nextVitY = mapDataTree:createValeur(monSpriteBrId, "float", "nextVitY");
	nextVitZ = mapDataTree:createValeur(monSpriteBrId, "float", "nextVitZ");
	
	-- Prochaine couleur du sprite (optimisation pour éviter des latences côté client)
	nextColorR = mapDataTree:createValeur(monSpriteBrId, "float", "nextColorRed");
	nextColorG = mapDataTree:createValeur(monSpriteBrId, "float", "nextColorGreen");
	nextColorB = mapDataTree:createValeur(monSpriteBrId, "float", "nextColorBlue");	
	
	vitX:setValue(0.02);
	vitY:setValue(0.005+index*0.05*math.random());
	vitZ:setValue(0.005+index*0.05*math.random());
	
	nextVitX:setValue(0.02);
	nextVitY:setValue(0.005+index*0.05*math.random());
	nextVitZ:setValue(0.005+index*0.05*math.random());

	posX:setValue(2*math.random());
	posY:setValue(2*math.random());
	posZ:setValue(0.5+2*math.random());	
	
	colorR:setValue(math.random());
	colorG:setValue(math.random());
	colorB:setValue(math.random());
	
	nextColorR:setValue(math.random());
	nextColorG:setValue(math.random());
	nextColorB:setValue(math.random());
	
	vitessesX[index] = vitX;
	vitessesY[index] = vitY;
	vitessesZ[index] = vitZ;
	
	nextVitessesX[index] = nextVitX;
	nextVitessesY[index] = nextVitY;
	nextVitessesZ[index] = nextVitZ;
	
	positionsX[index] = posX;
	positionsY[index] = posY;
	positionsZ[index] = posZ;
	
	-- Couleur de l'objet
	colorsR[index] = colorR;
	colorsG[index] = colorG;
	colorsB[index] = colorB;
	
	-- Prochaine couleur de l'objet (optimisation pour éviter des latences côté client)
	nextColorsR[index] = nextColorR;
	nextColorsG[index] = nextColorG;
	nextColorsB[index] = nextColorB;
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
			for index = 1, nbrSprites do
				updateVitPos(index);
			end
		end
	end
end

function updateVitPos(index)
	vitX = vitessesX[index];
	vitY = vitessesY[index];
	vitZ = vitessesZ[index];
	
	nextVitX = nextVitessesX[index];
	nextVitY = nextVitessesY[index];
	nextVitZ = nextVitessesZ[index];
	
	posX = positionsX[index];
	posY = positionsY[index];
	posZ = positionsZ[index];
	
	colorR = colorsR[index];
	colorG = colorsG[index];
	colorB = colorsB[index];
	
	nextColorR = nextColorsR[index];
	nextColorG = nextColorsG[index];
	nextColorB = nextColorsB[index];

	change = false
	
	-- Change la vitesse du sprite
	if (posX:getValue() < 0 and vitX:getValue() < 0) then
		vitX:setValue(nextVitX:getValue());
		change = true
	elseif (posX:getValue() > 2 and vitX:getValue() > 0) then
		vitX:setValue(-nextVitX:getValue());
		change = true
	end
	
	if (posY:getValue() < 0 and vitY:getValue() < 0) then
		vitY:setValue(nextVitY:getValue());
		change = true
	elseif (posY:getValue() > 2 and vitY:getValue() > 0) then
		vitY:setValue(-nextVitY:getValue());
		change = true
	end

	if (posZ:getValue() < 0.5 and vitZ:getValue() < 0) then
		vitZ:setValue(nextVitZ:getValue());
		change = true
	elseif (posZ:getValue() > 2.5 and vitZ:getValue() > 0) then
		vitZ:setValue(-nextVitZ:getValue());
		change = true
	end

	if (change) then
		-- Change la couleur du sprite
		colorR:setValue(nextColorR:getValue());
		colorG:setValue(nextColorG:getValue());
		colorB:setValue(nextColorB:getValue());

		if (isModeServer() or isModeLocal()) then		
			-- Calcule les prochaines vitesses et couleurs
			nextVitX:setValue(0.001+0.05*math.random());
			nextVitY:setValue(0.001+0.05*math.random());
			nextVitZ:setValue(0.001+0.05*math.random());
			nextColorR:setValue(math.random());
			nextColorG:setValue(math.random());
			nextColorB:setValue(math.random());
		end
	end	
	
	-- Position du sprite
	posX:setValue(posX:getValue() + vitX:getValue());
	posY:setValue(posY:getValue() + vitY:getValue());
	posZ:setValue(posZ:getValue() + vitZ:getValue());
end