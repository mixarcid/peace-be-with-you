#include "PhysicsComp.hpp"
#include "BoundingObject.hpp"
#include "GameObject.hpp"
#include "Engine.hpp"

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

  void StaticPhysicsComp::init(Pointer<GameObject>& object) {
    f32 mass = object->getTightBoundingObject()->getVolume()
      * material.density;
    f32 inertia = object->getTightBoundingObject()->getInertia(mass);
    mass_data = MassData(mass, inertia);
  }

  Vec3f StaticPhysicsComp::getVeloc() {
    return Vec3f(0,0,0);
  }

  bool StaticPhysicsComp::isMoving() {
    return false;
  }

  Vec3f DynamicPhysicsComp::getVeloc() {
    return veloc;
  }

  void DynamicPhysicsComp::update(Pointer<DynamicObject>& object, f32 dt) {
    veloc += force*mass_data.inv_mass*dt;
    object->transRel(veloc*dt);
  }

  void DynamicPhysicsComp::applyImpulse(Vec3f j) {
    veloc += j*mass_data.inv_mass;
  }

  void DynamicPhysicsComp::onMove(Pointer<DynamicObject>& object) {
    if (moving_object_handle == -1) {
      moving_object_handle = Engine::engine->physics.moving_objects.size();
      Engine::engine->physics.moving_objects.emplace_back
	(object);
    }
  }
  void DynamicPhysicsComp::onStop(Pointer<DynamicObject>& object) {
    if (moving_object_handle != -1) {
      Engine::engine->physics.moving_objects.removeAndReplace
	(&Engine::engine->physics.moving_objects[moving_object_handle]);
      if (moving_object_handle <
	  Engine::engine->physics.moving_objects.size()) {
	Engine::engine->physics.moving_objects[moving_object_handle]
	  ->getComponent<DynamicPhysicsComp>()
	  ->moving_object_handle = moving_object_handle;
      }
      moving_object_handle = -1;
      veloc = Vec3f(0,0,0);
      prev_veloc = Vec3f(0,0,0);
    }
  }

  const static f32 VELOC_EPSILON = 0.01f;
  const static f32 VELOC_CHANGE_EPSILON = 0.001f;
  bool DynamicPhysicsComp::isMoving() {
    return (veloc.normSquared() > VELOC_EPSILON) &&
    ((veloc-prev_veloc).normSquared() > VELOC_CHANGE_EPSILON);
  }
  
}
