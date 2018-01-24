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
	table.insert(tCityInfoAddins, {["Key"] = "NubiaAmunCityInfo", ["SortOrder"] = 1})

	table.insert(tEventsToHook, Events.SerialEventCityHexHighlightDirty)
	table.insert(tEventsToHook, Events.SpecificCityInfoDirty)
end
LuaEvents.CityInfoStackDataRefresh.Add(CityInfoStackDataRefresh)
LuaEvents.RequestCityInfoStackDataRefresh()

function CityInfoStackDirty(key, instance)
	if key ~= "NubiaAmunCityInfo" then return end
	
	Events.SerialEventEnterCityScreen.Remove(EventEnterCityScreen_Nubia)
	Events.SerialEventCityHexHighlightDirty.Remove(SerialEventCityDirty_Nubia)
	Events.SpecificCityInfoDirty.Remove(SerialEventCityDirty_Nubia)
	
	Controls.IconFrame:SetHide(true)
	ProcessCityScreen(instance)
end
LuaEvents.CityInfoStackDirty.Add(CityInfoStackDirty)

local amunPolicyID = GameInfoTypes.POLICY_NUBIA_TEMPLEAMUN

isCityScreenOpen = false
g_NubiaAmunTipControls = {}
TTManager:GetTypeControlTable("NubiaAmunTooltip", g_NubiaAmunTipControls)

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
	
	local hasTempleAmun = activePlayer:HasPolicy(amunPolicyID)
	if not (hasTempleAmun) then
		instance.IconFrame:SetHide(true)
		return
	end
		
	local buildingTempleAmun = GameInfo.Buildings[GameInfoTypes.BUILDING_TEMPLE]
	instance.IconFrame:SetToolTipType("NubiaAmunTooltip")
	IconHookup(buildingTempleAmun.PortraitIndex, 64, buildingTempleAmun.IconAtlas, instance.IconImage)
	
	local titleText = "[COLOR_POSITIVE_TEXT]" .. string.upper(Locale.ConvertTextKey("TXT_KEY_NUBIA_TEMPLE_AMUN_DESC")) .. "[ENDCOLOR]"
	local helpText = Locale.ConvertTextKey("TXT_KEY_NUBIA_TEMPLE_AMUN_HELP")
	g_NubiaAmunTipControls.Heading:SetText(titleText)
	g_NubiaAmunTipControls.Body:SetText(helpText)
	g_NubiaAmunTipControls.Box:DoAutoSize()
	instance.IconFrame:SetHide(false)
end

function EventEnterCityScreen_TempleAmun()
	isCityScreenOpen = true
	ProcessCityScreen()
end
Events.SerialEventEnterCityScreen.Add(EventEnterCityScreen_TempleAmun)

function SerialEventCityDirty_TempleAmun()
	if isCityScreenOpen then
		ProcessCityScreen()
	end
end
Events.SerialEventCityHexHighlightDirty.Add(SerialEventCityDirty_TempleAmun)
Events.SpecificCityInfoDirty.Add(SerialEventCityDirty_TempleAmun)

function EventExitCityScreen_TempleAmun()
	isCityScreenOpen = false
	Controls.IconFrame:SetHide(true)
end
Events.SerialEventExitCityScreen.Add(EventExitCityScreen_TempleAmun)
