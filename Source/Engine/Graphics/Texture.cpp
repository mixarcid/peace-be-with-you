#include <SOIL/SOIL.h>
#include "Texture.hpp"
#include "Assets.hpp"

NAMESPACE {

  Texture::Texture(u32 num_textures)
    : length(num_textures) {}

  Texture::~Texture() {
    if (ids) {
      glDeleteTextures(length, ids);
      delete[] ids;
    }
  }

  void Texture::init() {
    ids = new u32[length];
    glGenTextures(length, ids);
  }
  
  void Texture::load(String filename,
		     ShaderUniform tex_uniform,
		     u32 index) {
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
    
    tex_uniform.registerInt(index);
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

  void Texture::use(u32 index) {
    GLenum tex_index = GL_TEXTURE0 + index;
    fatalAssert(tex_index < GL_MAX_TEXTURE_UNITS,
	       "The requested texture index is too large.");
    glActiveTexture(tex_index);
    glBindTexture(GL_TEXTURE_2D, ids[index]);
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
    inserted.first->second.load(name, Shader::UNI_TEXTURE);
#ifdef PEACE_LOG_LOADED_ASSETS
    Log::message("Loaded Texture %s", name.c_str());
#endif
    return &inserted.first->second;
  }

}
