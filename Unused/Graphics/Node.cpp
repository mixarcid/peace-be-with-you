#include "Node.hpp"
#include "BoundingObject.hpp"
#include "Shader.hpp"

NAMESPACE {

  //Node::Node(Node* node_parent) : parent(node_parent) {}

  ArrayHandle Node::addChild(Node* child) {
    debugAssert(child, "Why are you passing to NULL pointer to addChild?");
    return children.push_back(child);
  }
  ArrayHandle Node::addRenderable(Renderable* renderable) {
    debugAssert(renderable, "Why are you passing to NULL pointer to addRenderable?");
    return renderables.push_back(renderable);
  }
  void Node::removeChild(ArrayHandle h) {
    children.removeAndReplace(h);
  }
  void Node::removeRenderable(ArrayHandle h) {
    children.removeAndReplace(h);
  }

  BoundingObject Node::getTightBoundingObject() {
    debugAssert(renderables.size() > 0,
	       "Why haven't you attached a renderable to the node?");
    BoundingObject obj = renderables[0]->getTightBoundingObject();
    obj.transform(this);
    return obj;
  }
  
  BoundingObject Node::getLooseBoundingObject() {
    debugAssert(renderables.size() > 0,
	       "Why haven't you attached a renderable to the node?");
    BoundingObject obj = renderables[0]->getLooseBoundingObject();
    obj.transform(this);
    return obj;
  }

  /*Mat4f Node::getAbsMat() {
    if (parent) {
      return parent->getAbsMat() *  this->getMat();
    } else {
      return this->getMat();
    }
    }*/

  void Node::render(RenderContext c, Mat4f model) {
    Mat4f comb = model*Transform::getMat();
    //Log::message("!");
    //Log::message(to_string(comb));
    for (Node* child : children) {
      child->render(c, comb);
    }
    for (Renderable* rend : renderables) {
      Shader::UNI_MODEL.registerVal(comb);
      rend->render(c);
    }
  }
}

/*|   1.000   0.000   0.000   0.000 |
  |   0.000   0.000  -1.000   0.000 |
  |  -0.000   1.000   0.000   0.000 |
  |   0.000   0.000   0.000   1.000 |*/
