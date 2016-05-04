#pragma once

#include "GameObject.hpp"
#include "MeshLoader.hpp"

NAMESPACE {

  enum TreeType {

    TREE_TEST,
    TREE_ELM,
    TREE_PINE,
    TREE_WILLOW,
    TREE_LAST

  };

  struct TreeData {
    
    Vec2f pos;
    TreeType type;

    TreeData(Vec2f _pos, TreeType _type)
      : pos(_pos), type(_type) {}
  };

  struct Tree : StaticObject {

    static const u8 NUM_MODELS = 1;		     
    static Asset<StaticMesh> meshes[TREE_LAST*NUM_MODELS];

    //num being the id of the model to use; e.i.,
    //use the 5th elm tree mesh
    Tree(TreeType type, Vec3f pos)
      : StaticObject(pos, Quaternionf
		     (0, 0, (M_PI/(f32)RAND_MAX)*rand())) {
      $rttiConstruct("Tree");
      init(type, (rand()*NUM_MODELS)/RAND_MAX);
    }
    void init(TreeType type, u8 num);
    ~Tree();

  };
  $registerRttiStruct();

}
