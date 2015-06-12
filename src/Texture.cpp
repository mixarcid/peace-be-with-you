#include <SOIL/SOIL.h>
#include "Texture.hpp"
#include "FileSystem.hpp"

NAMESPACE {

  /*Texture::Texture(Texture* tex) {
    index = tex->index;
    }*/

  unsigned int Texture::length = 0;
  GLuint* Texture::ids = NULL;
  bool Texture::is_initialized  = 0;
  unsigned int Texture::current = 0;

  
  void Texture::load(String filename, ShaderUniform tex_uniform) {
    int width, height;
    const char* full_name = (DIR_TEXTURES + filename
			     + DIR_TEXTURE_EXTENSION).c_str();
    unsigned char* image =
      SOIL_load_image(full_name, &width, &height,
		      0, SOIL_LOAD_RGB);

    //logCurrentDirectory();
    exitAssert(image != NULL,
	       "Unable to load image %s", full_name);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    SOIL_free_image_data(image);
    
    tex_uniform.registerInt(index);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		    GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		    GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		    GL_LINEAR);
    
    log::message("Loaded texture %s", filename.c_str());
  }

  void Texture::use() {
    GLenum tex_index = GL_TEXTURE0 + index;
    exitAssert(tex_index < GL_MAX_TEXTURE_UNITS,
	       "The requested texture index is too large.");
    glActiveTexture(tex_index);
    glBindTexture(GL_TEXTURE_2D, ids[index]);
  }
  

  void Texture::init(unsigned int num_vaos) {
    glGenTextures(length, ids);
    Texture::length = num_vaos;
    Texture::ids = new GLuint[length];
    Texture::is_initialized = true;
  }

  Texture Texture::getTexture() {
    static unsigned int last_index = 0;
    Texture ret;
    ret.index = last_index++;
    return ret;
  }
  
  void Texture::terminate() {
    if (Texture::ids != NULL && Texture::is_initialized) {
      glDeleteTextures(length, ids);
      delete ids;
    } else {
      log::error("Why are you calling Texture::terminate() without"
		 " calling Texture::init()?");
    }
  }

}
