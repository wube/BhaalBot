#pragma once
#include <CostReservation.hpp>
#include <BuildLocationType.hpp>
#include <Assignment.hpp>
#include <UnitTarget.hpp>

class BuildTaskInProgress : public CostReservationItem
{
public:
  BuildTaskInProgress(BuildLocationType locationType,
                      BWAPI::UnitType unitType);
  ~BuildTaskInProgress();
  std::string str() const override;
  bool onFrame(); /** @return @c true if the task was finished */
  BWAPI::TilePosition getBuildPosition(Unit** buildingTarget);
  BWAPI::UnitType getUnitType() const { return this->unitType; }

private:
  friend class BuildTaskAssignment;
  void unassign(Unit* unit);

  Unit* worker = nullptr;
  UnitTarget targetBuilding;
  BuildLocationType locationType;
  BWAPI::TilePosition position = BWAPI::TilePositions::None;
  BWAPI::UnitType unitType;
  Unit* target = nullptr; // used when building over geyser
  uint32_t frameOfLastOrder;
};

class BuildTaskAssignment : public Assignment
{
public:
  BuildTaskAssignment(BuildTaskInProgress* buildTask) : buildTask(buildTask) {}
  ~BuildTaskAssignment();
  std::string str() const override;

private:
  BuildTaskInProgress* buildTask;
};
