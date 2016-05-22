#include "Audio.hpp"
#include "Assets.hpp"
#include "GameObject.hpp"

NAMESPACE {

  const f32 AUDIO_DISTANCE_SCALE = 0.01;
  
  Audio::Audio() {
    YSE::System().init();
  }
  
  Audio::~Audio() {
    YSE::System().close();
  }

  static inline YSE::Vec vecConvert(Vec3f vec) {
    Vec3f vec2(vec);
    return YSE::Vec(vec2.x(), vec2.z(), vec2.y());
  }

  void Audio::setListener(Pointer<GameObject>& listener) {
    listener_obj = listener;
  }

  void Audio::update() {
    debugAssert(listener_obj,
		"You must set a listener object before calling Audio::update()");
    Vec3f pos(listener_obj->getTrans()*AUDIO_DISTANCE_SCALE);
    Quaternionf rot(listener_obj->getRot());
    Vec3f forward(rot*Vec3f(0,1,0));
    Vec3f up(rot*Vec3f(0,0,1));
    YSE::Listener().setPosition(vecConvert(pos));
    YSE::Listener().setOrientation(vecConvert(forward),
				   vecConvert(up));
    YSE::System().update();
  }

  AmbientSound::AmbientSound() {}
  
  void AmbientSound::play(String filename, bool use_wav_extension) {
    String fullname = DIR_AUDIO + filename;
    if (use_wav_extension) {
      fullname += DIR_AUDIO_EXTENSION;
    }
    sound.create(fullname.c_str(), &YSE::ChannelAmbient())
      .setRelative(true)
      .setDoppler(false)
      .play();
  }

  void AudioComp::onMove(Pointer<GameObject>& obj) {
    if (sound.isValid()) {
      sound.setPosition(vecConvert(obj->getTrans()*AUDIO_DISTANCE_SCALE));
    }
  }

  void AudioComp::play(String filename, GameObject* obj) {
    if (!sound.isPlaying()) {
      String fullname = DIR_AUDIO + filename + DIR_AUDIO_EXTENSION;
      sound.create(fullname.c_str(), &YSE::ChannelFX())
	.setPosition(vecConvert(obj->getTrans()*AUDIO_DISTANCE_SCALE))
	.play();
    }
  }
  
}
