#pragma once

#include <Common.h>

namespace CainEngine::EntitySystem {

class Scene;
class EntityID;

// Transform (mutable view) and CTransform (read-only view) are two instantiations of the same
// TransformImpl<bool> template; see EntitySystem/Transform.h.
template<bool IsConst>
class TransformImpl;

using Transform = TransformImpl<false>;
using CTransform = TransformImpl<true>;

}; // namespace CainEngine::EntitySystem

#include "EntitySystem/Enums.h"
#include "EntitySystem/Structs.h"
#include "EntitySystem/EntityID.h"

#include "EntitySystem/Scene.h"
#include "EntitySystem/Transform.h"
#include "EntitySystem/Scene.inl"