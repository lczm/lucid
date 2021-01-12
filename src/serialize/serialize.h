#pragma once

#include "ecs.h"
#include "startup.h"

static void SerializeAll(Registry* registry)
{
  std::vector<Entity> entities = registry->GetAllEntityIds();

  // For every entity
  for (Entity entity : entities)
  {
  }

  std::cout << "Entity size : " << entities.size() << std::endl;
}