-- Move wealth/research processes to earlier techs, downgrade both
UPDATE Processes SET TechPrereq="TECH_MINING" WHERE Type="PROCESS_WEALTH";
UPDATE Processes SET TechPrereq="TECH_WRITING" WHERE Type="PROCESS_RESEARCH";
UPDATE Process_ProductionYields SET Yield=10 WHERE ProcessType="PROCESS_WEALTH";
UPDATE Process_ProductionYields SET Yield=10 WHERE ProcessType="PROCESS_RESEARCH";

-- Change difficulty level specific settings
-- Production free units
UPDATE HandicapInfos SET ProductionFreeUnits=65-(5*ID);

-- Production free units per city
UPDATE HandicapInfos SET ProductionFreeUnitsPerCity=8 WHERE Type="HANDICAP_SETTLER";
UPDATE HandicapInfos SET ProductionFreeUnitsPerCity=7 WHERE Type="HANDICAP_CHIEFTAIN";
UPDATE HandicapInfos SET ProductionFreeUnitsPerCity=6 WHERE Type="HANDICAP_WARLORD";
UPDATE HandicapInfos SET ProductionFreeUnitsPerCity=5 WHERE Type in ("HANDICAP_PRINCE", "HANDICAP_AI_DEFAULT", "HANDICAP_KING", "HANDICAP_EMPEROR", "HANDICAP_IMMORTAL", "HANDICAP_DEITY");

-- Production free units per population percent
UPDATE HandicapInfos SET ProductionFreeUnitsPopulationPercent=50 WHERE Type="HANDICAP_SETTLER" OR Type="HANDICAP_CHIEFTAIN";
UPDATE HandicapInfos SET ProductionFreeUnitsPopulationPercent=25 WHERE Type in ("HANDICAP_WARLORD", "HANDICAP_PRINCE", "HANDICAP_AI_DEFAULT");
UPDATE HandicapInfos SET ProductionFreeUnitsPopulationPercent=10 WHERE Type in ("HANDICAP_KING", "HANDICAP_EMPEROR", "HANDICAP_IMMORTAL", "HANDICAP_DEITY");

-- AI unit supply percent
UPDATE HandicapInfos SET AIUnitSupplyPercent=0 WHERE Type="HANDICAP_SETTLER";
UPDATE HandicapInfos SET AIUnitSupplyPercent=10 WHERE Type="HANDICAP_CHIEFTAIN";
UPDATE HandicapInfos SET AIUnitSupplyPercent=20 WHERE Type="HANDICAP_WARLORD";
UPDATE HandicapInfos SET AIUnitSupplyPercent=50 WHERE Type="HANDICAP_PRINCE" OR Type="HANDICAP_AI_DEFAULT";
UPDATE HandicapInfos SET AIUnitSupplyPercent=75 WHERE Type="HANDICAP_KING";
UPDATE HandicapInfos SET AIUnitSupplyPercent=100 WHERE Type in ("HANDICAP_EMPEROR", "HANDICAP_IMMORTAL", "HANDICAP_DEITY");

-- AI unit cost percent
UPDATE HandicapInfos SET AIUnitCostPercent=80 WHERE Type="HANDICAP_SETTLER" OR Type="HANDICAP_CHIEFTAIN";
UPDATE HandicapInfos SET AIUnitCostPercent=75 WHERE Type in ("HANDICAP_WARLORD", "HANDICAP_PRINCE", "HANDICAP_AI_DEFAULT");
UPDATE HandicapInfos SET AIUnitCostPercent=70 WHERE Type="HANDICAP_KING";
UPDATE HandicapInfos SET AIUnitCostPercent=65 WHERE Type="HANDICAP_EMPEROR";
UPDATE HandicapInfos SET AIUnitCostPercent=60 WHERE Type="HANDICAP_IMMORTAL";
UPDATE HandicapInfos SET AIUnitCostPercent=55 WHERE Type="HANDICAP_DEITY";

-- Handicap free units
UPDATE HandicapInfos SET GoldFreeUnits=50 WHERE Type in ("HANDICAP_SETTLER", "HANDICAP_CHIEFTAIN", "HANDICAP_WARLORD");
UPDATE HandicapInfos SET GoldFreeUnits=40 WHERE Type in ("HANDICAP_PRINCE", "HANDICAP_AI_DEFAULT");
UPDATE HandicapInfos SET GoldFreeUnits=30 WHERE Type="HANDICAP_KING";
UPDATE HandicapInfos SET GoldFreeUnits=25 WHERE Type in ("HANDICAP_EMPEROR", "HANDICAP_IMMORTAL", "HANDICAP_DEITY");

-- Update unit maintenance
UPDATE Defines SET Value=75 WHERE Name="INITIAL_GOLD_PER_UNIT_TIMES_100";
UPDATE Defines SET Value=3 WHERE Name="UNIT_MAINTENANCE_GAME_MULTIPLIER";
UPDATE Defines SET Value=20 WHERE Name="UNIT_MAINTENANCE_GAME_EXPONENT_DIVISOR";

-- Update buildings costs
UPDATE Buildings SET Cost=Cost*1.25 WHERE PrereqTech in (SELECT Type FROM Technologies WHERE Era="ERA_MEDIEVAL" OR Era="ERA_RENAISSANCE");
UPDATE Buildings SET Cost=Cost*1.50 WHERE PrereqTech in (SELECT Type FROM Technologies WHERE Era="ERA_INDUSTRIAL" OR Era="ERA_MODERN");
UPDATE Buildings SET Cost=Cost*1.75 WHERE PrereqTech in (SELECT Type FROM Technologies WHERE Era="ERA_POSTMODERN" OR Era="ERA_FUTURE");

-- Update unit costs
UPDATE Units SET Cost=Cost*1.25 WHERE PrereqTech in (SELECT Type FROM Technologies WHERE Era="ERA_MEDIEVAL" OR Era="ERA_RENAISSANCE");
UPDATE Units SET Cost=Cost*1.50 WHERE PrereqTech in (SELECT Type FROM Technologies WHERE Era="ERA_INDUSTRIAL" OR Era="ERA_MODERN");
UPDATE Units SET Cost=Cost*1.75 WHERE PrereqTech in (SELECT Type FROM Technologies WHERE Era="ERA_POSTMODERN" OR Era="ERA_FUTURE");

-- Update wonders cost and early era buildings gold maintenance
UPDATE Buildings SET Cost=Cost*2 WHERE BuildingClass in (SELECT Type FROM BuildingClasses WHERE MaxGlobalInstances=1) AND Cost > 0;

-- Update technologies costs
UPDATE Technologies SET Cost=Cost*2.50 WHERE Era="ERA_ANCIENT" AND Type IN (SELECT TechType FROM Technology_PrereqTechs WHERE PrereqTech="TECH_AGRICULTURE");
UPDATE Technologies SET Cost=Cost*5.25 WHERE Era="ERA_ANCIENT" AND Type NOT IN (SELECT TechType FROM Technology_PrereqTechs WHERE PrereqTech="TECH_AGRICULTURE");
UPDATE Technologies SET Cost=Cost*6.25 WHERE Era="ERA_CLASSICAL";
UPDATE Technologies SET Cost=Cost*6.75 WHERE Era="ERA_MEDIEVAL";
UPDATE Technologies SET Cost=Cost*5.25 WHERE Era="ERA_RENAISSANCE";
UPDATE Technologies SET Cost=Cost*4.25 WHERE Era="ERA_INDUSTRIAL";
UPDATE Technologies SET Cost=Cost*3.75 WHERE Era="ERA_MODERN";
UPDATE Technologies SET Cost=Cost*3.75 WHERE Era="ERA_POSTMODERN";
UPDATE Technologies SET Cost=Cost*3.25 WHERE Era="ERA_FUTURE";

-- Update advanced start research/growth percents
UPDATE Eras SET ResearchPercent=100;
UPDATE Eras SET GrowthPercent=60 WHERE Type="ERA_CLASSICAL";
UPDATE Eras SET GrowthPercent=50 WHERE Type="ERA_MEDIEVAL";
UPDATE Eras SET GrowthPercent=40 WHERE Type="ERA_RENAISSANCE";
UPDATE Eras SET GrowthPercent=30 WHERE Type="ERA_INDUSTRIAL";
UPDATE Eras SET GrowthPercent=20 WHERE Type="ERA_MODERN";
UPDATE Eras SET GrowthPercent=10 WHERE Type="ERA_POSTMODERN";
UPDATE Eras SET GrowthPercent=5 WHERE Type="ERA_FUTURE";

-- Update science buildings
UPDATE Building_YieldChangesPerPop SET Yield=25 WHERE (BuildingType="BUILDING_LIBRARY" OR BuildingType="BUILDING_PUBLIC_SCHOOL" OR BuildingType="BUILDING_PAPER_MAKER" OR BuildingType="BUILDING_ROYAL_LIBRARY") AND YieldType="YIELD_SCIENCE";
UPDATE Building_YieldModifiers SET Yield=25 WHERE BuildingType in ("BUILDING_OBSERVATORY", "BUILDING_LABORATORY", "BUILDING_UNIVERSITY", "BUILDING_WAT") AND YieldType="YIELD_SCIENCE";
UPDATE Building_YieldChanges SET Yield=1 WHERE BuildingType="BUILDING_PUBLIC_SCHOOL" AND YieldType="YIELD_SCIENCE";
UPDATE Building_YieldChanges SET Yield=2 WHERE BuildingType="BUILDING_LABORATORY" AND YieldType="YIELD_SCIENCE";

-- Update various science related entries
UPDATE Improvement_Yields SET Yield=4 WHERE ImprovementType="IMPROVEMENT_ACADEMY";
UPDATE Improvement_TechYieldChanges SET Yield=1 WHERE ImprovementType="IMPROVEMENT_ACADEMY";
UPDATE Policy_SpecialistExtraYields SET Yield=1 WHERE PolicyType="POLICY_SECULARISM";
UPDATE Policy_BuildingClassYieldModifiers SET YieldMod=5 WHERE PolicyType="POLICY_FREE_THOUGHT";
UPDATE Policy_BuildingClassYieldModifiers SET YieldMod=10 WHERE PolicyType="POLICY_WORKERS_FACULTIES";
UPDATE Policies SET GreatScientistRateModifier=10 WHERE Type="POLICY_HUMANISM";
UPDATE Policies SET HappinessToScience=5 WHERE Type="POLICY_RATIONALISM";
UPDATE Defines SET Value=-10 WHERE Name="PUPPET_SCIENCE_MODIFIER";
UPDATE Trait_ImprovementYieldChanges SET Yield=1 WHERE YieldType="YIELD_SCIENCE";
UPDATE Trait_SpecialistYieldChanges SET Yield=1 WHERE YieldType="YIELD_SCIENCE";

-- Update world size science mod per city
UPDATE Worlds SET NumCitiesTechCostMod=NumCitiesTechCostMod-1.5;

-- Update leader biases to be more forgiving
UPDATE Leaders SET WarmongerHate=CASE WHEN WarmongerHate <= 3 THEN 1 ELSE WarmongerHate-3 END;
UPDATE Leaders SET Forgiveness=CASE WHEN Forgiveness > 9 THEN 12 ELSE Forgiveness+3 END;
UPDATE Leaders SET DenounceWillingness=CASE WHEN DenounceWillingness <= 3 THEN 1 ELSE DenounceWillingness-3 END;
UPDATE Leader_MajorCivApproachBiases SET Bias=CASE WHEN Bias <= 3 THEN 1 ELSE Bias-3 END WHERE MajorCivApproachType="MAJOR_CIV_APPROACH_HOSTILE";

-- Update offense/defense flavors for certain civs
UPDATE Leader_Flavors SET Flavor=8 WHERE LeaderType="LEADER_ASHURBANIPAL" AND FlavorType="FLAVOR_OFFENSE";
UPDATE Leader_Flavors SET Flavor=7 WHERE (LeaderType="LEADER_ISABELLA" OR LeaderType="LEADER_NAPOLEON") AND FlavorType="FLAVOR_OFFENSE";
UPDATE Leader_Flavors SET Flavor=6 WHERE LeaderType="LEADER_MARIA_I" AND FlavorType="FLAVOR_OFFENSE";
UPDATE Leader_Flavors SET Flavor=4 WHERE (LeaderType="LEADER_ELIZABETH" OR LeaderType="LEADER_WILLIAM") AND FlavorType="FLAVOR_OFFENSE";
UPDATE Leader_Flavors SET Flavor=8 WHERE LeaderType IN ("LEADER_DARIUS", "LEADER_ENRICO_DANDOLO", "LEADER_HIAWATHA") AND FlavorType="FLAVOR_DEFENSE";
UPDATE Leader_Flavors SET Flavor=7 WHERE LeaderType IN ("LEADER_ELIZABETH", "LEADER_GANDHI", "LEADER_HARUN_AL_RASHID", "LEADER_PEDRO", "LEADER_RAMESSES", "LEADER_WILLIAM") AND FlavorType="FLAVOR_DEFENSE";
UPDATE Leader_Flavors SET Flavor=6 WHERE LeaderType="LEADER_MARIA_I" AND FlavorType="FLAVOR_DEFENSE";
UPDATE Leader_Flavors SET Flavor=4 WHERE LeaderType="LEADER_NAPOLEON" AND FlavorType="FLAVOR_DEFENSE";

-- Update minor civ growth/production
UPDATE Defines SET Value=250 WHERE Name = "MINOR_CIV_PRODUCTION_PERCENT";
UPDATE Defines SET Value=200 WHERE Name = "MINOR_CIV_GROWTH_PERCENT";

-- Update AI military settings
UPDATE Defines SET Value=4.0 WHERE Name="AI_STRATEGY_DEFEND_MY_LANDS_UNITS_PER_CITY";
UPDATE Defines SET Value=0.5 WHERE Name="AI_STRATEGY_NAVAL_UNITS_PER_CITY";
UPDATE Defines SET Value=8 WHERE Name="AI_STRATEGY_DEFEND_MY_LANDS_BASE_UNITS";

-- Update AI city strategies to cut down on worker spam
UPDATE AICityStrategies SET WeightThreshold=50 WHERE Type="AICITYSTRATEGY_ENOUGH_TILE_IMPROVERS";
UPDATE AICityStrategies SET WeightThreshold=100 WHERE TYPE="AICITYSTRATEGY_NEED_TILE_IMPROVERS";
UPDATE AICityStrategies SET WeightThreshold=67 WHERE TYPE="AICITYSTRATEGY_WANT_TILE_IMPROVERS";

-- Update unhappiness settings to allow for wider conquered empires
UPDATE Defines SET Value=4 WHERE Name="UNHAPPINESS_PER_CAPTURED_CITY";
UPDATE Defines SET Value=1.25 WHERE Name="UNHAPPINESS_PER_OCCUPIED_POPULATION";

-- Update faith settings
UPDATE Defines SET Value=20 WHERE Name="RELIGION_MIN_FAITH_FIRST_PANTHEON";
UPDATE Defines SET Value=400 WHERE Name="RELIGION_MAIN_FAITH_FIRST_PROPHET";
UPDATE Defines SET Value=2000 WHERE Name="RELIGION_MIN_FAITH_FIRST_GREAT_PERSON";
UPDATE Defines SET Value=200 WHERE Name="RELIGION_FAITH_DELTA_NEXT_PROPHET";
UPDATE Defines SET Value=750 WHERE Name="RELIGION_FAITH_DELTA_NEXT_GREAT_PERSON";
UPDATE Units SET FaithCost=500 WHERE Type in ("UNIT_MISSIONARY", "UNIT_INQUISITOR");

-- Update offense/defense/military training flavors
UPDATE Leader_Flavors SET Flavor=Flavor*2 WHERE FlavorType in ("FLAVOR_OFFENSE", "FLAVOR_DEFENSE", "FLAVOR_MILITARY_TRAINING");

ALTER TABLE Leader_Flavors ADD COLUMN 'Updated' INTEGER DEFAULT 0;

-- Update offense flavor if a mod changes a base civ
CREATE TRIGGER OnUpdate_FlavorOffense AFTER UPDATE ON Leader_Flavors
WHEN OLD.FlavorType="FLAVOR_OFFENSE" AND OLD.LeaderType=NEW.LeaderType AND OLD.Updated=0
BEGIN
	UPDATE Leader_Flavors SET Updated=1 WHERE FlavorType="FLAVOR_OFFENSE" AND LeaderType=OLD.LeaderType;
    UPDATE Leader_Flavors SET Flavor=Flavor*2 WHERE FlavorType="FLAVOR_OFFENSE" AND LeaderType=OLD.LeaderType;
END;

-- Update defense flavor if a mod changes a base civ
CREATE TRIGGER OnUpdate_FlavorDefense AFTER UPDATE ON Leader_Flavors
WHEN OLD.FlavorType="FLAVOR_DEFENSE" AND OLD.LeaderType=NEW.LeaderType AND OLD.Updated=0
BEGIN
	UPDATE Leader_Flavors SET Updated=1 WHERE FlavorType="FLAVOR_DEFENSE" AND LeaderType=OLD.LeaderType;
    UPDATE Leader_Flavors SET Flavor=Flavor*2 WHERE FlavorType="FLAVOR_DEFENSE" AND LeaderType=OLD.LeaderType;
END;

-- Update military training if a mod changes base civ
CREATE TRIGGER OnUpdate_FlavorMilitaryTraining AFTER UPDATE ON Leader_Flavors
WHEN OLD.FlavorType="FLAVOR_MILITARY_TRAINING" AND OLD.LeaderType=NEW.LeaderType AND OLD.Updated=0
BEGIN
	UPDATE Leader_Flavors SET Updated=1 WHERE FlavorType="FLAVOR_MILITARY_TRAINING" AND LeaderType=OLD.LeaderType;
    UPDATE Leader_Flavors SET Flavor=Flavor*2 WHERE FlavorType="FLAVOR_MILITARY_TRAINING" AND LeaderType=OLD.LeaderType;
END;

-- Update offense flavor for new modded civs
CREATE TRIGGER OnInsert_FlavorOffense AFTER INSERT ON Leader_Flavors
WHEN NEW.FlavorType="FLAVOR_OFFENSE"
BEGIN
	UPDATE Leader_Flavors SET Flavor=Flavor*2 WHERE FlavorType="FLAVOR_OFFENSE" AND LeaderType=New.LeaderType;
END;

-- Update defense flavor for new modded civs
CREATE TRIGGER OnInsert_FlavorDefense AFTER INSERT ON Leader_Flavors
WHEN NEW.FlavorType="FLAVOR_DEFENSE"
BEGIN
	UPDATE Leader_Flavors SET Flavor=Flavor*2 WHERE FlavorType="FLAVOR_DEFENSE" AND LeaderType=NEW.LeaderType;
END;

-- Update military training flavor for new modded civs
CREATE TRIGGER OnInsert_FlavorMilitaryTraining AFTER INSERT ON Leader_Flavors
WHEN NEW.FlavorType="FLAVOR_MILITARY_TRAINING"
BEGIN
	UPDATE Leader_Flavors SET Flavor=Flavor*2 WHERE FlavorType="FLAVOR_MILITARY_TRAINING" AND LeaderType=NEW.LeaderType;
END;