﻿
-- Constantes
windowWeight = 200
windowHeight = 300

-- Variables globales
window = 0

backButton = 0

joueurButton = 0
commandesButton = 0
audioButton = 0
videoButton = 0
reseauButton = 0
centralisateurButton = 0
agarButton = 0
advancedConfigurationButton = 0
pluginsButton = 0

function onLoad()
	log("Version Lua : " .. _VERSION)

	-- ----------------------------------------
	-- Creation IHM
	-- ----------------------------------------

	-- Ouverture fenêtre
	window = Window();
	window:setTitle("Configuration");
	
	-- Combos
	joueurButton = window:addButton("Joueur");
	joueurButton:expandHoriz();
	
	commandesButton = window:addButton("Commandes");
	commandesButton:expandHoriz();
	
	audioButton = window:addButton("Audio");
	audioButton:expandHoriz();
	
	videoButton = window:addButton("Vidéo");
	videoButton:expandHoriz();
	
	reseauButton = window:addButton("Réseau");
	reseauButton:expandHoriz();
	
	centralisateurButton = window:addButton("Centralisateur");
	centralisateurButton:expandHoriz();
	
	agarButton = window:addButton("Configuration Agar");
	agarButton:expandHoriz();
	
	advancedConfigurationButton = window:addButton("Configuration avancée");
	advancedConfigurationButton:expandHoriz();

	pluginsButton = window:addButton("Plugins");
	pluginsButton:expandHoriz();
	
	-- Boutons
	window:addSeparator();

	backButton = window:addButton("Retour");
	backButton:expandHoriz();
	
	-- Affichage de la fenêtre
	window:setSize(windowWeight, windowHeight);
	
	local screenSizeWeight, screenSizeHeight = getScreenSize();
	window:setPosition((screenSizeWeight - windowWeight) / 2, (screenSizeHeight - windowHeight) / 2);
	
	window:hide();
end

function eventManager(event)
	local actionId = event:getActionId();
	log("Event actionId=" .. actionId);
	
	if actionId == 1104 then		-- Show configuration menu
		log("SHOW");
		window:show();
		
	elseif actionId == 1001 then	-- Hide menu
		log("HIDE");
		window:hide();

	elseif actionId == 1004 then		-- Widget event
		local source = event:getSource();

		if source == backButton then
			window:hide();
			pushEvent(1002);	-- Show main menu
			
		elseif source == joueurButton then
			window:hide();
			pushEvent(1107);
		
		elseif source == commandesButton then
			window:hide();
			pushEvent(1113);
			
		elseif source == audioButton then
			window:hide();
			pushEvent(10000);
			
		elseif source == videoButton then
			window:hide();
			pushEvent(1108);
			
		elseif source == reseauButton then
			window:hide();
			pushEvent(1109);
			
		elseif source == centralisateurButton then
			window:hide();
			pushEvent(1110);
			
		elseif source == agarButton then
			window:hide();
			pushEvent(1112);
		
		elseif source == advancedConfigurationButton then
			window:hide();
			pushEvent(1119);
		
		elseif source == pluginsButton then
			window:hide();
			pushEvent(1111);
			
		-- Evênement non-pris en compte
		else
			log("Evênement ignoré ("..event:__tostring()..")");
		end
	end
end
