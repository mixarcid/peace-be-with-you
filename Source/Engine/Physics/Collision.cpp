#include "Collision.hpp"
#include "Manifold.hpp"

NAMESPACE {

  const static f32 COLLISION_POS_CORRECTION = 0.9;

  bool resolveCollision(ComponentPair<PhysicsComp> a,
			ComponentPair<PhysicsComp> b,
			bool second_arg_is_static) {

    if (b.comp == a.comp) return false;
    
    BoundingObject* a_bound;
    BoundingObject* b_bound;
    a_bound = a.obj->getLooseBoundingObject()->transform(a.obj->getBasicTransform());
    b_bound = b.obj->getLooseBoundingObject()->transform(b.obj->getBasicTransform());
    
    if (!a_bound->intersects(b_bound)) {
      delete a_bound;
      delete b_bound;
      return false;
    }
    delete a_bound;
    delete b_bound;
    
    a_bound = a.obj->getTightBoundingObject()->transform(a.obj->getBasicTransform());
    b_bound = b.obj->getTightBoundingObject()->transform(b.obj->getBasicTransform());
    
    Manifold m;
    if (!a_bound->intersects(b_bound, &m)) {
      delete a_bound;
      delete b_bound;
      return false;
    }
    delete a_bound;
    delete b_bound;
    
    //Log::message(to_string(m.normal) + " " + to_string(m.penetration));
    
    Vec3f rv = b.comp->veloc - a.comp->veloc;
    f32 vn = Vec3f::dot(rv, m.normal);

    f32 e = min(a.comp->material.cor, b.comp->material.cor);
    f32 j = ((1 + e) * vn)
      /(a.comp->mass_data.inv_mass + b.comp->mass_data.inv_mass);
    Vec3f impulse = m.normal * j;

        
    /*if (second_arg_is_static) {

      a.comp->veloc += impulse*a.comp->mass_data.inv_mass;
      a.obj->transRel(-m.normal*m.penetration*COLLISION_POS_CORRECTION);
      //Log::message("%f", m.penetration);
      
      } else {*/
      
    f32 cf  = COLLISION_POS_CORRECTION * m.penetration
      / (a.comp->mass_data.inv_mass + b.comp->mass_data.inv_mass);
    Vec3f correction = m.normal*cf;

    a.obj->transRel(-correction * a.comp->mass_data.inv_mass);
    b.obj->transRel(correction * b.comp->mass_data.inv_mass);
    a.comp->applyImpulse(impulse);
    b.comp->applyImpulse(-impulse);

    //friction stuff
    rv = b.comp->veloc - a.comp->veloc;
    Vec3f t = rv - (m.normal*Vec3f::dot(rv, m.normal));
    t.normalize();
    f32 jt = -Vec3f::dot(rv, t) /
      (a.comp->mass_data.inv_mass +
       b.comp->mass_data.inv_mass);
    Vec3f f_impulse;
    f32 cof = (a.comp->material.cof_static +
	       a.comp->material.cof_static)/2;
    if (abs(jt) < j*cof) {
      f_impulse = t*jt;
    } else {
      cof = (a.comp->material.cof_dynamic +
	     a.comp->material.cof_dynamic)/2;
      f_impulse = -t*j*cof;
    }
    //Log::message(to_string(f_impulse) + to_string(impulse));
    a.comp->applyImpulse(f_impulse);
    b.comp->applyImpulse(-f_impulse);
	
    CollisionMessage a_msg(a);
    CollisionMessage b_msg(b);
    a.obj->message(&b_msg);
    b.obj->message(&a_msg);

    return true;
  }

}
