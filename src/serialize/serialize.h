#pragma once

#include "ecs.h"
#include "startup.h"

#include <string>
#include <sstream>
#include "cereal/archives/json.hpp"
#include "cereal/types/string.hpp"
#include "glm.hpp"
#include "gtx/string_cast.hpp"

static void SerializeAllOut(Registry* registry, std::string filename)
{
  std::ofstream os(filename);
  std::vector<Entity> entities = registry->GetAllEntityIds();

  {
    // Create a json output archive
    cereal::JSONOutputArchive archive(os);

    // For every entity
    for (Entity entity : entities)
    {
      // Skip the debug entities
      // TODO : EntityHasComponents not exactly working
      // if (registry->EntityHasComponents<Camera, GridLine>(entity))

      // Skip the gridline entities
      if (registry->EntityHasComponent<GridLine>(entity))
      {
        continue;
      }

      // If it is the scene camera, skip it
      // DevDebug& devDebug = registry->GetEditorResource<DevDebug>();

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

static void SerializeAllIn(Registry* registry, std::string filename)
{
  std::ifstream is(filename);

  {
    cereal::JSONInputArchive archive(is);

    bool cont = true;
    while (cont)
    {
      if (archive.getNodeName() == nullptr)
      {
        cont = false;
        // std::cout << "broke" << std::endl;
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

// This serializes all the data as above; SerializeAllIn
// But also takes into account the camera entities
static void SerializeAllIn(Registry* registry, std::string filename, std::vector<Entity> cameraIds)
{
  std::ifstream is(filename);

  // Should only add twice throughout, scene + game cameras
  uint32_t cameraCount = 0;

  {
    cereal::JSONInputArchive archive(is);

    bool cont = true;
    while (cont)
    {
      if (archive.getNodeName() == nullptr)
      {
        cont = false;
        // std::cout << "broke" << std::endl;
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

      if (cameraCount != 2)
      {
        if (registry->EntityHasComponent<Camera>(entity))
        {
          registry->AddComponentData<Camera>(cameraIds[cameraCount],
                                             registry->GetComponent<Camera>(entity));
          registry->AddComponentData<Transform>(cameraIds[cameraCount],
                                                registry->GetComponent<Transform>(entity));

          cameraCount++;

          registry->DeleteEntity<Deleter>(entity);
        }
      }
      archive.finishNode();
    }
  }
}