#include "MonkeyHead.hpp"
#include "Assets.hpp"

NAMESPACE {

  StaticMesh* MonkeyHead::mesh = NULL;
  static MeshLoader* loader;

  CONSTRUCT_ASSET(MONKEY_LOADER);

  loader = new MeshLoader("Monkey");
  MonkeyHead::mesh = loader->getStaticMesh("Suzanne");
  
  DELETE_ASSET;

  delete loader;
  
  END_ASSET;

  MonkeyHead::MonkeyHead(Vec3f pos,
			 Vec3f veloc,
			 Physics* phys,
			 Graphics* graphics)
    : PhysicalObject(mesh, Material(1, 0.5), pos, veloc) {
    graphics->addNode(this);
    phys->addDynamicObject(this);
  }

  MonkeyHead::~MonkeyHead() {}

}
