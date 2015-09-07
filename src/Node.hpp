#pragma once

#include "Standard.hpp"
#include "Transform.hpp"
#include "Renderable.hpp"
#include "Containers.hpp"

NAMESPACE {

  struct BoundingObject;

  struct Node : Transform {

    //Node* parent;
    /*BoundingObject* bounding_obj;
    BoundingObject* physical_bounding_obj;
    BoundingObject* primary_bounding_obj;*/
    Array<Node*> children;
    Array<Renderable*> renderables;

    //Node(Node* node_parent);
    using Transform::Transform;
    
    void addChild(Node* child);
    void addRenderable(Renderable* renderable);
    BoundingObject getBoundingObject();
    BoundingObject getPhysicalBoundingObject();
    BoundingObject getPrimaryBoundingObject();
    //Mat4f getAbsMat();
    void render(RenderContext c, Mat4f model);

  };

}
