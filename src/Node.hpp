#pragma once

#include "Standard.hpp"
#include "Transform.hpp"
#include "Renderable.hpp"
#include "Containers.hpp"

NAMESPACE {

  struct Node : Transform {

    Node* parent;
    Array<Node*> children;
    Array<Renderable*> renderables;

    //Node(Node* node_parent);
    using Transform::Transform;
    
    void addChild(Node* child);
    void addRenderable(Renderable* renderable);
    Mat4f getAbsMat();
    void render(Mat4f trans);

  };

}
