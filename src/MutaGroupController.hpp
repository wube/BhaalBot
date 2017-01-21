#pragma once
#include <GroupController.hpp>
#include <BWAPI.h>
#include <vector>
class Unit;

class MutaGroupManager : public GroupController
{
public:
  MutaGroupManager(Group& owner);
  bool isFull() const { return stackMutas.size() + joiningMutas.size() >= 11; }
  void add(Unit* muta);
  void onFrame();
  void stack(bool secondPhase);
  void stackFast();
  double getAverageVelocity();
  double getVelocity(Unit* unit);
  double getStackError();
  BWAPI::Position getCenter();
  void sendJoiningMutas();
  void transferJoiningMutasToStackMutas();
  void attackTarget(BWAPI::Unit unit);
  void chooseClosestTarget();
  void logic();
  void moveStackedMutasWithOverlord(BWAPI::Position position);
  void attackStackedMutasWithOverlord(BWAPI::Unit unit, BWAPI::Position position);
  double averageCooldownValue();
  int maxCooldownValue();
  int countOfMutasWith0Cooldown();
  BWAPI::Unit getOverlordFarAway(BWAPI::Position position);

  enum class OverallPhase
  {
    StackingPhase1,
    StackingPhase2,
    Attacking
  };
  static std::string overallPhaseToString(OverallPhase overallPhase);

  enum class AttackPhase
  {
    Nothing,
    MovingTowardsTarget,
    MovingAway
  };
  static std::string attackPhaseToString(AttackPhase attackPhase);

  std::vector<Unit*> joiningMutas;
  std::vector<Unit*> stackMutas;
  BWAPI::Unit unitTarget = nullptr;
  BWAPI::Position target;
  double angle = 0;
  OverallPhase overalLPhase = OverallPhase::StackingPhase1;
  AttackPhase attackPhase = AttackPhase::Nothing;
  uint32_t ticksSinceAttack = 0;
};