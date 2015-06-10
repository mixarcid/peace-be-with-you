#pragma once

#include "Graphics.hpp"
#include "Standard.hpp"
#include "Containers.hpp"

NAMESPACE {

  struct ShaderVar;

  const unsigned int DEFAULT_NUM_VAOS = 200;
  
  struct VAO {

    unsigned int index;

    void use();
    /*the types of the shader vars must entirely be composed of 
      either doubles or floats; for example Vector3f and Color4d 
      must not be mixed */
    void registerVars(Array<ShaderVar> vars);

    static unsigned int length;
    static GLuint* ids;
    static bool is_initialized;
    static unsigned int current;

    static void init(unsigned int num_vaos = DEFAULT_NUM_VAOS);
    static VAO getVAO();
    static void terminate();
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
    /*void rebindArray(Array<GLuint> arr, bool dynamic,
      unsigned int index = 0);*/
    void draw(GLenum mode, unsigned int index = 0);
    ~EBO();
    
  };
  
}
