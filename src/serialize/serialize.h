#pragma once

#include "ecs.h"
#include "startup.h"

#include <string>
#include <sstream>
#include "cereal/archives/json.hpp"
#include "cereal/types/string.hpp"
#include "glm.hpp"
#include "gtx/string_cast.hpp"

static void SerializeAllOut(Registry* registry)
{
  std::ofstream os("test.json");
  std::vector<Entity> entities = registry->GetAllEntityIds();

  {
    // Create a json output archive
    cereal::JSONOutputArchive archive(os);

    // For every entity
    for (Entity entity : entities)
    {
      // When compiled with -O2 (Release), if using
      // `oarchive.setNextName(std::to_string(entity).c_str());`
      // this will bug out for whatever reason, I think its an MSVC
      // bug, this fixes the problem
      std::string entityString = std::to_string(entity);
      archive.setNextName(entityString.c_str());
      archive.startNode();

      SERIALIZE_ALL_COMPONENTS_OUT(entity);

      archive.finishNode();
    }
  }
}

static void SerializeAllIn(Registry* registry)
{
  std::ifstream is("test.json");

  std::cout << "Test serialize in" << std::endl;

  {
    cereal::JSONInputArchive archive(is);

    bool cont = true;
    while (cont)
    {
      if (archive.getNodeName() == nullptr)
      {
        cont = false;
        std::cout << "broke" << std::endl;
        break;
      }

      // Get the entity id and create the entiy
      std::string nodeName = archive.getNodeName();
      Entity entity = std::stoul(nodeName, nullptr, 0);
      registry->CreateEntity(entity);

      std::cout << "Entity : " << entity << std::endl;

      archive.startNode();

      // std::cout << "got to after start node" << std::endl;
      // std::cout << std::string(archive.getNodeName()) << std::endl;

      // While the entity still has component that can be serialized in
      while (archive.getNodeName() != nullptr)
      {
        std::string currentComponent = archive.getNodeName();
        // std::cout << "CurrentComponent : " << currentComponent << std::endl;

        SERIALIZE_ALL_COMPONENTS_IN(entity);
      }

      archive.finishNode();
    }
  }
}