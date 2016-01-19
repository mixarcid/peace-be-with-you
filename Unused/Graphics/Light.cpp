#include "Light.hpp"

NAMESPACE {

  DirLight::DirLight(Vec3f _dir, Vec3f _color)
    : dir(_dir.x(), _dir.z(), _dir.y()),
    color(_color) {}

}
