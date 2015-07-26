#pragma once

#include "Standard.hpp"
#include "String.hpp"
#include "GL.hpp"
#include "VectorMath.hpp"

NAMESPACE {
  
  struct ShaderTypeInfo {
    
    GLenum elem_type; //GL_FLOAT, etc
    GLint elem_num;
    size_t size; //total size of the object in bytes

    ShaderTypeInfo(GLenum type_elem, GLint num_elem,
		   size_t total_size);
  };

  enum ShaderTypeName {
    TYPE_F32,
    TYPE_U32,
    TYPE_VECTOR2F,
    TYPE_VECTOR3F,
    TYPE_VECTOR4F,
    TYPE_VECTOR4U,
    TYPE_LAST
  };
  
  struct ShaderVar {
    
    GLint id;
    const ShaderTypeInfo* info;

    ShaderVar(GLint var_id, ShaderTypeName var_type);
  };

  struct ShaderUniform {

    union {
      i32 id;
      struct {  //if it's a uniform buffer object
	u32 block_id;
	u32 buffer_id;
      };
    };

    ShaderUniform(GLint uniform_id);
    void registerInt(GLint i) const;
    void registerMat4f(Mat4f mat) const;
    template <typename T>
    void registerBufferData(Array<T> data) {
      glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
      glBufferData(GL_UNIFORM_BUFFER,
		   data.size()*sizeof(T),
		   &data[0], GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER, block_id, buffer_id);
    }
  };
  
  struct Shader {
    
    u32 id;

    Shader(const String filename = "Default");
    void use();
    ShaderVar getVar(const char* name, ShaderTypeName type);
    ShaderUniform getUniform(const char* name);
    ~Shader();

    const static ShaderVar POSITION;
    const static ShaderVar COLOR;
    const static ShaderVar TEX_COORD;
    const static ShaderVar NORMAL;
    const static ShaderVar BONE_INDEXES0;
    const static ShaderVar BONE_WEIGHTS0;

    static ShaderUniform UNI_TEXTURE;
    static ShaderUniform UNI_MODEL;
    static ShaderUniform UNI_VIEW;
    static ShaderUniform UNI_PROJ;
    static ShaderUniform UNI_BONES;
    static ShaderUniform UNI_FLAGS;

    const static u8 MAX_BONES = 50;
    const static u8 MAX_BONES_PER_VERTEX = 4;
  };
  
}
