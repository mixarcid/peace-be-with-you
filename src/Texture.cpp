#include <SOIL/SOIL.h>
#include "Texture.hpp"
#include "Assets.hpp"

NAMESPACE {

  Texture::Texture(u32 num_textures) {
    length = num_textures;
    ids = new u32[length];
    glGenTextures(length, ids);
  }
  
  void Texture::load(String filename, ShaderUniform tex_uniform,
		     u32 index) {
    i32 width, height;
    const char* full_name = (DIR_TEXTURES + filename
			     + DIR_TEXTURE_EXTENSION).c_str();
    unsigned char* image =
      SOIL_load_image(full_name, &width, &height,
		      0, SOIL_LOAD_RGB);

    //LogCurrentDirectory();
    fatalAssert(image != NULL,
	       "Unable to load image %s", full_name);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    SOIL_free_image_data(image);
    
    tex_uniform.registerInt(index);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		    GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		    GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		    GL_LINEAR);
    
    Log::message("Loaded texture %s", filename.c_str());
  }

  void Texture::use(u32 index) {
    GLenum tex_index = GL_TEXTURE0 + index;
    fatalAssert(tex_index < GL_MAX_TEXTURE_UNITS,
	       "The requested texture index is too large.");
    glActiveTexture(tex_index);
    glBindTexture(GL_TEXTURE_2D, ids[index]);
  }
  
  Texture::~Texture() {
    glDeleteTextures(length, ids);
    delete ids;
  }

}
