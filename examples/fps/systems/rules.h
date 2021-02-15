#pragma once
#include <vector>
#include <cereal/types/string.hpp>

struct FpsRules
{
  uint32_t score = 0;
  uint32_t enemyCount = 0;
  uint32_t maxEnemyCount = 5;

  uint32_t fontId = 0;
  uint32_t soundId = 0;
  std::vector<uint32_t> ugandanIds;
};

struct Enemy
{
  uint32_t dead = 0;
  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(cereal::make_nvp("dead", dead)  //
    );
  }
};