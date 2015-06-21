#pragma once

#include "Standard.hpp"
#include "Renderable.hpp"
#include "VectorMath.hpp"
#include "Containers.hpp"
#include "String.hpp"
#include "Texture.hpp"
#include "BasicMesh.hpp"
#include "BoundingObject.hpp"

NAMESPACE {

  struct StaticMesh : Renderable {
    
    Array<BasicMeshData> data;
    Array<GLuint> elements;
    Texture* tex;
    BoundingSphere b_sphere;
    
    StaticMesh(Array<BasicMeshData> mesh_data,
	       Array<GLuint> mesh_elems, Texture* texture);
    virtual BoundingObject* getBoundingObject();
    virtual BoundingObject* getPhysicalBoundingObject();
    virtual void render();
    ~StaticMesh();
    
  };
  
}
