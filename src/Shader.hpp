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
    TYPE_VECTOR2F,
    TYPE_VECTOR3F,
    TYPE_COLOR4F,
    TYPE_BONES_INDEX,
    TYPE_BONE_WEIGHT,
    TYPE_LAST
  };
  
  struct ShaderVar {
    
    GLint id;
    const ShaderTypeInfo* info;

    ShaderVar(GLint var_id, ShaderTypeName var_type);
  };

  struct ShaderUniform {

    GLint id;

    ShaderUniform(GLint uniform_id);
    void registerInt(GLint i) const;
    void registerMat4f(Mat4f mat) const;
  };
  
  struct Shader {
    
    GLuint id;

    Shader(const String filename = "Default");
    void use();
    ShaderVar getVar(const char* name, ShaderTypeName type);
    ShaderUniform getUniform(const char* name);
    ~Shader();

    const static ShaderVar POSITION;
    const static ShaderVar COLOR;
    const static ShaderVar TEX_COORD;
    const static ShaderVar NORMAL;

    static ShaderUniform UNI_TEXTURE;
    static ShaderUniform UNI_MODEL;
    static ShaderUniform UNI_VIEW;
    static ShaderUniform UNI_PROJ;
  };
  
}
