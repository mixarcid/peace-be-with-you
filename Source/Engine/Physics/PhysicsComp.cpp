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

  Material::Material(f32 _density,
		     f32 _restitution,
		     f32 _cof_static,
		     f32 _cof_dynamic)
    : density(_density),
    cor(_restitution),
    cof_static(_cof_static),
    cof_dynamic(_cof_dynamic) {}

  StaticMaterial::StaticMaterial(f32 cof_static,
				 f32 cof_dynamic)
    : Material(1, 0, cof_static, cof_dynamic) {}

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

  void PhysicsComp::applyImpulse(Vec3f j) {
    veloc += j*mass_data.inv_mass;
  }
  
}
