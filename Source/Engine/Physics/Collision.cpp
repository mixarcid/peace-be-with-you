#include "Collision.hpp"
#include "Manifold.hpp"

NAMESPACE {

  const static f32 COLLISION_POS_CORRECTION = 0.9;
  const static f32 VN_DEAD_SPACE = 0.001;

  template <typename Obj, typename Comp, bool Static>
    static inline bool collisionHelper(DynamicObject* a_obj,
				       DynamicPhysicsComp* a_comp,
				       Obj* b_obj,
				       Comp* b_comp) {
    if (b_comp == a_comp) return false;
    
    BoundingObject* a_bound = a_obj->getLooseBoundingObject();
    BoundingObject* b_bound = b_bound = b_obj->getLooseBoundingObject();
    
    if (!a_bound->intersects(b_bound)) {
      return false;
    }
    
    a_bound = a_obj->getTightBoundingObject();
    b_bound = b_obj->getTightBoundingObject();
    
    Manifold m;
    if (!a_bound->intersects(b_bound, &m)) {
      return false;
    }

    f32 cf  = COLLISION_POS_CORRECTION * m.penetration
      / (a_comp->mass_data.inv_mass + b_comp->mass_data.inv_mass);
    Vec3f correction = m.normal*cf;
    a_obj->transRel(-correction * a_comp->mass_data.inv_mass);
    b_obj->transRel(correction * b_comp->mass_data.inv_mass);
    
    Vec3f rv = b_comp->getVeloc() - a_comp->getVeloc();
    f32 vn = Vec3f::dot(rv, m.normal);

    if (abs(vn) < VN_DEAD_SPACE) return true;
    
    f32 e = min(a_comp->material.cor, b_comp->material.cor);
    f32 j = ((1 + e) * vn)
      /(a_comp->mass_data.inv_mass + b_comp->mass_data.inv_mass);
    Vec3f impulse = m.normal * j;

        
    /*if (Static) {

      a_comp->veloc += impulse*a_comp->mass_data.inv_mass;
      a_obj->transRel(-m.normal*m.penetration*COLLISION_POS_CORRECTION);
      //Log::message("%f", m.penetration);
      
      } else {*/
    
    a_comp->applyImpulse(impulse);
    b_comp->applyImpulse(-impulse);

    //friction stuff
    rv = b_comp->getVeloc() - a_comp->getVeloc();
    Vec3f t = rv - (m.normal*Vec3f::dot(rv, m.normal));
    t.normalize();
    f32 jt = -Vec3f::dot(rv, t) /
      (a_comp->mass_data.inv_mass +
       b_comp->mass_data.inv_mass);
    Vec3f f_impulse;
    f32 cof = (a_comp->material.cof_static +
	       a_comp->material.cof_static)/2;
    if (abs(jt) < j*cof) {
      f_impulse = t*jt;
    } else {
      cof = (a_comp->material.cof_dynamic +
	     a_comp->material.cof_dynamic)/2;
      f_impulse = -t*j*cof;
    }
    //Log::message(to_string(f_impulse) + to_string(impulse));
    a_comp->applyImpulse(f_impulse);
    if (!Static) {
      b_comp->applyImpulse(-f_impulse);
    }
	
    CollisionMessage a_msg(a_obj);
    CollisionMessage b_msg(b_obj);
    a_obj->message(&b_msg);
    b_obj->message(&a_msg);

    return true;
  }

  bool resolveCollision(DynamicComponentPair<DynamicPhysicsComp> a,
			DynamicComponentPair<DynamicPhysicsComp> b) {
    return collisionHelper<DynamicObject, DynamicPhysicsComp, false>
      (a.obj, a.comp, b.obj, b.comp);
  }

  bool resolveCollision(DynamicComponentPair<DynamicPhysicsComp> a,
			StaticComponentPair<StaticPhysicsComp> b) {
    return collisionHelper<StaticObject, StaticPhysicsComp, false>
      (a.obj, a.comp, b.obj, b.comp);
  }
}