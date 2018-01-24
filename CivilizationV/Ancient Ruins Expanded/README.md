Author: monju125

Version 5 Updates:

- Changed the Barbarian trap reward to spawn fewer units closer to cities
- Added the probability that a Barbarian trap reward will spawn units appropriate to the player's current era
- Fixed an issue that allowed the settler reward to be given when One City Challenge is enabled
- Changed the population/food rewards so that they won't be given if the civilization is already unhappy
- Fixed an issue that prevented ruins from being flagged as an archaeological site

Version 4 Updates:

- Changed the unit upgrade reward to work only if the upgrade unit type is from the Ancient/Classical era
- Fixed an issue that allowed Venice to receive the settler reward
- Fixed a bug that allowed the reveal ruins reward to be given even if all ruins have already been revealed
- Made a variety of changes to improve performance

Version 3 Updates:

- Fixed an issue that prevented previously revealed strategic resources from being shown after loading a saved game
- Fixed an issue that allowed rewards to be received multiple times in succession by saving/reloading a game

Version 2 Updates:

- Fixed a bug that occasionally caused Pathfinder units not to be shown a reward choice
- Fixed a bug that allowed the Production reward to be given when training units
- Changed the Reveal Resource reward so that it won't reveal luxury resources on plots owned by other civs

A complete rewrite of the Ancient Ruins system allowing for new and updated rewards.

Adds the following new rewards:

- Food growth in the capital city (alternative to population growth used in higher difficulty games)
- Production boost in the capital city (adds hammers to the current building construction; does not apply to training units)
- Research boost (alternative to free technology used in higher difficulty games)
- Faith boost (alternative to pantheon/prophet reward, useful for specifying the faith amount)
- Reveal other ancient ruins (every other ancient ruin on the map has the probability of being revealed specified in RevealProb)

Enables the following previously unused rewards:

- Trigger a trap that spawns barbarian brutes around the ruins (more will appear in higher difficulty games; not triggered by scout units)
- Low gold and high gold (used to differentiate amount given based on difficulty)
- Spawn a friendly scout or warrior
- Grant the unit experience (used in place of unit upgrade in higher difficulty games)
- Heal the unit if damaged
- Reveal the nearest random resource (luxury and strategic only)

As well, the Culture, Experience, Gold (all), Food, Science, Faith, and Production rewards are modified by the current difficulty and gamespeed. Lower difficulty settings and slower gamespeeds yield higher amounts. The HandicapModifier setting for each reward type in AncientRuinsRewards.xml is multiplied by 7-(0=Settler, 7=Deity). The GameSpeedModifier refers to the setting in GameSpeeds.xml used to scale the reward, multiplying it by the setting/100.

Determination of the reward given is different from the base game. Every ancient ruin has the same chance of giving any available reward for normal units. However, Ancient Ruins discovered by a unit with the Pathfinder promotion will have a random list of available rewards.