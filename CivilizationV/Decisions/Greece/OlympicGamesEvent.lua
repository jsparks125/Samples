-- OlympicGamesEvent
-- Author: monju125
-- DateCreated: 8/27/2015 10:47:49 AM
--------------------------------------------------------------

local GreekCivilizations = {"CIVILIZATION_GREECE", "CIVILIZATION_MC_ATHENS", "CIVILIZATION_MC_SPARTA", "CIVILIZATION_MC_PERGAMON"}
local iNumberOfHappinessTurns = 2
local iNumberOfUnhappinessTurns = 1

local Event_MC_OlympicGames = {}
	Event_MC_OlympicGames.Name = "TXT_KEY_EVENT_MC_OLYMPIC_GAMES"
	Event_MC_OlympicGames.Desc = "TXT_KEY_EVENT_MC_OLYMPIC_GAMES_DESC"
	Event_MC_OlympicGames.Weight = 5
	Event_MC_OlympicGames.CanFunc = (
		function(pPlayer)
			if (pPlayer:GetGold() < 50) then
				return false
			end

			local bCivFound = false
			for _,civ in pairs(GreekCivilizations) do
				if (pPlayer:GetCivilizationType() == GameInfoTypes[civ]) then
					bCivFound = true
					break
				end
			end
			if (not bCivFound) then
				return false
			end

			return true
		end
		)

	Event_MC_OlympicGames.Outcomes = {}

	Event_MC_OlympicGames.Outcomes[1] = {}
	Event_MC_OlympicGames.Outcomes[1].Name = "TXT_KEY_EVENT_MC_OLYMPIC_GAMES_OUTCOME_1"
	Event_MC_OlympicGames.Outcomes[1].Desc = Locale.ConvertTextKey("TXT_KEY_EVENT_MC_OLYMPIC_GAMES_OUTCOME_RESULT_1", iNumberOfUnhappinessTurns)
	Event_MC_OlympicGames.Outcomes[1].Weight = 1
	Event_MC_OlympicGames.Outcomes[1].CanFunc = (
		function(pPlayer)
			if (pPlayer:GetGold() < 100) then
				return false
			end
			return true
		end
		)

	Event_MC_OlympicGames.Outcomes[1].DoFunc = (
		function(pPlayer)
			pPlayer:ChangeGold(-100)
			local iRandResult = GetRandom(1, 100)
			if (iRandResult <= 75) then
				pPlayer:ChangeGoldenAgeTurns(pPlayer:GetGoldenAgeLength())
			else
				pPlayer:SetHappiness(pPlayer:GetHappiness() - 1)
				save(pPlayer, "Event_MC_OlympicGames_Unhappiness_Turns", iNumberOfUnhappinessTurns)
			end
		end
		)
		
	Event_MC_OlympicGames.Outcomes[2] = {}
	Event_MC_OlympicGames.Outcomes[2].Name = "TXT_KEY_EVENT_MC_OLYMPIC_GAMES_OUTCOME_2"
	Event_MC_OlympicGames.Outcomes[2].Desc = Locale.ConvertTextKey("TXT_KEY_EVENT_MC_OLYMPIC_GAMES_OUTCOME_RESULT_2", iNumberOfHappinessTurns)
	Event_MC_OlympicGames.Outcomes[2].Weight = 1
	Event_MC_OlympicGames.Outcomes[2].CanFunc = (
		function(pPlayer)
			if (pPlayer:GetGold() < 50) then
				return false
			end
			return true
		end
		)

	Event_MC_OlympicGames.Outcomes[2].DoFunc = (
		function(pPlayer)
			pPlayer:ChangeGold(-50)
			pPlayer:SetHappiness(pPlayer:GetHappiness() + 2)
			save(pPlayer, "Event_MC_OlympicGames_Happiness_Turns", iNumberOfHappinessTurns)
		end
		)
		
	Event_MC_OlympicGames.Outcomes[3] = {}
	Event_MC_OlympicGames.Outcomes[3].Name = "TXT_KEY_EVENT_MC_OLYMPIC_GAMES_OUTCOME_3"
	Event_MC_OlympicGames.Outcomes[3].Desc = "TXT_KEY_EVENT_MC_OLYMPIC_GAMES_OUTCOME_RESULT_3"
	Event_MC_OlympicGames.Outcomes[3].Weight = 1

	Event_MC_OlympicGames.Outcomes[3].DoFunc = (
		function(pPlayer)
		end
		)

Events_AddCivilisationSpecific(GameInfoTypes.CIVILIZATION_GREECE, "Event_MC_OlympicGames", Event_MC_OlympicGames)
Events_AddCivilisationSpecific(GameInfoTypes.CIVILIZATION_MC_ATHENS, "Event_MC_OlympicGames", Event_MC_OlympicGames)
Events_AddCivilisationSpecific(GameInfoTypes.CIVILIZATION_MC_SPARTA, "Event_MC_OlympicGames", Event_MC_OlympicGames)
Events_AddCivilisationSpecific(GameInfoTypes.CIVILIZATION_MC_PERGAMON, "Event_MC_OlympicGames", Event_MC_OlympicGames)

function CheckOlympicTurns(player)
	local pPlayer = Players[player]
	local iOlympicGamesTurns = load(pPlayer, "Event_MC_OlympicGames_Happiness_Turns")
	if (iOlympicGamesTurns ~= nil) then
		iOlympicGamesTurns = iOlympicGamesTurns - 1
		if (iOlympicGamesTurns > 0) then
			pPlayer:SetHappiness(pPlayer:GetHappiness() + 2)
			save(pPlayer, "Event_MC_OlympicGames_Happiness_Turns", iOlympicGamesTurns)
		else
			save(pPlayer, "Event_MC_OlympicGames_Happiness_Turns", nil)
		end
		return
	end
	iOlympicGamesTurns = load(pPlayer, "Event_MC_OlympicGames_Unhappiness_Turns")
	if (iOlympicGamesTurns ~= nil) then
		iOlympicGamesTurns = iOlympicGamesTurns - 1
		if (iOlympicGamesTurns > 0) then
			pPlayer:SetHappiness(pPlayer:GetHappiness() - 1)
			save(pPlayer, "Event_MC_OlympicGames_Unhappiness_Turns", iOlympicGamesTurns)
		else
			save(pPlayer, "Event_MC_OlympicGames_Unhappiness_Turns", nil)
		end
		return
	end
end
GameEvents.PlayerDoTurn.Add(CheckOlympicTurns)