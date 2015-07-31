#include "Collision.hpp"
#include "Contact.hpp"

NAMESPACE {

  const static f32 COLLISION_POS_CORRECTION = 0.80;

  void resolveCollision(PhysicalObject* a, PhysicalObject* b) {

    if (b == a) return;
    if (!testIntersection(a->getPhysicalBoundingObject(),
			  b->getPhysicalBoundingObject())) return;
    Log::message("!");
    /*Manifold m;
    if (!testIntersection(a->getPrimaryBoundingObject(),
			  b->getPrimaryBoundingObject()), &m) return;

    f32 vn = Vec3f::dot((b->veloc - a->veloc), m.normal);
    
    if (vn <= 0) return;
    //Log::message("VN: %f", vn);

    f32 e = min(a->material.cor, b->material.cor);
    f32 j = ((1 + e) * vn)
      /(a->mass_data.inv_mass + b->mass_data.inv_mass);

    Vec3f impulse = m.normal * j;
    //Log::message((impulse*a->mass_data.inv_mass).toString());
    a->veloc += impulse*a->mass_data.inv_mass;
    b->veloc -= impulse*b->mass_data.inv_mass;

    f32 cf  = COLLISION_POS_CORRECTION * m.penetration
      / (a->mass_data.inv_mass + b->mass_data.inv_mass);
    Vec3f correction = -m.normal*cf;
    a->translateRel(-correction * a->mass_data.inv_mass);
    b->translateRel(correction * b->mass_data.inv_mass);*/
    
  }

}
