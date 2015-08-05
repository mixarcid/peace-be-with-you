#pragma once

#include "GL.hpp"
#include "Standard.hpp"
#include "Containers.hpp"

NAMESPACE {

  struct ShaderVar;
  
  struct VAO {

    u32 length;
    u32* ids;

    void init(u32 num_vaos = 1);
    void use(u32 index  = 0);
    /*the types of the shader vars must entirely be composed of 
      either f64s or f32s; for example Vector3f and Color4d 
      must not be mixed */
    void registerVars(Array<ShaderVar> vars,
		      u32 index = 0);
    ~VAO();
    
  };

  struct VBO {
    
    u32 length;
    u32* ids;
    u32* lengths;

    void init(u32 num_vbos = 1);
    template <typename T>
    void bindArray(Array<T> arr, bool dynamic,
		   u32 index = 0) {
      lengths[index] = arr.size()*sizeof(T);
      //g::message("l: %u a: %u", lengths[index], &arr[0]);
      glBindBuffer(GL_ARRAY_BUFFER, ids[index]);
      glBufferData(GL_ARRAY_BUFFER, lengths[index], &arr[0],
      		   dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }
    //the new array must be the same size as before
    template <typename T>
    void rebindArray(Array<T> arr, bool dynamic,
		   u32 index = 0) {
      glBufferData(GL_ARRAY_BUFFER, lengths[index], &arr[0],
		   dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }
    void draw(GLenum mode, u32 index = 0);
    ~VBO();

    //static VBO getVBO(u32 num_vbos = 1);
  };

  struct EBO {
    
    u32 length;
    u32* ids;
    u32* lengths;

    void init(u32 num_ebos = 1);
    void bindArray(Array<u32> arr, bool dynamic,
		   u32 index = 0);
    //the new array must be the same size as before
    /*void rebindArray(Array<u32> arr, bool dynamic,
      u32 index = 0);*/
    void draw(GLenum mode, u32 index = 0);
    ~EBO();

    //static EBO getEBO(u32 num_ebos = 1);
  };
  
}
