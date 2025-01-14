#pragma once
#include <UnitTarget.hpp>
#include <GroupController.hpp>
#include <BWAPI.h>
#include <vector>
class Unit;
class UnitMemoryInfo;

// this describes what is the best found target of one zergling.
enum class LingTargetType
{
  DRONE_1_HIT,
  LING_1_HIT,
  OTHER_1_HIT,
  LING_2_HIT,
  DRONE_2_HIT,
  LING_IN_RANGE,
  DRONE_IN_RANGE,
  LING,
  DRONE,
  OTHER,
  NONE
};

struct BestTarget
{
  BWAPI::Unit target;
  LingTargetType type;
};

class LingGroupController : public GroupController
{
private:
  BestTarget chooseLingTarget(Unit* ling); //!< finds best available target of a ling
  //!> returns first found target in radius that match the criteria and has < maxAttackers already on him.
  BWAPI::Unit chooseTargetByDistance(Unit* ling, int radius, int maxAttackers, const BWAPI::UnitFilter &pred = nullptr);
  void flee(Unit* ling, int radiusToConsider, double howFar);

  double min = DBL_MAX, max = 0; // TODO delete

public:
  LingGroupController(Group& owner);
  virtual ~LingGroupController() {};

  void onAdded(Unit* ling) override;
  void onFrame() override;

  virtual void actionAttackMove() override;
  virtual void updateLingAttackMoveAction(Unit* ling); // either finish attack or select new target 
};
