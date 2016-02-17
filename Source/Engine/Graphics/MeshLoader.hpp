#pragma once

#include "Standard.hpp"
#include "String.hpp"
#include "Containers.hpp"
#include "BonedMesh.hpp"
#include "Assets.hpp"

NAMESPACE {

  /*Both these Assets expect the name to be 
    in the form "Filename:Meshname"
   */
  template<>
    StaticMesh* loadAsset<StaticMesh>(String name);
  template<>
    BonedMeshBase* loadAsset<BonedMeshBase>(String name);
  
}
