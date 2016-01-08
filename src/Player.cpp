#include "Player.hpp"
#include "MeshLoader.hpp"

NAMESPACE {

  BonedMesh Player::mesh;
  static MeshLoader* loader;

  CONSTRUCT_ASSET(PLAYER_LOADER);
 
  loader = new MeshLoader("SubjectB");
  Player::mesh = loader->getBonedMesh("Subject");
  
  DELETE_ASSET;
 
  delete loader;
  
  END_ASSET;

  void Player::update() {
    cam.translateAbs(this->trans+Vec3f(5,5,0));
  }

  Player::~Player() {
    for (MonkeyHead* head : heads) {
      delete head;
    }
  }

}
