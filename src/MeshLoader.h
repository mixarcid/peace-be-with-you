#pragma once

#include "Standard.h"
#include "String.h"
#include "Containers.h"
#include "StaticMesh.h"

NAMESPACE {

  struct MeshLoader {

    //HashMap<String, StaticMesh> static_meshes;
    
    MeshLoader(String filename);
    StaticMesh* getStaticMesh(String mesh_name);
    
  };

}
