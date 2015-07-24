#include "Node.hpp"
#include "BoundingObject.hpp"
#include "Shader.hpp"

NAMESPACE {

  //Node::Node(Node* node_parent) : parent(node_parent) {}

  void Node::addChild(Node* child) {
    children.push_back(child);
  }

  void Node::addRenderable(Renderable* renderable) {
    /*if (renderables.size() == 0) {
      bounding_obj = renderable->getBoundingObject();
      physical_bounding_obj = renderable->getPhysicalBoundingObject();
      primary_bounding_obj = renderable->getPrimaryBoundingObject();
      }*/
    renderables.push_back(renderable);
  }

  BoundingObject Node::getBoundingObject() {
    debugAssert(renderables.size() > 0,
	       "Why haven't you attached a renderable to the node?");
    BoundingObject obj =  renderables[0]->getBoundingObject();
    obj.transform(this);
    return obj;
  }
  
  BoundingObject Node::getPhysicalBoundingObject() {
    debugAssert(renderables.size() > 0,
	       "Why haven't you attached a renderable to the node?");
    BoundingObject obj = renderables[0]->getPhysicalBoundingObject();
    obj.transform(this);
    return obj;
  }

  BoundingObject Node::getPrimaryBoundingObject() {
    debugAssert(renderables.size() > 0,
	       "Why haven't you attached a renderable to the node?");
    BoundingObject obj = renderables[0]->getPrimaryBoundingObject();
    obj.transform(this);
    return obj;
  }

  Mat4f Node::getAbsMat() {
    if (parent) {
      return parent->getAbsMat() *  this->getMat();
    } else {
      return this->getMat();
    }
  }

  void Node::render(Mat4f mat) {
    Mat4f comb = this->getMat() * mat;
    //Log::message(comb.toString());
    for (Node* child : children) {
      child->render(comb);
    }
    for (Renderable* rend : renderables) {
      Shader::UNI_MODEL.registerMat4f(comb);
      rend->render();
    }
  }
}
