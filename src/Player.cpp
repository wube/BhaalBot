#include <Player.hpp>
#include <Unit.hpp>

Player::Player(BWAPI::Player player)
  : bwapiPlayer(player)
  , isEnemy(!player->isNeutral() && BWAPI::Broodwar->self()->isEnemy(player))
  , myself(BWAPI::Broodwar->self() == player)
{}

void Player::onUnitComplete(Unit* unit)
{
  this->unitCounts[unit->getType()]++;
  this->units.insert(unit);
}

void Player::onUnitDestroy(Unit* unit)
{
  this->unitCounts[unit->getType()]--;
  this->units.erase(unit);
}

void Player::onUnitMorph(Unit* unit, BWAPI::UnitType from)
{
  this->unitCounts[from]--;
  if (unit->getType() == BWAPI::UnitTypes::Zerg_Egg)
    this->unitCounts[unit->getType()]++;
}

void Player::drawDebug(BWAPI::Position position)
{
  int yPosition = position.y;
  BWAPI::Broodwar->drawText(BWAPI::CoordinateType::Screen, position.x, yPosition, "%s", this->bwapiPlayer->getName().c_str());
  yPosition += 15;
  for (auto& item: this->unitCounts)
    if (item.second != 0)
    {
      BWAPI::Broodwar->drawText(BWAPI::CoordinateType::Screen, position.x, yPosition, "%s", item.first.getName().c_str());
      BWAPI::Broodwar->drawText(BWAPI::CoordinateType::Screen, position.x + 80, yPosition, "%d", item.second);
      yPosition += 15;
    }
}