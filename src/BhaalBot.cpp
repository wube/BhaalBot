#include <BhaalBot.hpp>
#include <iostream>
#include <BWEM/bwem.h>
#include <PropertyTreeIni.hpp>
#include <Unit.hpp>
#include <Log.hpp>

BhaalBot* bhaalBot = nullptr;
namespace { auto & theMap = BWEM::Map::Instance(); }

Filesystem::Path myFolder = "bwapi-data/BhaalBot";
Filesystem::Path configPath = myFolder / "config.ini";
Filesystem::Path localConfigPath = myFolder / "config-local.ini";

BhaalBot::BhaalBot()
  : logger(myFolder)
  , overmind(this->moduleContainer)
  , bases(this->moduleContainer)
  , units(this->moduleContainer)
  , players(this->moduleContainer)
  , buildOrderManager(this->moduleContainer)
  , harvestingManager(this->moduleContainer)
  , morphingUnits(this->moduleContainer)
  , producingUnits(this->moduleContainer)
  , buildingPlaceabilityHelper(this->moduleContainer)
  , fightManager(this->moduleContainer)
  , deleyedExitChecker(this->moduleContainer)
  , costReservation(this->moduleContainer)
  , produceManager(this->moduleContainer)
  , larvaReservations(this->moduleContainer)
  , productionQueueReservations(this->moduleContainer)
  , scoutingManager(this->moduleContainer)
  , discoveredMemory(this->moduleContainer)
  , buildTasks(this->moduleContainer)
  , dangerZones(this->moduleContainer)
  , strategyExecutor(this->moduleContainer)
{
  if (Filesystem::exists(configPath))
    this->config = readIni(configPath);
  if (Filesystem::exists(localConfigPath))
  {
    PropertyTree localConfig = readIni(localConfigPath);
    this->config.merge(localConfig);
  }
  bhaalBot = this;
}

void BhaalBot::onStart()
{
  // Enable the UserInput flag, which allows us to control the bot and type messages.
  BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

  // Set the command optimization level so that common commands can be grouped
  // and reduce the bots APM (Actions Per Minute).
  BWAPI::Broodwar->setCommandOptimizationLevel(0);
  BWAPI::Broodwar->setLatCom(false);

  // Retrieve you and your enemy's races. enemy() will just return the first enemy.
  // If you wish to deal with multiple enemies then you must use enemies().
  if (BWAPI::Broodwar->enemy()) // First make sure there is an enemy
    BWAPI::Broodwar << "The matchup is " << BWAPI::Broodwar->self()->getRace() << " vs " << BWAPI::Broodwar->enemy()->getRace() << std::endl;
  try
  {
    if (!this->isUMSMap())
    {
      BWAPI::Broodwar << "Map initialization..." << std::endl;
      theMap.Initialize();
      theMap.EnableAutomaticPathAnalysis();
      bool startingLocationsOK = theMap.FindBasesForStartingLocations();
      assert(startingLocationsOK);

      //BWEM::utils::MapPrinter::Initialize(&theMap);
      this->bases.init();

      BWAPI::Broodwar->setLocalSpeed(10);
      //BWAPI::Broodwar->setFrameSkip(10);
    }
    this->units.initMap();
    this->moduleContainer.onStart();
  }
  catch (const std::exception & e)
  {
    BWAPI::Broodwar << "EXCEPTION: " << e.what() << std::endl;
  }
}

void BhaalBot::onEnd(bool isWinner)
{
  this->moduleContainer.onEnd(isWinner);
  if (isWinner)
    BWAPI::Broodwar << "Hooray";
}

void BhaalBot::onFrame()
{
  if (this->deleyedExitChecker.shouldExit())
  {
    BWAPI::Broodwar->leaveGame();
    return;
  }

  BWAPI::Broodwar->drawTextScreen(380, 0, "APM: %u", BWAPI::Broodwar->getAPM());
  BWAPI::Broodwar->drawTextScreen(380, 15, "Latency: %u", BWAPI::Broodwar->getLatencyFrames());

  /*
  // Prevent spamming by only running our onFrame once every number of latency frames.
  // Latency frames are the number of frames before commands are processed.
  if (BWAPI::Broodwar->getFrameCount() % BWAPI::Broodwar->getLatencyFrames() != 0)
    return;*/

  // Return if the game is a replay or is paused
  if (BWAPI::Broodwar->isReplay() || BWAPI::Broodwar->isPaused() || !BWAPI::Broodwar->self())
    return;

  this->moduleContainer.onFrame();
  this->units.printAssignments();
}

void BhaalBot::onSendText(std::string text)
{
  // Send the text to the game if it is not being processed.
  BWAPI::Broodwar->sendText("%s", text.c_str());
}

void BhaalBot::onReceiveText(BWAPI::Player player, std::string text)
{
  // Parse the received text
  BWAPI::Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void BhaalBot::onPlayerLeft(BWAPI::Player player)
{
  // Interact verbally with the other players in the game by
  // announcing that the other player has left.
  BWAPI::Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void BhaalBot::onNukeDetect(BWAPI::Position target)
{}

void BhaalBot::onUnitDiscover(BWAPI::Unit unit)
{
  Unit* ourUnit = this->units.onUnitComplete(unit);
  if (ourUnit->getPlayer() != bhaalBot->players.self)
    this->moduleContainer.onUnitComplete(ourUnit);
}

void BhaalBot::onUnitEvade(BWAPI::Unit unit)
{}

void BhaalBot::onUnitShow(BWAPI::Unit unit)
{}

void BhaalBot::onUnitHide(BWAPI::Unit unit)
{}

void BhaalBot::onUnitCreate(BWAPI::Unit unit)
{
  LOG_INFO("%s", unit->getType().getName().c_str());
  Unit* ourUnit = this->units.onUnitComplete(unit);
  this->moduleContainer.onUnitCreate(ourUnit);
}

void BhaalBot::onUnitDestroy(BWAPI::Unit unit)
{
  try
  {
    this->moduleContainer.onUnitDestroy(this->units.findOrThrow(unit));
    this->units.onUnitDestroy(unit);

    if (unit->getType().isMineralField())
      theMap.OnMineralDestroyed(unit);
    else if (unit->getType().isSpecialBuilding())
      theMap.OnStaticBuildingDestroyed(unit);
  }
  catch (const std::exception & e)
  {
    BWAPI::Broodwar << "EXCEPTION: " << e.what() << std::endl;
  }
}

void BhaalBot::onUnitMorph(BWAPI::Unit unit)
{
  Unit* ourUnit = this->units.find(unit);
  if (ourUnit)
  {
    BWAPI::UnitType lastSeenType = ourUnit->lastSeenUnitType;
    ourUnit->lastSeenUnitType = ourUnit->getType();
    this->moduleContainer.onUnitMorph(ourUnit, lastSeenType);
  }
}

void BhaalBot::onUnitRenegade(BWAPI::Unit unit)
{}

void BhaalBot::onUnitComplete(BWAPI::Unit unit)
{
  Unit* ourUnit = this->units.onUnitComplete(unit);
  this->moduleContainer.onUnitComplete(ourUnit);
}

bool BhaalBot::isUMSMap()
{
  return  BWAPI::Broodwar->getGameType() == BWAPI::GameTypes::Use_Map_Settings &&
          BWAPI::Broodwar->mapFileName() != "rush-test.scm";
}
