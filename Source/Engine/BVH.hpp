#include "GameObject.hpp"
#include "BoundingAABB.hpp"

NAMESPACE {

  struct BVHNode {

    BoundingAABB aabb;
    
    u32 parent;
    u32 left;
    u32 right;

    Array<GameObject*> objects;
    
  };

  struct BVH {

    Array<BVHNode> nodes;

  };

}
