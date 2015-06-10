#pragma once

#include "Standard.hpp"
#include "String.hpp"
#include "Containers.hpp"
#include "StaticMesh.hpp"

NAMESPACE {

  struct MeshLoader {

    HashMap<String, StaticMesh*> static_meshes;
    
    MeshLoader(String filename);
    StaticMesh* getStaticMesh(String mesh_name);
    ~MeshLoader();
  };

}
