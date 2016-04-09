#pragma once

#include "GL.hpp"
#include "Standard.hpp"
#include "Containers.hpp"

NAMESPACE {

  struct ShaderVar;

  PEACE_DEFINE_BITFLAGS(GLObjectFlags, 8,
			PEACE_GL_NO_FLAGS = 0x0,
			PEACE_GL_UNINITIALIZED = 0x01,
			PEACE_GL_IS_COPY = 0x02);
  
  //macro used in destructor of objects extending GLObject
#define PEACE_GL_SHOULD_DELETE			\
  (!(this->flags & PEACE_GL_UNINITIALIZED ||	\
     this->flags & PEACE_GL_IS_COPY))
  
  struct GLObject {

    u32 id;
    GLObjectFlags flags;

    GLObject();
    GLObject(const GLObject& b);
    GLObject(GLObject&& b);
    virtual ~GLObject();
    GLObject& operator=(const GLObject& b);
    GLObject& operator=(GLObject&& b);

    void init();
    
  };
  
  struct VAO : GLObject {
    
    VAO() {}
    ~VAO();
    
    void init();
    void use();
    /*the types of the shader vars must entirely be composed of 
      either f64s or f32s; for example Vec3f and Vec4d 
      must not be mixed */
    void registerVars(Array<ShaderVar> vars);
    
  };

  struct VBO : GLObject {
    
    u32 length;

    VBO();
    ~VBO();
    
    void init();
    template <typename T>
    void bindArray(Array<T>& arr, bool dynamic) {
      use();
      length = arr.size()*sizeof(T);
      glBufferData(GL_ARRAY_BUFFER, length, arr.begin(),
      		   dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }
    void use();
    void draw(u32 instances = 1,
	      GLenum mode = gl::getDrawMode());

  };

  struct EBO : GLObject {
    
    u32 length;

    EBO();
    ~EBO();

    void init();
    void bindArray(Array<u32>& arr, bool dynamic);
    //the new array must be the same size as before
    /*void rebindArray(Array<u32> arr, bool dynamic,
      u32 index = 0);*/
    void draw(u32 instances = 1,
	      GLenum mode = gl::getDrawMode());

  };
  
}
