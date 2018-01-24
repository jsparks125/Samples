-- NubiaCityInfo
-- Author: monju125 
-- Original by: Sukitract/JFD
-- DateCreated: 8/25/2015 7:26:18 AM
--------------------------------------------------------------
include("IconSupport")
-------------------------------------------------------------------------------------------------------------------------
-- Sukritact's Modular City Info Stack Support
-------------------------------------------------------------------------------------------------------------------------
function CityInfoStackDataRefresh(tCityInfoAddins, tEventsToHook)
	table.insert(tCityInfoAddins, {["Key"] = "NubiaMeroeCityInfo", ["SortOrder"] = 1})

	table.insert(tEventsToHook, Events.SerialEventCityHexHighlightDirty)
	table.insert(tEventsToHook, Events.SpecificCityInfoDirty)
end
LuaEvents.CityInfoStackDataRefresh.Add(CityInfoStackDataRefresh)
LuaEvents.RequestCityInfoStackDataRefresh()

function CityInfoStackDirty(key, instance)
	if key ~= "NubiaMeroeCityInfo" then return end
	
	Events.SerialEventEnterCityScreen.Remove(EventEnterCityScreen_Nubia)
	Events.SerialEventCityHexHighlightDirty.Remove(SerialEventCityDirty_Nubia)
	Events.SpecificCityInfoDirty.Remove(SerialEventCityDirty_Nubia)
	
	Controls.IconFrame:SetHide(true)
	ProcessCityScreen(instance)
end
LuaEvents.CityInfoStackDirty.Add(CityInfoStackDirty)

local buildingFreeForgeID = GameInfoTypes.BUILDING_FREE_FORGE
local buildingMeroeForgeID = GameInfoTypes.BUILDING_MEROE_FORGE

isCityScreenOpen = false
g_NubiaMeroeTipControls = {}
TTManager:GetTypeControlTable("NubiaMeroeTooltip", g_NubiaMeroeTipControls)

function ProcessCityScreen(instance)
	if not(instance) then
		instance = Controls
	end

	local activePlayer = Players[Game.GetActivePlayer()]
	local city = UI.GetHeadSelectedCity()
	if not(city) then
		instance.IconFrame:SetHide(true)
		return
	end
	
	local hasMeroeForge = city:IsHasBuilding(buildingMeroeForgeID)
	if not (hasMeroeForge) then
		instance.IconFrame:SetHide(true)
		return
	end
		
	local buildingMeroeForge = GameInfo.Buildings[buildingMeroeForgeID]
	instance.IconFrame:SetToolTipType("NubiaMeroeTooltip")
	IconHookup(buildingMeroeForge.PortraitIndex, 64, buildingMeroeForge.IconAtlas, instance.IconImage)
	
	local freeForge = city:GetNumBuilding(buildingFreeForgeID)
	local freeForgeText = ""
	if (freeForge > 0) then
		freeForgeText = " (FREE)"
	end
	
	local titleText = "[COLOR_POSITIVE_TEXT]" .. string.upper(Locale.ConvertTextKey("TXT_KEY_NUBIA_MEROE_FORGE_DESC")) .. freeForgeText .. "[ENDCOLOR]"
	local helpText = Locale.ConvertTextKey("TXT_KEY_NUBIA_MEROE_FORGE_HELP")
	g_NubiaMeroeTipControls.Heading:SetText(titleText)
	g_NubiaMeroeTipControls.Body:SetText(helpText)
	g_NubiaMeroeTipControls.Box:DoAutoSize()
	instance.IconFrame:SetHide(false)
end

function EventEnterCityScreen_MeroeForge()
	isCityScreenOpen = true
	ProcessCityScreen()
end
Events.SerialEventEnterCityScreen.Add(EventEnterCityScreen_MeroeForge)

function SerialEventCityDirty_MeroeForge()
	if isCityScreenOpen then
		ProcessCityScreen()
	end
end
Events.SerialEventCityHexHighlightDirty.Add(SerialEventCityDirty_MeroeForge)
Events.SpecificCityInfoDirty.Add(SerialEventCityDirty_MeroeForge)

function EventExitCityScreen_MeroeForge()
	isCityScreenOpen = false
	Controls.IconFrame:SetHide(true)
end
Events.SerialEventExitCityScreen.Add(EventExitCityScreen_MeroeForge)