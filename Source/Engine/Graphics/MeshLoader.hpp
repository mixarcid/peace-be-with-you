#pragma once

#include "Standard.hpp"
#include "String.hpp"
#include "Containers.hpp"
#include "BonedMesh.hpp"

NAMESPACE {

  struct MeshLoader {

    HashMap<String, StaticMesh*> static_meshes;
    HashMap<String, BonedMeshBase*> boned_meshes;
    Array<Texture*> textures;
    
    MeshLoader(String filename);
    StaticMesh* getStaticMesh(String mesh_name);
    BonedMesh getBonedMesh(String mesh_name);
    ~MeshLoader();
  };

}
