#include "PhysicalObjects.hpp"

NAMESPACE {

  DynamicObject::DynamicObject(float m, Vec3f xi, Vec3f vi)
    : Node(xi), mass(m), veloc(vi) {}

  void DynamicObject::updatePhysics(float dt) {

    for (Vec3f accel : accels) {
      veloc += accel * dt;
    }
    Node::translateRel(veloc * dt);

  }

  unsigned int DynamicObject::addForce(Vec3f force) {
    accels.push_back(force/mass); //that's right
    return accels.size() - 1;
  }

  void DynamicObject::removeForce(unsigned int index) {
    removeAndReplace(accels, index);
  }

}
