-- SpartaDecisions
-- Author: monju125
-- DateCreated: 8/26/2015 4:40:49 PM
--------------------------------------------------------------

local Decisions_MC_Sparta_CallToWar = {}
	Decisions_MC_Sparta_CallToWar.Name = "TXT_KEY_DECISIONS_MC_SPARTA_CALLTOWAR_NAME"
	Decisions_MC_Sparta_CallToWar.Desc = "TXT_KEY_DECISIONS_MC_SPARTA_CALLTOWAR_DESC"
	HookDecisionCivilizationIcon(Decisions_MC_Sparta_CallToWar, "CIVILIZATION_MC_SPARTA")

	Decisions_MC_Sparta_CallToWar.CanFunc = (
	function(pPlayer)
		if (pPlayer:GetCivilizationType() ~= GameInfoTypes.CIVILIZATION_MC_SPARTA) then
			return false, false
		end
		
		local sAtWarWith = ""
		for i = 1, GameDefines.MAX_MAJOR_CIVS-1, 1 do
			if (Teams[pPlayer:GetTeam()]:IsAtWar(Players[i]:GetTeam())) then
				sAtWarWith = Players[i]:GetCivilizationShortDescription()
				break
			end
		end
		if (sAtWarWith == "") then
			return false, false
		end

		local iEra = load(pPlayer, "Decisions_MC_Sparta_CallToWar")
		if (iEra == nil) then
			iEra = -1
		end
		if (iEra < pPlayer:GetCurrentEra()) then
			save(pPlayer, "Decisions_MC_Sparta_CallToWar", nil)
		else
			sAtWarWith = load(pPlayer, "Decisions_MC_Sparta_CallToWar_AtWarWith")
			Decisions_MC_Sparta_CallToWar.Desc = Locale.ConvertTextKey("TXT_KEY_DECISIONS_MC_SPARTA_CALLTOWAR_ADOPTED", sAtWarWith)
			return false, false, true
		end
		Decisions_MC_Sparta_CallToWar.Desc = Locale.ConvertTextKey("TXT_KEY_DECISIONS_MC_SPARTA_CALLTOWAR_DESC", sAtWarWith)

		if (pPlayer:GetNumResourceAvailable(iMagistrate, false) < 2) then
			return true, false
		end
		if (pPlayer:GetGold() < 200) then
			return true, false
		end

		local bCanReceiveUnit = false
		for i = GameDefines.MAX_MAJOR_CIVS, GameDefines.MAX_CIV_PLAYERS-1, 1 do
			local pMinorCiv = Players[i]
			if (pMinorCiv:GetCapitalCity() ~= nil) then
				local bFriendly = pMinorCiv:GetMinorCivFriendshipWithMajor(pPlayer:GetID()) >= 30
				local bBully = pMinorCiv:CanMajorBullyGold(pPlayer:GetID()) or pMinorCiv:CanMajorBullyUnit(pPlayer:GetID())
				if (bFriendly or bBully) then
					bCanReceiveUnit = true
					break
				end
			end
		end
		if (not bCanReceiveUnit) then
			return true, false
		end
		
		save(pPlayer, "Decisions_MC_Sparta_CallToWar_AtWarWith", sAtWarWith)
		return true, true
	end
	)

	Decisions_MC_Sparta_CallToWar.DoFunc = (
	function(pPlayer)
		pPlayer:ChangeNumResourceTotal(iMagistrate, -2)
		pPlayer:ChangeGold(-200)
		SpartaCallToWar(pPlayer)
		save(pPlayer, "Decisions_MC_Sparta_CallToWar", pPlayer:GetCurrentEra())
	end
	)

Decisions_AddCivilisationSpecific(GameInfoTypes.CIVILIZATION_MC_SPARTA, "Decisions_MC_Sparta_CallToWar", Decisions_MC_Sparta_CallToWar)

-- Lua port of DLL's CvMinorCivAI::DoSpawnUnit
function SpartaCallToWar(pPlayer)
	for i = GameDefines.MAX_MAJOR_CIVS, GameDefines.MAX_CIV_PLAYERS-1, 1 do
		local pMinorCiv = Players[i]
		if (Players[i]:IsAlive()) then
			local bFriendly = pMinorCiv:GetMinorCivFriendshipWithMajor(pPlayer:GetID()) >= 30
			local bBully = pMinorCiv:CanMajorBullyGold(pPlayer:GetID()) or pMinorCiv:CanMajorBullyUnit(pPlayer:GetID())
			if (bFriendly or bBully) then
				local pMinorCivCapitalCity = pMinorCiv:GetCapitalCity()
				if (pMinorCivCapitalCity ~= nil) then
					-- get player's city closest to minor civ's capital
					local pMinorCivCapitalPlot = pMinorCivCapitalCity:Plot()
					local pClosestCity = GetClosestFriendlyCity(pPlayer, pMinorCivCapitalPlot)
					local iX = pMinorCivCapitalPlot:GetX()
					local iY = pMinorCivCapitalPlot:GetY()
					if (pClosestCity ~= nil) then
						iX = pClosestCity:GetX()
						iY = pClosestCity:GetY()
					end

					-- find best unit to spawn
					-- GetStrongestMilitaryUnit only uses Combat, so pull both sets and compare RangedCombat as well
					local iBestUnitType = -1
					local iBestMeleeUnitType = GetStrongestMilitaryUnit(pPlayer, true, "UNITCOMBAT_MOUNTED", "UNITCOMBAT_MELEE", "UNITCOMBAT_GUN", "UNITCOMBAT_ARMOR", "UNITCOMBAT_HELICOPTER", "UNITCOMBAT_NAVALMELEE")
					local iBestRangedUnitType = GetStrongestMilitaryUnit(pPlayer, true, "UNITCOMBAT_ARCHER", "UNITCOMBAT_SIEGE", "UNITCOMBAT_NAVALRANGED")
					local uBestMeleeUnit = GameInfo.Units[iBestMeleeUnitType]
					local uBestRangedUnit = GameInfo.Units[iBestRangedUnitType]
					if (uBestRangedUnit.RangedCombat > uBestMeleeUnit.Combat or uBestRangedUnit.Combat > uBestMeleeUnit.Combat) then
						iBestUnitType = iBestRangedUnitType
					else
						iBestUnitType = iBestMeleeUnitType
					end

					-- check if we can use the minor civ's unique unit
					if (pMinorCiv:GetAlly() == pPlayer:GetID()) then
						local bUseUnique = false
						if (pMinorCiv:IsMinorCivHasUniqueUnit()) then
							local iMinorCivUniqueUnit = pMinorCiv:GetMinorCivUniqueUnit()
							local pTeam = Teams[pPlayer:GetTeam()]
							local sPrereqTech = GameInfo.Units[iMinorCivUniqueUnit].PrereqTech
							local sObsoleteTech = GameInfo.Units[iMinorCivUniqueUnit].ObsoleteTech
							local iPrereqTech = GameInfo.Technologies[sPrereqTech].ID
							local iObsoleteTech = GameInfo.Technologies[sObsoleteTech].ID
							if (pTeam:IsHasTech(iPrereqTech) and not pTeam:IsHasTech(iObsoleteTech)) then
								bUseUnique = true
							end
						end

						if (bUseUnique) then
							iBestUnitType = iMinorCivUniqueUnit
						end
					end

					-- couldn't use E&D's InitUnitFromCity function since the spawn city and owner are different
					if (iBestUnitType ~= -1) then
						local pUnit = pPlayer:InitUnit(iBestUnitType, iX, iY)
						pUnit:JumpToNearestValidPlot()
						pUnit:SetOriginalOwner(i)
						pUnit:SetExperience(pMinorCivCapitalCity:GetDomainFreeExperience(pUnit:GetDomainType()))
						for promotion in GameInfo.UnitPromotions() do
							iPromotion = promotion.ID
							if (pMinorCivCapitalCity:GetFreePromotionCount(iPromotion) > 0 and pUnit:IsPromotionValid(iPromotion)) then
								pUnit:SetHasPromotion(iPromotion, true)
							end
						end
					end
				end
			end
		end
	end
end

function GetClosestFriendlyCity(pPlayer, pPlot)
	local iBestDistance = 100000
	local pClosestCity
	for pCity in pPlayer:Cities() do
		local pCityPlot = pCity:Plot()
		local iDistance = Map.PlotDistance(pCityPlot:GetX(), pCityPlot:GetY(), pPlot:GetX(), pPlot:GetY())
		if (iDistance < iBestDistance) then
			iBestDistance = iDistance
			pClosestCity = pCity
		end
	end
	return pClosestCity
end