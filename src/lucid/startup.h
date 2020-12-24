#pragma once

#include "ecs.h"
#include "component.h"
#include "engineComponents.h"

/*
 *  User Guide
 *  For every new component that is defined and that will be used within
 *  the engine, please add them to the two macros defined below.
 */

#define MOVE_ALL_COMPONENTS(from, to, index)      \
  MOVE_COMPONENT(uint32_t, from, to, index)       \
  MOVE_COMPONENT(Transform, from, to, index)      \
  MOVE_COMPONENT(RigidBody, from, to, index)      \
  MOVE_COMPONENT(Animation, from, to, index)      \
  MOVE_COMPONENT(SoundEffect, from, to, index)    \
  MOVE_COMPONENT(Music, from, to, index)          \
  MOVE_COMPONENT(Cube, from, to, index)           \
  MOVE_COMPONENT(Sphere, from, to, index)         \
  MOVE_COMPONENT(Model, from, to, index)          \
  MOVE_COMPONENT(ColliderCube, from, to, index)   \
  MOVE_COMPONENT(ColliderSphere, from, to, index) \
  MOVE_COMPONENT(ColliderPolygon, from, to, index)

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
