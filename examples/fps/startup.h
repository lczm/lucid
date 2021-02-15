#pragma once

#include "ecs.h"
#include "component.h"
#include "engineComponents.h"
#include "sound.h"
#include "font.h"
#include "rules.h"

/*
 *  User Guide
 *  For every new component that is defined and that will be used within
 *  the engine, please add them to the macros defined below.
 */

#define MOVE_ALL_COMPONENTS(search, from, to, index)       \
  MOVE_COMPONENT(uint32_t, search, from, to, index)        \
  MOVE_COMPONENT(Transform, search, from, to, index)       \
  MOVE_COMPONENT(RigidBody, search, from, to, index)       \
  MOVE_COMPONENT(Sound, search, from, to, index)           \
  MOVE_COMPONENT(Cube, search, from, to, index)            \
  MOVE_COMPONENT(Sphere, search, from, to, index)          \
  MOVE_COMPONENT(Model, search, from, to, index)           \
  MOVE_COMPONENT(ColliderCube, search, from, to, index)    \
  MOVE_COMPONENT(ColliderSphere, search, from, to, index)  \
  MOVE_COMPONENT(ColliderPolygon, search, from, to, index) \
  MOVE_COMPONENT(Sound, search, from, to, index)           \
  MOVE_COMPONENT(Font, search, from, to, index)            \
  MOVE_COMPONENT(Enemy, search, from, to, index)           \
  MOVE_COMPONENT(Camera, search, from, to, index)

#define REMOVE_ALL_COMPONENTS(keyPtr, index)       \
  REMOVE_COMPONENT(uint32_t, keyPtr, index)        \
  REMOVE_COMPONENT(Transform, keyPtr, index)       \
  REMOVE_COMPONENT(RigidBody, keyPtr, index)       \
  REMOVE_COMPONENT(Sound, keyPtr, index)           \
  REMOVE_COMPONENT(Cube, keyPtr, index)            \
  REMOVE_COMPONENT(Sphere, keyPtr, index)          \
  REMOVE_COMPONENT(Model, keyPtr, index)           \
  REMOVE_COMPONENT(ColliderCube, keyPtr, index)    \
  REMOVE_COMPONENT(ColliderSphere, keyPtr, index)  \
  REMOVE_COMPONENT(ColliderPolygon, keyPtr, index) \
  REMOVE_COMPONENT(Sound, keyPtr, index)           \
  REMOVE_COMPONENT(Font, keyPtr, index)            \
  REMOVE_COMPONENT(Enemy, keyPtr, index)           \
  REMOVE_COMPONENT(Camera, keyPtr, index)

#define REGISTER_COMPONENT_CREATE(archetype, keyPtr)          \
  CREATE_COMPONENT_VECTOR(uint32_t, archetype, keyPtr)        \
  CREATE_COMPONENT_VECTOR(Transform, archetype, keyPtr)       \
  CREATE_COMPONENT_VECTOR(RigidBody, archetype, keyPtr)       \
  CREATE_COMPONENT_VECTOR(Sound, archetype, keyPtr)           \
  CREATE_COMPONENT_VECTOR(Cube, archetype, keyPtr)            \
  CREATE_COMPONENT_VECTOR(Sphere, archetype, keyPtr)          \
  CREATE_COMPONENT_VECTOR(Model, archetype, keyPtr)           \
  CREATE_COMPONENT_VECTOR(ColliderCube, archetype, keyPtr)    \
  CREATE_COMPONENT_VECTOR(ColliderSphere, archetype, keyPtr)  \
  CREATE_COMPONENT_VECTOR(ColliderPolygon, archetype, keyPtr) \
  CREATE_COMPONENT_VECTOR(Sound, archetype, keyPtr)           \
  CREATE_COMPONENT_VECTOR(Font, archetype, keyPtr)            \
  CREATE_COMPONENT_VECTOR(Enemy, archetype, keyPtr)           \
  CREATE_COMPONENT_VECTOR(Camera, archetype, keyPtr)

#define SERIALIZE_ALL_COMPONENTS_OUT(entity)       \
  SERIALIZE_COMPONENT_OUT(Transform, entity)       \
  SERIALIZE_COMPONENT_OUT(RigidBody, entity)       \
  SERIALIZE_COMPONENT_OUT(Cube, entity)            \
  SERIALIZE_COMPONENT_OUT(Sphere, entity)          \
  SERIALIZE_COMPONENT_OUT(Model, entity)           \
  SERIALIZE_COMPONENT_OUT(ColliderCube, entity)    \
  SERIALIZE_COMPONENT_OUT(ColliderSphere, entity)  \
  SERIALIZE_COMPONENT_OUT(ColliderPolygon, entity) \
  SERIALIZE_COMPONENT_OUT(Sound, entity)           \
  SERIALIZE_COMPONENT_OUT(Font, entity)            \
  SERIALIZE_COMPONENT_OUT(Enemy, entity)           \
  SERIALIZE_COMPONENT_OUT(Camera, entity)

#define SERIALIZE_ALL_COMPONENTS_IN(entity)       \
  SERIALIZE_COMPONENT_IN(Transform, entity)       \
  SERIALIZE_COMPONENT_IN(RigidBody, entity)       \
  SERIALIZE_COMPONENT_IN(Cube, entity)            \
  SERIALIZE_COMPONENT_IN(Sphere, entity)          \
  SERIALIZE_COMPONENT_IN(Model, entity)           \
  SERIALIZE_COMPONENT_IN(ColliderCube, entity)    \
  SERIALIZE_COMPONENT_IN(ColliderSphere, entity)  \
  SERIALIZE_COMPONENT_IN(ColliderPolygon, entity) \
  SERIALIZE_COMPONENT_IN(Sound, entity)           \
  SERIALIZE_COMPONENT_IN(Font, entity)            \
  SERIALIZE_COMPONENT_IN(Enemy, entity)           \
  SERIALIZE_COMPONENT_IN(Camera, entity)

#define ADD_ALL_COMPONENTS(entity, name)       \
  ADD_COMPONENT(Transform, entity, name)       \
  ADD_COMPONENT(RigidBody, entity, name)       \
  ADD_COMPONENT(Cube, entity, name)            \
  ADD_COMPONENT(Sphere, entity, name)          \
  ADD_COMPONENT(Model, entity, name)           \
  ADD_COMPONENT(ColliderCube, entity, name)    \
  ADD_COMPONENT(ColliderSphere, entity, name)  \
  ADD_COMPONENT(ColliderPolygon, entity, name) \
  ADD_COMPONENT(Sound, entity, name)           \
  ADD_COMPONENT(Font, entity, name)            \
  ADD_COMPONENT(Enemy, entity, name)           \
  ADD_COMPONENT(Camera, entity, name)
