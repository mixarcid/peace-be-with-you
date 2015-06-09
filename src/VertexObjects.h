#pragma once

#include "Graphics.h"
#include "Standard.h"
#include "Containers.h"

NAMESPACE {

  struct ShaderVar;

  const unsigned int NUM_VAOS = 200;
  
  struct VAO {

    unsigned int length;
    GLuint* ids;
    bool is_initialized;

    VAO(unsigned int num_vaos, bool initialize = true);
    void init();
    void use(unsigned int index  = 0);
    /*the types of the shader vars must entirely be composed of 
      either doubles or floats; for example Vector3f and Color4d 
      must not be mixed */
    void registerVars(Array<ShaderVar> vars,
		      unsigned int index = 0);
    ~VAO();

    static VAO* current_vao;
    static VAO all_vaos;

    static unsigned int getNextVAOIndex();
  };

  struct VBO {
    
    unsigned int length;
    GLuint* ids;
    unsigned int* lengths;

    VBO(unsigned int num_vaos = 1);
    template <typename T>
    void bindArray(Array<T> arr, bool dynamic,
		   unsigned int index = 0) {
      lengths[index] = arr.size()*sizeof(T);
      glBindBuffer(GL_ARRAY_BUFFER, ids[index]);
      glBufferData(GL_ARRAY_BUFFER, lengths[index], &arr[0],
		   dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }
    //the new array must be the same size as before
    template <typename T>
    void rebindArray(Array<T> arr, bool dynamic,
		   unsigned int index = 0) {
      glBufferData(GL_ARRAY_BUFFER, lengths[index], &arr[0],
		   dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }
    void draw(GLenum mode, unsigned int index = 0);
    ~VBO();
    
  };

  struct EBO {
    
    unsigned int length;
    GLuint* ids;
    unsigned int* lengths;

    EBO(unsigned int num_ebos = 1);
    void bindArray(Array<GLuint> arr, bool dynamic,
		   unsigned int index = 0);
    //the new array must be the same size as before
    void rebindArray(Array<GLuint> arr, bool dynamic,
		     unsigned int index = 0);
    void draw(GLenum mode, unsigned int index = 0);
    ~EBO();
    
  };
  
}
