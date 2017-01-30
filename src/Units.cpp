#include <Units.hpp>
#include <Unit.hpp>
#include <Log.hpp>
#include <Player.hpp>

Units::~Units()
{
  for (auto& item: this->units)
    delete item.second;
}

Unit* Units::onUnitComplete(BWAPI::Unit unit)
{
  auto position = this->units.find(unit);
  if (position != this->units.end())
  {
    // I assume that our unit just morphed into different unit here
    if (position->second->player->bwapiPlayer == unit->getPlayer() ||
        position->second->lastSeenUnitType == BWAPI::UnitTypes::Resource_Vespene_Geyser)
      return position->second;
    delete this->units[unit];
    this->units.erase(unit);
  }
#ifdef DEBUG
  for (auto& item: this->units)
    if (item.second->getID() == unit->getID())
    {
      LOG_AND_ABORT("Adding unit %s %d that is already there as different instance.", unit->getType().getName().c_str(), unit->getID());
    }
#endif
  Unit* ourUnit = new Unit(unit);
  this->units[unit] = ourUnit;
  this->unitSet.insert(ourUnit);
  return ourUnit;
}

void Units::onUnitDestroy(BWAPI::Unit unit)
{
  auto position = this->units.find(unit);
  if (position == this->units.end())
    return;
  this->unitSet.erase(position->second);
  delete position->second;
  this->units.erase(position);
}

Unit* Units::find(BWAPI::Unit unit)
{
  auto position = this->units.find(unit);
  if (position == this->units.end())
    return nullptr;
  return position->second;
}

void Units::printAssignments()
{
  for (auto& item: this->units)
    item.second->printAssignment();
}

Unit* Units::findOrThrow(BWAPI::Unit unit)
{
  auto position = this->units.find(unit);
  if (position == this->units.end())
    LOG_AND_ABORT("Trying to find unknown " + unit->getType().getName());
  return position->second;
}
