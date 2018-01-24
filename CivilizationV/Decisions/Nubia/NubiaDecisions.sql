CREATE TABLE IF NOT EXISTS DecisionsAddin_Support(FileName);
INSERT INTO DecisionsAddin_Support (FileName) VALUES ('NubiaDecisions.lua');

INSERT INTO Policies (Type, Description)
VALUES ('POLICY_NUBIA_TEMPLEAMUN', 'TXT_KEY_DECISIONS_NUBIA_TEMPLEAMUN_POLICY');

INSERT INTO Policy_BuildingClassCultureChanges (PolicyType, BuildingClassType, CultureChange)
VALUES ('POLICY_NUBIA_TEMPLEAMUN', 'BUILDINGCLASS_TEMPLE', 1);

INSERT INTO Policy_ImprovementYieldChanges (PolicyType, ImprovementType, YieldType, Yield)
VALUES ('POLICY_NUBIA_TEMPLEAMUN', 'IMPROVEMENT_MINE', 'YIELD_FAITH', 1);