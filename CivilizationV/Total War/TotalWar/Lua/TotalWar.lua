-- TotalWar
-- Author: monju125
-- DateCreated: 10/17/2015 8:15:01 PM
--------------------------------------------------------------

function TotalWar()
	for i=0,GameDefines.MAX_MAJOR_CIVS-1,1 do
		local pPlayer = Players[i]
		if pPlayer:IsAlive() then
			for j=0,GameDefines.MAX_MAJOR_CIVS-1,1 do
				if i ~= j then
					local pNextPlayer = Players[j]
					if pNextPlayer:IsAlive() then
						local pTeam = Teams[pPlayer:GetTeam()]
						local pNextTeam = pNextPlayer:GetTeam()
						if pTeam:CanDeclareWar(pNextTeam) and not pTeam:IsAtWar(pNextTeam) then
							pTeam:DeclareWar(pNextTeam)
							pTeam:SetPermanentWarPeace(pNextTeam, true)
						end
					end
				end
			end
		end
	end
end
GameEvents.PlayerDoTurn.Add(TotalWar)
Events.AILeaderMessage.Add(TotalWar)