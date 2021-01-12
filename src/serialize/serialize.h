#pragma once

#include "ecs.h"

static void SerializeAll(Registry* registry)
{
  std::vector<Entity> entities = registry->GetAllEntityIds();

  std::cout << "Entity size : " << entities.size() << std::endl;
}