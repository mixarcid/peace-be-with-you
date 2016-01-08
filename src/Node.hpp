#pragma once

#include "Standard.hpp"
#include "Transform.hpp"
#include "Renderable.hpp"
#include "HandledArray.hpp"
#include "Messageable.hpp"

NAMESPACE {

  struct BoundingObject;

  struct Node : Transform, Messageable {

    //Node* parent;
    /*BoundingObject* bounding_obj;
    BoundingObject* physical_bounding_obj;
    BoundingObject* primary_bounding_obj;*/
    HandledArray<Node*> children;
    HandledArray<Renderable*> renderables;

    //Node(Node* node_parent);
    using Transform::Transform;
    
    ArrayHandle addChild(Node* child);
    ArrayHandle addRenderable(Renderable* renderable);
    void removeChild(ArrayHandle h);
    void removeRenderable(ArrayHandle h);
    BoundingObject getBoundingObject();
    BoundingObject getPhysicalBoundingObject();
    BoundingObject getPrimaryBoundingObject();
    //Mat4f getAbsMat();
    void render(RenderContext c, Mat4f model);

  };

}
