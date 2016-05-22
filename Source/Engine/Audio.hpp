#pragma once

#include "Standard.hpp"
#include "Component.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#include <YSE/yse.hpp>
#pragma GCC diagnostic pop
#pragma clang diagnostic pop

NAMESPACE {

  struct Audio {

    Pointer<GameObject> listener_obj;

    Audio();
    ~Audio();

    void setListener(Pointer<GameObject>& listener);
    void update();
    
  };

  //2d sound
  struct AmbientSound {

    YSE::sound sound;

    AmbientSound();
    void play(String filename, bool use_wav_extension = true);

  };

  //3d sound, must be associated with a GameObject
  struct AudioComp : Component {

    YSE::sound sound;

    AudioComp() {
      $rttiConstruct("AudioComp");
    }

    void onMove(Pointer<GameObject>& obj);
    void play(String filename, GameObject* obj);
    
  };
  $registerRttiStruct();

}
