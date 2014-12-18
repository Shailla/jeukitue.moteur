
-- Constantes
windowWeight = 200
windowHeight = 300

-- Variables globales
window = 0

sceneButton = 0
multiButton = 0
configurationButton = 0
aboutButton = 0
debugButton = 0
quitMapButton = 0
quitButton = 0

function onLoad()
	log("Version Lua : " .. _VERSION)

	-- ----------------------------------------
	-- Creation IHM
	-- ----------------------------------------

	-- Ouverture fenêtre
	window = Window();
	window:setTitle("Menu principal");
	
	-- Combos	
	sceneButton = window:addButton("Ouvrir une scene");
	sceneButton:expandHoriz();
	
	multiButton = window:addButton("Multi-joueurs");
	multiButton:expandHoriz();
	
	configurationButton = window:addButton("Configuration");
	configurationButton:expandHoriz();
	
	aboutButton = window:addButton("A propos");
	aboutButton:expandHoriz();
	
	debugButton = window:addButton("Debug");
	debugButton:expandHoriz();
		
	-- Boutons
	window:addSeparator();

	backButton = window:addButton("Retour au jeu");
	backButton:expandHoriz();

	quitMapButton = window:addButton("Quitter la partie");
	quitMapButton:expandHoriz();
	
	quitButton = window:addButton("Quitter");
	quitButton:expandHoriz();
	
	-- Affichage de la fenêtre
	window:setSize(windowWeight, windowHeight);
	
	local screenSizeWeight, screenSizeHeight = getScreenSize();
	window:setPosition((screenSizeWeight - windowWeight) / 2, (screenSizeHeight - windowHeight) / 2);
	
	window:hide();
end

function eventManager(event)
	local type = event:getType();
		
	log("Event");
		
	-- Action event
	if type == 1 then
		local actionId = event:getActionId();
		
		log("Action event : "..actionId);
		
		if actionId == 1002 then		-- Show the window
			--log("SHOW");
			window:show();
		elseif actionId == 1001 then	-- Hide the window
			log("HIDE");
			window:hide();
		end
		
	-- Widget event
	elseif type == 3 then
		log("Widget event");	
		local source = event:getSource();
		
		if source == sceneButton then
			window:hide();
			pushEvent(1016);	-- Show main menu
			
		elseif source == multiButton then
			window:hide();
			pushEvent(1005);
		
		elseif source == configurationButton then
			window:hide();
			pushEvent(1004);
			
		elseif source == quitMapButton then
			window:hide();
			pushEvent(1017);
			
		elseif source == aboutButton then
			window:hide();
			pushEvent(10001);
			
		elseif source == debugButton then
			window:hide();
			pushEvent(1014);
			
		elseif source == quitButton then
			window:hide();
			pushEvent(1018);
			
		elseif source == backButton then
			window:hide();
			
		-- Evênement non-pris en compte
		else
			log("Evenement ignoré ("..event:__tostring()..")");
		end
	end
end
