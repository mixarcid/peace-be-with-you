#include "StaticMesh.hpp"
#include "Quaternion.hpp"

NAMESPACE {

  const u8 MAX_WEIGHTS_PER_VERTEX;

  struct Bone {
    Quaternionf rot;
  };

  struct BonedMeshData {
    struct {
      f32 weight;
      u32 index;
    } data[MAX_WEIGHTS_PER_VERTEX];
  }

  struct BonedMeshBase : StaticMesh {
    
  };

  struct BonedMesh : Renderable {

    

  }

}
