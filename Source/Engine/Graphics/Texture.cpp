#include <SOIL/SOIL.h>
#include "Texture.hpp"
#include "Assets.hpp"

NAMESPACE {

  Texture::~Texture() {
    if (PEACE_GL_SHOULD_DELETE) {
      glDeleteTextures(1, &this->id);
    }
  }

  void Texture::init() {
    GLObject::init();
    glGenTextures(1, &this->id);
  }
  
  void Texture::loadFromFile(String filename) {
    PEACE_GL_CHECK_ERROR;
    i32 width, height;
    String full_name_str = DIR_TEXTURES + filename
      + DIR_TEXTURE_EXTENSION;
    const char* full_name = full_name_str.c_str();

    use();
    
    unsigned char* image =
      SOIL_load_image(full_name, &width, &height,
		      0, SOIL_LOAD_RGBA);

    fatalAssert(image != NULL,
	       "Unable to load image %s", full_name);
    PEACE_GL_CHECK_ERROR;
    glTexImage2D(GL_TEXTURE_2D,
		 0, GL_RGBA, width, height,
		 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    PEACE_GL_CHECK_ERROR;
    glGenerateMipmap(GL_TEXTURE_2D);
    PEACE_GL_CHECK_ERROR;
    
    SOIL_free_image_data(image);
    
    PEACE_GL_CHECK_ERROR;
    glTexParameteri(GL_TEXTURE_2D,
		    GL_TEXTURE_WRAP_S,
		    GL_REPEAT);
    PEACE_GL_CHECK_ERROR;
    glTexParameteri(GL_TEXTURE_2D,
		    GL_TEXTURE_WRAP_T,
		    GL_REPEAT);
    PEACE_GL_CHECK_ERROR;
    glTexParameteri(GL_TEXTURE_2D,
		    GL_TEXTURE_MIN_FILTER,
		    GL_LINEAR);
    PEACE_GL_CHECK_ERROR;
    glTexParameteri(GL_TEXTURE_2D,
		    GL_TEXTURE_MAG_FILTER,
		    GL_LINEAR);
    PEACE_GL_CHECK_ERROR;

  }

  void Texture::createEmpty(Vec2i size) {
    use();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x(), size.y(),
		 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
		    GL_TEXTURE_WRAP_S,
		    GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
		    GL_TEXTURE_WRAP_T,
		    GL_CLAMP_TO_EDGE);
  }

  void Texture::use() {
    debugAssert(!(this->flags & PEACE_GL_UNINITIALIZED),
		"You must initialize a Texture before using it");
    glBindTexture(GL_TEXTURE_2D, this->id);
  }

  DEFINE_ASSET_LOADER(Texture);
  
  template<>
    Texture* loadAsset<Texture>(String name) {
    auto inserted = AssetLoader<Texture>::loaded_assets
      .insert(makePair(name, Texture()));
    debugAssert(inserted.second,
		"There was a problem inserting Texture %s"
		" into the HashMap", name.c_str());
    inserted.first->second.init();
    inserted.first->second.loadFromFile(name);
#ifdef PEACE_LOG_LOADED_ASSETS
    Log::message("Loaded Texture %s", name.c_str());
#endif
    return &inserted.first->second;
  }

}
