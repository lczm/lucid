#pragma once

#include "ecs.h"
#include "component.h"
#include "engineComponents.h"

/*
 *  User Guide
 *  For every new component that is defined and that will be used within
 *  the engine, please add them to the two macros defined below.
 */

#define MOVE_ALL_COMPONENTS(search, from, to, index)      \
  MOVE_COMPONENT(uint32_t, search, from, to, index)       \
  MOVE_COMPONENT(Transform, search, from, to, index)      \
  MOVE_COMPONENT(RigidBody, search, from, to, index)      \
  MOVE_COMPONENT(Animation, search, from, to, index)      \
  MOVE_COMPONENT(SoundEffect, search, from, to, index)    \
  MOVE_COMPONENT(Music, search, from, to, index)          \
  MOVE_COMPONENT(Cube, search, from, to, index)           \
  MOVE_COMPONENT(Sphere, search, from, to, index)         \
  MOVE_COMPONENT(Model, search, from, to, index)          \
  MOVE_COMPONENT(ColliderCube, search, from, to, index)   \
  MOVE_COMPONENT(ColliderSphere, search, from, to, index) \
  MOVE_COMPONENT(ColliderPolygon, search, from, to, index)

#define REGISTER_COMPONENT_CREATE(archetype, keyPtr)         \
  CREATE_COMPONENT_VECTOR(uint32_t, archetype, keyPtr)       \
  CREATE_COMPONENT_VECTOR(Transform, archetype, keyPtr)      \
  CREATE_COMPONENT_VECTOR(RigidBody, archetype, keyPtr)      \
  CREATE_COMPONENT_VECTOR(Animation, archetype, keyPtr)      \
  CREATE_COMPONENT_VECTOR(SoundEffect, archetype, keyPtr)    \
  CREATE_COMPONENT_VECTOR(Music, archetype, keyPtr)          \
  CREATE_COMPONENT_VECTOR(Cube, archetype, keyPtr)           \
  CREATE_COMPONENT_VECTOR(Sphere, archetype, keyPtr)         \
  CREATE_COMPONENT_VECTOR(Model, archetype, keyPtr)          \
  CREATE_COMPONENT_VECTOR(ColliderCube, archetype, keyPtr)   \
  CREATE_COMPONENT_VECTOR(ColliderSphere, archetype, keyPtr) \
  CREATE_COMPONENT_VECTOR(ColliderPolygon, archetype, keyPtr)