#include "Node.hpp"
#include "Shader.hpp"

NAMESPACE {

  //Node::Node(Node* node_parent) : parent(node_parent) {}

  void Node::addChild(Node* child) {
    children.push_back(child);
  }

  void Node::addRenderable(Renderable* renderable) {
    renderables.push_back(renderable);
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
    for (Node* child : children) {
      child->render(comb);
    }
    for (Renderable* rend : renderables) {
      Shader::UNI_MODEL.registerMat4f(comb);
      rend->render();
    }
  }
}
