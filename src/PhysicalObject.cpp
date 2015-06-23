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

  Material::Material(f32 mat_density, f32 mat_restitution)
    : density(mat_density), cor(mat_restitution) {}

  PhysicalObject::PhysicalObject(Renderable* rend, Material mat,
				 Vec3f xi, Vec3f vi)
    : Node(xi), material(mat), mass_data(0,0),
    veloc(vi), force(Vec3f(0,0,0)) {
    
    Node::addRenderable(rend);
    f32 mass = Node::getPrimaryBoundingObject().getVolume()
      * mat.density;
    f32 inertia = Node::getPrimaryBoundingObject().getInertia(mass);
    mass_data = MassData(mass, inertia);
    
  }

  void PhysicalObject::update(f32 dt) {

    veloc += force*mass_data.inv_mass*dt;
    Node::translateRel(veloc * dt);

  }

  void PhysicalObject::applyForce(Vec3f f) {
    force += f;
  }

  StaticObject::StaticObject(Renderable* rend, Vec3f xi, Vec3f vi)
    : PhysicalObject(rend, Material::STATIC, xi, vi) {}
}
