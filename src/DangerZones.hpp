#pragma once
#include <Module.hpp>
#include <Direction.hpp>
#include <TargetWithPath.hpp>

class DangerZones : public Module
{
public:
  DangerZones(ModuleContainer& moduleContainer);
  void onStart() override;
  void onFrame() override;
  void addDanger(BWAPI::Position centerPosition, int32_t radius, int32_t value);
  void removeDanger(BWAPI::Position centerPosition, int32_t radius, int32_t value);
  TargetWithPath findBestTarget(BWAPI::Position position);
  void clearCostFromStart();

  class Node
  {
  public:
    int32_t danger = 0;
    double distanceFromStart = 0;
    Direction cameFrom;
  };
  std::vector<std::vector<Node>> data;
  int32_t width = 0, height = 0;
};
