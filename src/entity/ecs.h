#pragma once
#include <map>
#include <vector>
#include <utility>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "component.h"
#include "input.h"

/*
   This header file will define all that is needed for the ecs structure to
   perform. Mostly consists of typedefs as well as constants that can be changed
   to suit different needs.
*/

/*
   TODOs:

    - Systems priority queue
        Allow systems to register with a number so that we can control systems
        run order
*/

// Entities are inherently just ids
typedef std::uint32_t Entity;

// Archetypes are inherently just a std::vector<unsigned int>
typedef std::vector<unsigned int> Archetype;

// Define a maximum number of entities that the game can have at any single
// instance This could be set to be dynamic at some point.
const Entity MAX_ENTITIES = 1000000;

enum class Move {
  ADD,
  REMOVE,
};

struct ComponentMovementStatus {
  Move move;
  bool reconstructArchetype;
  uint32_t componentHash;
  uint32_t previousIndex;
};

/*
   The role of a ComponentVector is so that the user is able to iterate over
   multiple vectors.

   In the case where the user queries for components that exists in multiple
   archetypes, A ComponentVector will allow the user to be able to iterate over
   the multiple vectors in each of the archetypes.

   e.g. User queries <Physics>
        Archetype 1 : <Physics, NotPhysics>
        Archetype 2 : <Physics, NotPhysics, Movement>

    ComponentVector will be a ComponentVector<Physics>
        where the data is Physics from Archetype1 and Archetype2
*/
template <typename Component>
class ComponentVector {
 private:
  std::vector<std::vector<Component>*> store;
  std::vector<unsigned int> storeSizeIndex;

 public:
  ComponentVector(){};
  void AddVector(std::vector<Component>*& vectorPtr) {
    store.push_back(vectorPtr);
    storeSizeIndex.push_back(vectorPtr->size());
  };

  // Usage: somethingComponentVector->at(i).{structFields};
  Component* At(unsigned int index) {
    unsigned int primaryIndex = 0;
    for (size_t i = 0; i < storeSizeIndex.size(); i++) {
      if (index < storeSizeIndex[i]) {
        primaryIndex = i;
        break;
      } else {
        index -= storeSizeIndex[i];
      }
    }
    return &store.at(primaryIndex)->at(index);
  };

  // Usage : somethingComponentVector.size();
  // This is mostly used to know the total size of the ComponentVector so that
  // it is iterable with a for loop
  // Generally : for (int i = 0; i < somethingComponentVector.size(); i++) {.
  unsigned int Size() {
    int size = 0;
    for (int i : storeSizeIndex) {
      size += i;
    }
    return size;
  };
};

// Forward declare registry so that systems can take in a registry, Registry
// itself will need to take in a systems, so one way or the other there needs to
// be a forward declaration here
class Registry;
/*
    Abstract class for other systems to inherit out of
    We just need the virtual dispatch on update.
*/
class System {
 public:
  virtual void Update(double dt, Registry* registry, Input* input) = 0;
};

/*
   The registry is the center of all the pieces in this systems.
   It will be used to get entity ids, store component data, register systems...
   etc.

   Idea being that any time anything related to ecs is needed, just go to the
   registry and call something, all that is needed should already exist here.
*/
class Registry {
 public:
  // Used to give out entity ids
  std::vector<Entity> availablePool;
  std::vector<Entity> unavailablePool;

  // EntityId to vector of components the id has
  std::unordered_map<unsigned int, Archetype> entityComponentMap;

  // EntityId to the index of the archetype it is currently holding
  std::unordered_map<unsigned int, unsigned int> entityIndexMap;

  // This is to keep track of the entities that have been moved and needs to be flushed
  std::unordered_map<uint32_t, std::vector<ComponentMovementStatus>> flushCache;

  // ComponentType/ArcheType -> unordered_map<unsigned int,
  // vector<Component>> Archetypes are the key, as a vector of unsigned
  // ints. Values is an unordered_map with keys being the hashcodes of the
  // components, to a vector of said components.
  std::map<Archetype, void*> archetypeComponentMap;

  // Store all the systems that are in use
  std::vector<System*> systems;

 public:
  Registry() {
    // Reserve the memory in advance as this value is fixed
    // We can reserve for both available and unavailable in this case
    // because there is always the case where we use up all the available
    // ids and they get pushed to unavailable, we might as well have the
    // memory allocated in that situation
    availablePool.reserve(MAX_ENTITIES);
    unavailablePool.reserve(MAX_ENTITIES);

    // Just start from the top to make more sense, where the first few ids
    // take the first few numbers
    for (size_t i = MAX_ENTITIES; i != 0; i--) {
      availablePool.push_back(i);
    }
  };
  ~Registry();

  Entity GetAvailableEntityId() {
    if (availablePool.size() == 0) {
      std::cout << "Not enough ids" << std::endl;
      return 0;
    }

    Entity id = availablePool.back();

    // Remove the last element in the vector
    availablePool.erase(availablePool.begin() + availablePool.size() - 1);
    unavailablePool.push_back(id);

    return id;
  };
  void DeleteEntity(Entity entity);

  template <typename Component>
  bool EntityHasComponent(Entity id) {
    unsigned int paramHashCode = GetHashCode<Component>();

    // Get entity archetype
    if (entityComponentMap.find(id) == entityComponentMap.end()) {
      std::cout << "Entity id : " << id << "  has not been created" << std::endl;
      return false;
    }
    Archetype archetype = entityComponentMap[id];

    for (auto& hashCode : archetype) {
      if (paramHashCode == hashCode) {
        return true;
      }
    }
    return false;
  }

  template <typename... Components>
  bool EntityHasComponents(Entity id) {
    Archetype paramArchetype = {(GetHashCode<Components>())...};

    // Get entity archetype
    if (entityComponentMap.find(id) == entityComponentMap.end()) {
      std::cout << "Entity id : " << id << "  has not been created" << std::endl;
      return false;
    }
    Archetype archetype = entityComponentMap[id];

    unsigned int matchCount = 0;
    for (auto& paramHashCode : paramArchetype) {
      for (auto& hashCode : archetype) {
        if (paramHashCode == hashCode) {
          matchCount++;
        }
      }
    }

    if (matchCount == paramArchetype.size()) {
      return true;
    }
    return false;
  }

  template <typename Component>
  void CreateDefaultComponentValue(Entity id, Archetype archetype) {
    // Get the archetype that is assigned to the id
    // Archetype archetype = entityComponentMap[id];

    // Specific hashcode for this component
    unsigned int hashCode = GetHashCode<Component>();

    std::unordered_map<unsigned int, void*>& keyPtr =
        *(static_cast<std::unordered_map<unsigned int, void*>*>(archetypeComponentMap[archetype]));

    std::vector<Component>& vectorPtr = *(static_cast<std::vector<Component>*>(keyPtr[hashCode]));

    // Note that even though this is for each vector component, this should
    // not matter as every component goes through this, resulting in the
    // size (i.e. index) being all the same.
    entityIndexMap[id] = vectorPtr.size();

    vectorPtr.push_back(Component());
  }

  template <typename... Components>
  void CreateEntity(Entity id) {
    Archetype archetype = {(GetHashCode<Components>())...};

    // Check if the archetype actually exists before first
    // If the archetype is not found
    if (archetypeComponentMap.find(archetype) == archetypeComponentMap.end()) {
      std::cout << "Archetype for entity : " << id << " is not found, please create it"
                << std::endl;
      return;
    }

    // Assign the archetype to the entity
    entityComponentMap[id] = archetype;

    // Create empty default values for the entity
    auto p = {(CreateDefaultComponentValue<Components>(id, archetype), 0)...};
    (void)p;  // To silence the compiler warning about unused vars
  }

  // General method to convert (Component) to an unsigned int
  template <typename Component>
  unsigned int GetHashCode(Component component) {
    return typeid(component).hash_code();
  }

  // General method to convert <Component> to an unsigned int
  template <typename Component>
  unsigned int GetHashCode() {
    return typeid(Component).hash_code();
  }

  template <typename Component>
  void InitializeArchetypeVector(Archetype archetype) {
    std::unordered_map<unsigned int, void*>& keyPtr =
        *(static_cast<std::unordered_map<unsigned int, void*>*>(archetypeComponentMap[archetype]));
    keyPtr[GetHashCode<Component>()] = new std::vector<Component>();
  }

  template <typename... Components>
  void RegisterArchetype() {
    // Get the hashes through an initializer list, the output type
    // can also be a std::initializer_list<unsigned int> but in this
    // case where getHashCode returns unsigned ints, this will work
    // fine.
    Archetype archetype = {(GetHashCode<Components>())...};

    // This archetype will then be a pointer to a dictionary of vectors
    // where the vectors are each component of the archetype.
    archetypeComponentMap[archetype] = new std::unordered_map<unsigned int, void*>();

    // Initialization list has to be assigned, default value of 0 to avoid
    // dealing with initializer list void returns from
    // initializeArchetypeVector
    auto p = {(InitializeArchetypeVector<Components>(archetype), 0)...};
    (void)p;  // To silence the compiler warning about unused vars

    return;
  }

  // Print out all the archetypes through their hashcodes
  void PrintRegisteredArchetypes() {
    for (auto& keyPair : archetypeComponentMap) {
      for (auto& hashCode : keyPair.first) {
        std::cout << hashCode << " ";
      }
      std::cout << std::endl;
    }
  }

  // This will explain how this will really work
  // In each entity, they will belong to a single archetype
  // By adding a new component to the entity, the entity will now belong to another archetype
  // i.e. (previous archetype) + (new component) = new/existing archetype
  template <typename Component>
  void AddComponent(Entity entity) {
    // Convert the component
    uint32_t componentHashCode = GetHashCode<Component>();

    // Get the entity's current archetype
    Archetype& archetype = entityComponentMap[entity];

    // Check that the new component does not already exist
    bool exists = false;
    for (uint32_t hashCode : archetype) {
      if (hashCode == componentHashCode) {
        exists = true;
      }
    }

    // TODO : Is this actually needed?
    if (exists) {
      std::cout << "Error : Component for this entity already exists" << std::endl;
      return;
    }

    uint32_t entityIndex = entityIndexMap[entity];

    // Update the archetype hashcode
    archetype.push_back(componentHashCode);

    bool reconstruct = false;
    // If the archetype does not exist, it should be created
    if (archetypeComponentMap.find(archetype) == archetypeComponentMap.end()) {
      // Create the new component vector first, after Flush() is called, then
      // the rest of the data will be moved over.
      archetypeComponentMap[archetype] = new std::unordered_map<unsigned int, void*>();
      InitializeArchetypeVector<Component>(archetype);
      reconstruct = true;
    }

    // Add the data to the archetype
    std::unordered_map<unsigned int, void*>& keyPtr =
        *(static_cast<std::unordered_map<unsigned int, void*>*>(archetypeComponentMap[archetype]));

    std::vector<Component>& vectorPtr =
        *(static_cast<std::vector<Component>*>(keyPtr[componentHashCode]));

    // Note that even though this is for each vector component, this should
    // not matter as every component goes through this, resulting in the
    // size (i.e. index) being all the same.
    // flushCache[entity] = {true, reconstruct, componentHashCode, entityIndexMap[entity]};
    flushCache[entity].push_back(
        {Move::ADD, reconstruct, componentHashCode, entityIndexMap[entity]});
    entityIndexMap[entity] = vectorPtr.size();

    vectorPtr.push_back(Component());
  }

  // Likewise for RemoveComponent, it is very similar to AddComponent
  // The issue here is just that there is a need to memcpy...
  // Once that is resolved, this should all be easy to fix.
  template <typename Component>
  void RemoveComponent(Entity entity) {
  }

  // This will make all the changes to the entity.
  // For now, this is assuming that all entities are only using AddComponent and not
  // RemoveComponent
  template <typename... Components>
  void FlushEntity(Entity entity) {
    // If trying to flush an entity that has no prior AddComponent or RemoveComponent
    if (flushCache.find(entity) == flushCache.end()) {
      std::cout << "Warning : Tried to flush an entity id that did not have any operations on it"
                << std::endl;
      return;
    }

    for (ComponentMovementStatus& status : flushCache[entity]) {
      Archetype archetype = entityComponentMap[entity];
      Archetype remainderArchetype;

      if (status.move == Move::ADD) {
        for (size_t i = 0; i < archetype.size(); i++) {
          if (archetype[i] != status.componentHash) {
            remainderArchetype.push_back(archetype[i]);
          }
        }
      } else if (status.move == Move::REMOVE) {  // TODO : Remove
      }

      // Reconstruct the vectors if needed
      if (status.reconstructArchetype) {
        auto p = {(InitializeArchetypeVector<Components>(archetype), 0)...};
        (void)p;
        auto c = {(CreateDefaultComponentValue<Components>(entity, archetype), 0)...};
        (void)c;
      }

      // Move the components from the old archetype to the new archetype
      uint32_t previousIndex = status.previousIndex;
      std::unordered_map<unsigned int, void*>& previousPtr =
          *(static_cast<std::unordered_map<unsigned int, void*>*>(
              archetypeComponentMap[remainderArchetype]));

      std::unordered_map<unsigned int, void*>& currentPtr = *(
          static_cast<std::unordered_map<unsigned int, void*>*>(archetypeComponentMap[archetype]));

      // Move the components around
      auto p = {(MoveComponent<Components>(status.previousIndex, entityIndexMap[entity],
                                           previousPtr, currentPtr, status.componentHash),
                 0)...};
      (void)p;

      // After moving the components from the previous archetype to the new archetype
      // To keep things packed, the space in the previous archetype needs to be filled
      // To fill this space, the latest entity can take its place
      // auto d = {(ReorderComponentVector<Components>(status.previousIndex, previousPtr), 0)...};
      // (void)d;
    }
  }

  template <typename Component>
  void MoveComponent(uint32_t previousIndex, uint32_t currentIndex,
                     std::unordered_map<uint32_t, void*> from,
                     std::unordered_map<uint32_t, void*> to, uint32_t exclude) {
    if (GetHashCode<Component>() == exclude) {
      return;
    }

    uint32_t hashCode = GetHashCode<Component>();
    std::vector<Component>& fromComponentVector =
        *(static_cast<std::vector<Component>*>(from[hashCode]));

    std::vector<Component>& toComponentVector =
        *(static_cast<std::vector<Component>*>(to[hashCode]));

    toComponentVector[currentIndex] = fromComponentVector[previousIndex];
  }

  template <typename Component>
  void ReorderComponentVector(uint32_t fillIndex, std::unordered_map<uint32_t, void*> keyPtr) {
    uint32_t hashCode = GetHashCode<Component>();

    std::vector<Component>& componentVector =
        *(static_cast<std::vector<Component>*>(keyPtr[hashCode]));

    // Cannot move elements around to do anything
    if (componentVector.size() <= 1) {
      return;
    }
  }

  // This is named addComponentData rather than addComponent because this does
  // not actually 'add' a component to an entity. The component is added by
  // default when it is created together with the archetype. What this really
  // does is to replace the data that is created by the default value of the
  // component with the one that is passed in.
  template <typename Component>
  void AddComponentData(Entity entity, Component component) {
    std::unordered_map<unsigned int, void*>& keyPtr =
        *(static_cast<std::unordered_map<unsigned int, void*>*>(
            archetypeComponentMap[entityComponentMap[entity]]));

    // Get a pointer to the vector of this type.
    unsigned int hashCode = GetHashCode(component);
    std::vector<Component>& vectorPtr = *(static_cast<std::vector<Component>*>(keyPtr[hashCode]));
    vectorPtr.at(entityIndexMap[entity]) = component;
  }

  template <typename Component>
  ComponentVector<Component>* MakeComponentVector() {
    return new ComponentVector<Component>();
  }

  template <typename Component>
  void FillComponentVector(Archetype archetype, std::vector<void*>& componentVectors,
                           int& componentIndex) {
    // Find out which index Component belongs in, through hashCodes so that
    // it can Cast to the right type at the right index in componentVectors
    unsigned int hashCode = GetHashCode<Component>();

    // This is a componentVector where all the data is stored.
    ComponentVector<Component>& componentVectorPtr =
        *(static_cast<ComponentVector<Component>*>(componentVectors[componentIndex]));

    std::unordered_map<unsigned int, void*>& keyPtr =
        *(static_cast<std::unordered_map<unsigned int, void*>*>(archetypeComponentMap[archetype]));

    std::vector<Component>* vectorPtr = static_cast<std::vector<Component>*>(keyPtr[hashCode]);

    // Add to componentvector if only the size is more than 0;
    // i.e. if there are entities.
    if (vectorPtr->size() > 0) {
      componentVectorPtr.AddVector(vectorPtr);
    }

    // Increment the index for the next component index when this function
    // gets called in an initializer list
    componentIndex++;
  }

  // This returns a std::vector<void*> that the user will have to cast to its
  // respective type afterwards. The types are known to be users as the order
  // of the <T> that is passed into the function.
  // Usage : getComponents<ComponentA, ComponentB>();
  template <typename... Components>
  std::vector<void*> GetComponents() {
    Archetype hashCodes = {(GetHashCode<Components>())...};
    std::vector<void*> componentVectors = {(MakeComponentVector<Components>())...};

    for (auto& archetypePair : archetypeComponentMap) {
      Archetype archetype = archetypePair.first;
      // I don't think there is a way to do this in a fast way as
      // iterators can only find one element at a time. So this has to be
      // done in separate iterator find calls equal to the number of
      // hashes that exist
      // The flag will turn false if any of the elements dont match
      bool archetypeMatch = true;
      for (unsigned int hashCode : hashCodes) {
        // If it is equals to the end of the iterator - not found
        if (std::find(archetype.begin(), archetype.end(), hashCode) == archetype.end()) {
          archetypeMatch = false;
        }
      }

      // if correct is stil true, this current keypair archetype is
      // matching the input components
      if (archetypeMatch) {
        // Auto fills in the 0 so that the compiler can deal with void
        // returns from fillComponentVector
        int componentIndex = 0;
        auto p = {
            (FillComponentVector<Components>(archetype, componentVectors, componentIndex), 0)...};
        (void)p;  // To silence the compiler warning about unused vars
      }
    }
    return componentVectors;
  }

  // This is to get exact components rather than 'inclusive of <Components>'
  // Usage : getComponentsExact<ComponentA, ComponentB>();
  template <typename... Components>
  std::vector<void*> GetComponentsExact() {
    Archetype hashCodes = {(GetHashCode<Components>())...};
    std::vector<void*> componentVectors = {(MakeComponentVector<Components>())...};

    for (auto& archetypePair : archetypeComponentMap) {
      Archetype archetype = archetypePair.first;
      if (archetype == hashCodes) {
        int componentIndex = 0;
        auto p = {
            (FillComponentVector<Components>(archetype, componentVectors, componentIndex), 0)...};
        (void)p;
      }
    }
    return componentVectors;
  }

  // This differs from getComponents() in the sense that getComponents is used
  // as a way to batch collect all the components of a certain pattern.
  // This exists as a way to get a component given the id.
  template <typename Component>
  Component* GetComponent(Entity id) {
    unsigned int hashCode = GetHashCode<Component>();

    // Check that the entity has the said component in the first place.
    if (!EntityHasComponent<Component>(id)) {
      std::cout << "Entity id : " << id << " does not have component!!" << std::endl;
      return nullptr;
    }

    // get entity archetype
    Archetype archetype = entityComponentMap[id];
    std::unordered_map<unsigned int, void*>& keyPtr =
        *(static_cast<std::unordered_map<unsigned int, void*>*>(archetypeComponentMap[archetype]));

    // Get a pointer to the vector of this type.
    std::vector<Component>& vectorPtr = *(static_cast<std::vector<Component>*>(keyPtr[hashCode]));

    // Get entity index
    unsigned int entityIndex = entityIndexMap[id];
    return &vectorPtr.at(entityIndex);
  }

  void SortArchetype(Archetype& archetype) {
    std::sort(archetype.begin(), archetype.end());
  }

  void RegisterSystem(System* system) {
    systems.push_back(system);
  };

  void UpdateSystems(double dt, Input* input) {
    for (System* system : systems) {
      system->Update(dt, this, input);
    }
  }
};
