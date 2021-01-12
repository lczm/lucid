#pragma once

#include "ecs.h"
#include "startup.h"

#include <string>
#include <sstream>
#include <cereal/archives/json.hpp>
#include "glm.hpp"
#include "gtx/string_cast.hpp"

static void SerializeAll(Registry* registry)
{
  std::ofstream os("test.json");
  std::vector<Entity> entities = registry->GetAllEntityIds();

  {
    // Create a json output archive
    cereal::JSONOutputArchive oarchive(os);

    // For every entity
    for (Entity entity : entities)
    {
      // Try only the entities with transform components
      // oarchive(cereal::make_nvp("entity", entity));
      // oarchive(cereal::make_map_item("entity", entity));

      // When compiled with -O2 (Release), if using
      // `oarchive.setNextName(std::to_string(entity).c_str());`
      // this will bug out for whatever reason, I think its an MSVC 
      // bug, this fixes the problem
      std::string entityString = std::to_string(entity);
      oarchive.setNextName(entityString.c_str());
      oarchive.startNode();

      SERIALIZE_ALL_COMPONENTS(entity);

      oarchive.finishNode();
    }
  }
}