#include "Collision.hpp"
#include "Contact.hpp"

NAMESPACE {

  const static f32 COLLISION_POS_CORRECTION = 0.80;

  bool resolveCollision(GameObject* a, GameObject* b) {

    if (b == a) return false;

    PhysicsComponent* ac
      = a->getComponent<PhysicsComponent>();
    if (!ac) return false;

    PhysicsComponent* bc
      = b->getComponent<PhysicsComponent>();
    if (!bc) return false;
    
    if (!testIntersection(a->getLooseBoundingObject(),
			  b->getLooseBoundingObject())) return false;
    Manifold m;
    if (!testIntersection(a->getTightBoundingObject(),
			  b->getTightBoundingObject(), &m)) return false;

    //Log::message(to_string(m.normal));
    
    f32 vn = Vec3f::dot((b->veloc - a->veloc), m.normal);
    
    //if (vn <= 0) return;
    //Log::message("VN: %f", vn);

    f32 e = min(a->material.cor, b->material.cor);
    f32 j = ((1 + e) * vn)
      /(a->mass_data.inv_mass + b->mass_data.inv_mass);

    Vec3f impulse = m.normal * j;
    //Log::message(to_string(impulse));
    //Log::message((impulse*a->mass_data.inv_mass).toString());
    a->veloc += impulse*a->mass_data.inv_mass;
    b->veloc -= impulse*b->mass_data.inv_mass;

    f32 cf  = COLLISION_POS_CORRECTION * m.penetration
      / (a->mass_data.inv_mass + b->mass_data.inv_mass);
    Vec3f correction = m.normal*cf;
    a->translateRel(-correction * a->mass_data.inv_mass);
    b->translateRel(correction * b->mass_data.inv_mass);

    CollisionMessage a_msg(a);
    CollisionMessage b_msg(b);
    a->message(&b_msg);
    b->message(&a_msg);

    return true;
  }

}
