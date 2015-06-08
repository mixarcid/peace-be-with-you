#pragma once

#include "Standard.h"
#include "String.h"
#include "Graphics.h"
#include "VectorMath.h"

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

    Shader(const String filename);
    void use();
    ShaderVar getVar(const char* name, ShaderTypeName type);
    ShaderUniform getUniform(const char* name);
    ~Shader();

    const static ShaderVar POSITION;
    const static ShaderVar COLOR;
    const static ShaderVar TEX_COORD;
    const static ShaderVar NORMAL;

    const static ShaderUniform UNI_TEXTURE;
    const static ShaderUniform UNI_MODEL;
    const static ShaderUniform UNI_VIEW;
    const static ShaderUniform UNI_PROJ;
  };
  
}
