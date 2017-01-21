#pragma once
#include <Assignment.hpp>
#include <BWAPI.h>
#include <set>
class Group;
class GroupController;
class Unit;

class SimpleGroupAssignment : public Assignment
{
public:
  SimpleGroupAssignment(Group* group) : group(group) {}
  ~SimpleGroupAssignment();
  std::string str() const override { return "SimpleGroup"; }

  Group* group;
};

class Group
{
public:
  void add(Unit* unit);
  void remove(Unit* unit);
  const std::set<Unit*>& getUnits() const { return this->units; }
  void onFrame();
  Unit* getUnit(BWAPI::UnitType unitType);

private:
  std::set<Unit*> units;
  GroupController* controller = nullptr;
};