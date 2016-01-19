#include "PhysicalObject.hpp"
#include "BoundingObject.hpp"

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

  PhysicalObject::PhysicalObject(Renderable* rend,
				 Material mat,
				 Vec3f xi,
				 Vec3f vi,
				 PhysicalObjectFlags _flags)
    : Node(xi),
    material(mat),
    mass_data(0,0),
    veloc(vi),
    force(Vec3f(0,0,0)),
    physics_flags(_flags) {

      if (rend) {
	this->addRenderable(rend);
	if (!(physics_flags & PHYSICS_STATIC)) {
	  f32 mass = this->getTightBoundingObject().getVolume()
	    * mat.density;
	  f32 inertia = this->getTightBoundingObject().getInertia(mass);
	  mass_data = MassData(mass, inertia);
	}
      } else {
	mass_data = MassData(0,0);
      }
    }

  void PhysicalObject::update(f32 dt) {

    veloc += force*mass_data.inv_mass*dt;
    Node::translateRel(veloc * dt);

  }

  void PhysicalObject::applyForce(Vec3f f) {
    force += f;
  }

  StaticObject::StaticObject(Renderable* rend, Vec3f xi)
    : PhysicalObject(rend,
		     Material::STATIC,
		     xi,
		     Vec3f(0,0,0),
		     PHYSICS_STATIC) {}
}
