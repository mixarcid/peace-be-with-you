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

}
