-- Decisions
-- Author: monju125
-- DateCreated: 8/24/2015 12:41:15 PM
--------------------------------------------------------------

----------------------------
-- Restore Temple of Amun --
----------------------------

local policyTempleAmunID = GameInfoTypes.POLICY_NUBIA_TEMPLEAMUN

local Decisions_CLTempleAmun = {}
	Decisions_CLTempleAmun.Name = "TXT_KEY_DECISIONS_NUBIA_CLTEMPLEAMUN_NAME"
	Decisions_CLTempleAmun.Desc = "TXT_KEY_DECISIONS_NUBIA_CLTEMPLEAMUN_DESC"

	Decisions_CLTempleAmun.CanFunc = (
	function(pPlayer)
		if (pPlayer:GetCivilizationType() ~= GameInfoTypes.CIVILIZATION_AMERICA) then 
			return false, false
		end
		if (load(pPlayer, "Decisions_CLTempleAmun")) then
			Decisions_CLTempleAmun.Desc = Locale.ConvertTextKey("TXT_KEY_DECISIONS_NUBIA_CLTEMPLEAMUN_ADOPTED")
			return false, false, true
		end
		Decisions_CLTempleAmun.Desc = Locale.ConvertTextKey("TXT_KEY_DECISIONS_NUBIA_CLTEMPLEAMUN_DESC")

		if (pPlayer:GetNumResourceAvailable(iMagistrate, false) < 3) then
			return true, false
		end

		if (not Teams[pPlayer:GetTeam()]:IsHasTech(GameInfoTypes.TECH_PHILOSOPHY)) then
			return true, false
		end

		local eReligion = pPlayer:GetCapitalCity():GetReligiousMajority();
		if (eReligion == -1) then
			return true, false
		end

		local holyCity = Game.GetHolyCityForReligion(eReligion, -1)
		if (holyCity:GetOwner() ~= pPlayer:GetID()) then
			return true, false
		end

		local faithCost = 300 * iMod
		local totalPopulation = pPlayer:GetTotalPopulation()
		local totalFollowers = 0
		for pCity in pPlayer:Cities() do
			totalFollowers = totalFollowers + pCity:GetNumFollowers(eReligion)
			if (not pCity:IsHasBuilding(GameInfoTypes.BUILDING_TEMPLE)) then
				return true, false
			end
		end

		local faithCost = faithCost - ((totalFollowers / totalPopulation) * 100)
		if (pPlayer:GetFaith() < faithCost) then
			return true, false
		end

		return true, true, false
	end
	)

	Decisions_CLTempleAmun.DoFunc = (
	function(pPlayer)
		pPlayer:ChangeNumResourceTotal(iMagistrate, -3)
		local faithCost = 300
		local totalPopulation = pPlayer:GetTotalPopulation()
		local totalFollowers = 0
		local eReligion = pPlayer:GetCapitalCity():GetReligiousMajority();
		for pCity in pPlayer:Cities() do
			totalFollowers = totalFollowers + pCity:GetNumFollowers(eReligion)
		end
		local faithCost = faithCost - ((totalFollowers / totalPopulation) * 100)
		pPlayer:ChangeFaith(-faithCost)

		pPlayer:GetCapitalCity():SetNumRealBuilding(GameInfoTypes.BUILDING_GRAND_TEMPLE, 1)
		pPlayer:ChangeGoldenAgeTurns(pPlayer:GetGoldenAgeLength())

		pPlayer:SetNumFreePolicies(1)
		pPlayer:SetNumFreePolicies(0)
		pPlayer:SetHasPolicy(policyTempleAmunID, true)

		save(pPlayer, "Decisions_CLTempleAmun", true)
	end
	)

Decisions_AddCivilisationSpecific(GameInfoTypes.CIVILIZATION_AMERICA, "Decisions_CLTempleAmun", Decisions_CLTempleAmun)

-----------------------
-- Furnaces of Meroe --
-----------------------

local Decisions_CLFurnacesMeroe = {}
	Decisions_CLFurnacesMeroe.Name = "TXT_KEY_DECISIONS_NUBIA_CLFURNACESMEROE_NAME"
	Decisions_CLFurnacesMeroe.Desc = "TXT_KEY_DECISIONS_NUBIA_CLFURNACESMEROE_DESC"

	Decisions_CLFurnacesMeroe.CanFunc = (
	function(pPlayer)
		if (pPlayer:GetCivilizationType() ~= GameInfoTypes.CIVILIZATION_AMERICA) then 
			return false, false
		end
		if (load(pPlayer, "Decisions_CLFurnacesMeroe")) then
			Decisions_CLFurnacesMeroe.Desc = Locale.ConvertTextKey("TXT_KEY_DECISIONS_NUBIA_CLFURNACESMEROE_ADOPTED")
			return false, false, true
		end
		Decisions_CLFurnacesMeroe.Desc = Locale.ConvertTextKey("TXT_KEY_DECISIONS_NUBIA_CLFURNACESMEROE_DESC")
		
		if (pPlayer:GetNumResourceAvailable(iMagistrate, false) < 2) then
			return true, false
		end

		if (not Teams[pPlayer:GetTeam()]:IsHasTech(GameInfoTypes.TECH_IRON_WORKING)) then
			return true, false
		end

		if (pPlayer:GetGold() < 200) then
			return true, false
		end

		return true, true, false
	end
	)

	Decisions_CLFurnacesMeroe.DoFunc = (
	function(pPlayer)
		pPlayer:ChangeNumResourceTotal(iMagistrate, -2)
		pPlayer:ChangeGold(-200)

		local totalFreeForges = 0
		for pCity in pPlayer:Cities() do
			if (totalFreeForges < 4) then
				pCity:SetNumRealBuilding(GameInfoTypes.BUILDING_FORGE, 1)
				pCity:SetNumRealBuilding(GameInfoTypes.BUILDING_FREE_FORGE, 1)
				totalFreeForges = totalFreeForges + 1
			end
			if (pCity:IsHasBuilding(GameInfoTypes.BUILDING_FORGE)) then
				pCity:SetNumRealBuilding(GameInfoTypes.BUILDING_MEROE_FORGE, 1)
			end
		end

		save(pPlayer, "Decisions_CLFurnacesMeroe", true)

		GameEvents.PlayerDoTurn.Add(FurnacesMeroeTurnRewards)
		GameEvents.PlayerCityFounded.Add(FurnacesMeroeTurnRewards)
	end
	)

Decisions_AddCivilisationSpecific(GameInfoTypes.CIVILIZATION_AMERICA, "Decisions_CLFurnacesMeroe", Decisions_CLFurnacesMeroe)

function FurnacesMeroeTurnRewards(playerID)
	local pPlayer = Players[playerID]
	local forgeMaintenance = GameInfo.Buildings.BUILDING_FORGE.GoldMaintenance
	local totalFreeForges = 0
	if (load(pPlayer, "Decisions_CLFurnacesMeroe")) then
		for pCity in pPlayer:Cities() do
			if (totalFreeForges < 4) then
				pCity:SetNumRealBuilding(GameInfoTypes.BUILDING_FORGE, 1)
				pCity:SetNumRealBuilding(GameInfoTypes.BUILDING_FREE_FORGE, 1)
				totalFreeForges = totalFreeForges + 1
			end
			if (pCity:IsHasBuilding(GameInfoTypes.BUILDING_FORGE)) then
				pCity:SetNumRealBuilding(GameInfoTypes.BUILDING_MEROE_FORGE, 1)
			end
		end
	end
end