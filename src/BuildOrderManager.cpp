#include <BuildOrderManager.hpp>
#include <BhaalBot.hpp>
#include <BWEM/bwem.h>
#include <BuildOrder.hpp>
#include <Log.hpp>

BuildOrderManager::BuildOrderManager(ModuleContainer& moduleContainer)
  : Module(moduleContainer)
{}

BuildOrderManager::~BuildOrderManager()
{
  for (auto& item: this->buildOrders)
    delete item.second;
}

void BuildOrderManager::onFrame()
{
  this->executor.update();
}

void BuildOrderManager::onStart()
{
  {
    BuildOrder* pool4 = new BuildOrder("4 pool");
    pool4->add(new SendScoutBuildOrderItem(BWAPI::UnitTypes::Zerg_Overlord));
    pool4->add(BWAPI::UnitTypes::Zerg_Spawning_Pool);
    pool4->add(1, BWAPI::UnitTypes::Zerg_Drone);
    pool4->add(5, BWAPI::UnitTypes::Zerg_Zergling);
    pool4->add(new SwitchToAutomaticSupplyBuilding());
    pool4->add(6, BWAPI::UnitTypes::Zerg_Zergling);
    this->add(pool4);
  }

  {
    BuildOrder* pool5 = new BuildOrder("5 pool");
    pool5->add(new SendScoutBuildOrderItem(BWAPI::UnitTypes::Zerg_Overlord));
    pool5->add(1, BWAPI::UnitTypes::Zerg_Drone);
    pool5->add(BWAPI::UnitTypes::Zerg_Spawning_Pool);
    pool5->add(1, BWAPI::UnitTypes::Zerg_Drone);
    pool5->add(new SendScoutBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone));
    pool5->add(4, BWAPI::UnitTypes::Zerg_Zergling);
    pool5->add(new SwitchToAutomaticSupplyBuilding());
    this->add(pool5);
  }
  {
    BuildOrder* pool9 = new BuildOrder("9 pool");
    pool9->add(new SendScoutBuildOrderItem(BWAPI::UnitTypes::Zerg_Overlord));
    pool9->add(5, BWAPI::UnitTypes::Zerg_Drone);
    pool9->add(BWAPI::UnitTypes::Zerg_Spawning_Pool);
    pool9->add(BWAPI::UnitTypes::Zerg_Drone);
    pool9->add(new SendScoutBuildOrderItem(BWAPI::UnitTypes::Zerg_Drone));
    pool9->add(BWAPI::UnitTypes::Zerg_Overlord);
    pool9->add(BWAPI::UnitTypes::Zerg_Drone);
    pool9->add(6, BWAPI::UnitTypes::Zerg_Zergling);
    pool9->add(new SwitchToAutomaticSupplyBuilding());
    pool9->add(150, BWAPI::UnitTypes::Zerg_Zergling);
    this->add(pool9);
  }
  {
    BuildOrder*  pool11Exp = new BuildOrder("11 pool exp");
    pool11Exp->add(new SendScoutBuildOrderItem(BWAPI::UnitTypes::Zerg_Overlord));
    pool11Exp->add(5, BWAPI::UnitTypes::Zerg_Drone);
    pool11Exp->add(BWAPI::UnitTypes::Zerg_Overlord);
    pool11Exp->add(2, BWAPI::UnitTypes::Zerg_Drone);
    pool11Exp->add(BWAPI::UnitTypes::Zerg_Spawning_Pool);
    pool11Exp->add(3, BWAPI::UnitTypes::Zerg_Drone);
    pool11Exp->add(BWAPI::UnitTypes::Zerg_Hatchery, BuildLocationType::ClosestExpansion);
    pool11Exp->add(2, BWAPI::UnitTypes::Zerg_Zergling);
    pool11Exp->add(new SendScoutBuildOrderItem(BWAPI::UnitTypes::Zerg_Zergling));
    pool11Exp->add(2, BWAPI::UnitTypes::Zerg_Zergling);
    pool11Exp->add(new SendScoutBuildOrderItem(BWAPI::UnitTypes::Zerg_Zergling));
    pool11Exp->add(BWAPI::UnitTypes::Zerg_Hatchery);
    pool11Exp->add(6, BWAPI::UnitTypes::Zerg_Drone);
    pool11Exp->add(2, BWAPI::UnitTypes::Zerg_Extractor);
    pool11Exp->add(BWAPI::UnitTypes::Zerg_Overlord);
    pool11Exp->add(new SwitchToAutomaticSupplyBuilding());
    pool11Exp->add(BWAPI::UnitTypes::Zerg_Creep_Colony, BuildLocationType::NearFirstExpansion);
    pool11Exp->add(2, BWAPI::UnitTypes::Zerg_Drone);
    pool11Exp->add(BWAPI::UnitTypes::Zerg_Creep_Colony, BuildLocationType::NearFirstExpansion);
    pool11Exp->add(2, BWAPI::UnitTypes::Zerg_Drone);
    pool11Exp->add(BWAPI::UnitTypes::Zerg_Creep_Colony, BuildLocationType::NearFirstExpansion);
    pool11Exp->add(2, BWAPI::UnitTypes::Zerg_Drone);
    pool11Exp->add(BWAPI::UnitTypes::Zerg_Creep_Colony, BuildLocationType::NearFirstExpansion);
    pool11Exp->add(2, BWAPI::UnitTypes::Zerg_Drone);
    pool11Exp->add(BWAPI::UnitTypes::Zerg_Creep_Colony, BuildLocationType::NearFirstExpansion);
    pool11Exp->add(5, BWAPI::UnitTypes::Zerg_Sunken_Colony);
    pool11Exp->add(5, BWAPI::UnitTypes::Zerg_Drone);
    pool11Exp->add(BWAPI::UnitTypes::Zerg_Lair);
    pool11Exp->add(11, BWAPI::UnitTypes::Zerg_Drone);
    pool11Exp->add(BWAPI::UnitTypes::Zerg_Spire);
    pool11Exp->add(6, BWAPI::UnitTypes::Zerg_Zergling);
    for (uint32_t i = 0; i < 6; ++i)
      pool11Exp->add(new SendScoutBuildOrderItem(BWAPI::UnitTypes::Zerg_Zergling));
    pool11Exp->add(3, BWAPI::UnitTypes::Zerg_Overlord);
    for (uint32_t i = 0; i < 50; ++i)
    {
      pool11Exp->add(5, BWAPI::UnitTypes::Zerg_Mutalisk);
      pool11Exp->add(1, BWAPI::UnitTypes::Zerg_Zergling);
      for (uint32_t i = 0; i < 2; ++i)
        pool11Exp->add(new SendScoutBuildOrderItem(BWAPI::UnitTypes::Zerg_Zergling));
    }
    this->add(pool11Exp);
  }
}

void BuildOrderManager::add(BuildOrder* buildOrder)
{
  this->buildOrders[buildOrder->name] = buildOrder;
}

BuildOrder::~BuildOrder()
{
  for (BuildOrderItem* item: this->items)
    delete item;
}

void BuildOrder::add(uint32_t count, BWAPI::UnitType unit)
{
  for (uint32_t i = 0; i < count; ++i)
    this->add(unit);
}

void BuildOrder::add(BWAPI::UnitType unit, BuildLocationType location)
{
  this->items.push_back(new BuildBuildOrderItem(unit, location));
}

void BuildOrder::add(BuildOrderItem* item)
{
  this->items.push_back(item);
}
