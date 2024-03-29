#ifndef _REPLICATOR_DEEPCOPY_H_
#define _REPLICATOR_DEEPCOPY_H_

#include "entt/entt.hpp"

entt::entity deepcopy( entt::registry& registry, entt::entity entity, entt::entity copy_parent = entt::null );

void deepdelete( entt::registry& registry, entt::entity entity );

#endif // _REPLICATOR_DEEPCOPY_H_
