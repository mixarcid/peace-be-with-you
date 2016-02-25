#include "PhysicsComp.hpp"
#include "BoundingObject.hpp"
#include "GameObject.hpp"

NAMESPACE {

  MassData::MassData(f32 m, f32 i) {
    if (m == 0) {
      mass = 0;
      inv_mass = 0;
    } else {
      mass = m;
      inv_mass = 1/m;
    }
    if (i == 0) {
      inertia = 0;
      inv_inertia = 0;
    } else {
      inertia = i;
      inv_inertia = 1/i;
    }
  }

  const Material Material::STATIC(0,1);

  Material::Material(f32 mat_density,
		     f32 mat_restitution)
    : density(mat_density),
    cor(mat_restitution) {}

  void PhysicsComp::init(GameObject* object) {
    //Log::message("%f", object->getTightBoundingObject()->getVolume());
    f32 mass = object->getTightBoundingObject()->getVolume()
      * material.density;
    f32 inertia = object->getTightBoundingObject()->getInertia(mass);
    mass_data = MassData(mass, inertia);
  }

  void PhysicsComp::update(f32 dt) {
    veloc += force*mass_data.inv_mass*dt;
  }

  void PhysicsComp::applyForce(Vec3f f) {
    force += f;
  }
  
}
