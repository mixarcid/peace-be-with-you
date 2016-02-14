#pragma once

#include "Standard.hpp"
#include "Containers.hpp"
#include "String.hpp"
#include "GL.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"

NAMESPACE {

  struct Texture;
  
  struct ShaderTypeInfo {
    
    GLenum elem_type; //GL_FLOAT, etc
    i32 elem_num;
    size_t size; //total size of the object in bytes

    ShaderTypeInfo(GLenum type_elem, i32 num_elem,
		   size_t total_size);
  };

  /* TYPE_VECTOR4U represents a 4-dimensional unsigned int vector that becomes
     a uvec in glsl. TYPE_VECTOR(N)S represents a N-dimensional short vector
     that becomes a regular vec is glsl. Yeah, it's stupid and hacky, but so 
     far it works */
  enum ShaderTypeName {
    TYPE_F32,
    TYPE_U32,
    TYPE_VECTOR2F,
    TYPE_VECTOR3F,
    TYPE_VECTOR4F,
    TYPE_VECTOR4U,
    TYPE_VECTOR2S,
    TYPE_LAST
  };
  
  struct ShaderVar {
    
    i32 id;
    const ShaderTypeInfo* info;

    ShaderVar();
    ShaderVar(i32 var_id, ShaderTypeName var_type);
  };

  struct ShaderUniform {

    u32 id;
    u32 block_id;
    u32 buffer_id;
    //could have different meanings depending on the uniform type
    //if it's a texture, it refers to the GL_TEXTUREn used by the
    //texture
    u32 num;

    ShaderUniform(u32 _num = 0);

    void initBuffer(u32 shader_id, String name);
    void keepBuffer(u32 shader_id, String name);

    void registerInt(i32 i) const;
    void registerTexture(Texture tex) const;
    
    template <typename T>
    void registerVal(T val) {
      glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
      glBufferData(GL_UNIFORM_BUFFER,
		   sizeof(T),
		   &val,
		   GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER,
		       id, buffer_id);
    }

    template <typename T>
    void registerArray(T* data, u32 num_elems) {
      glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
      glBufferData(GL_UNIFORM_BUFFER,
		   num_elems*sizeof(T),
		   data,
		   GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER,
		       id, buffer_id);
    }
    
    template <typename T>
    void registerArray(Array<T> data) {
      registerArray(data.begin(), data.size());
    }

    static u32 largest_id;
  };

  /*
    SHADER_PLAIN is used to define a plain old shader,
    without any of the regular uniforms and variables
    we add in
  */
  PEACE_DEFINE_BITFLAGS(ShaderFlags, 8,
			SHADER_NO_FLAGS = 0x00,
			SHADER_USE_COLOR = 0x01,
			SHADER_SKELETAL = 0x02,
			SHADER_2D = 0x04,
			SHADER_LAST_RENDER_FLAG = 0x05,
			SHADER_UNINITIALIZED = 0x08,
			SHADER_PLAIN = 0x10);
  struct Shader {

    i32 id;
    ShaderFlags flags;
    i32 flag_ids[SHADER_LAST_RENDER_FLAG];
    String vert_str;
    String frag_str;

    Shader(ShaderFlags _flags = SHADER_NO_FLAGS);
    ~Shader();
    void init(const String vert, const String frag);
    void use();
    void _use_no_check();
    void localSetFlags(ShaderFlags shade_flags);
    
    ShaderVar getVar(String name, ShaderTypeName type);
    ShaderUniform getUniform(String name, u32 num=0);
    ShaderUniform getUniformBuffer(String name,
				   u32 num=0);
    void bindOutputs(Array<String> names);

    const static ShaderVar POSITION;
    const static ShaderVar POSITION_2D_SHORT;
    const static ShaderVar COLOR;
    const static ShaderVar TEX_COORD;
    const static ShaderVar NORMAL;
    const static ShaderVar BONE_INDEXES0;
    const static ShaderVar BONE_WEIGHTS0;

    static ShaderUniform UNI_TEXTURE;
    static ShaderUniform UNI_MODEL;
    static ShaderUniform UNI_VIEW_PROJ;
    static ShaderUniform UNI_BONES;
    static ShaderUniform UNI_DIR_LIGHTS;
    static ShaderUniform UNI_AMBIENT;
    static ShaderUniform UNI_COLOR;

    static Shader* cur_shader;

    const static u8 MAX_BONES = 50;
    const static u8 MAX_BONES_PER_VERTEX = 4;
    const static u8 MAX_DIR_LIGHTS = 1;

    const static Vec4f DEFAULT_COLOR;
    
    static void setFlags(ShaderFlags flags);
  };
  
}
