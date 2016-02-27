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

    ShaderUniform(u32 _num = 0, bool is_buffer = true);

    void initBuffer(u32 shader_id, String name);
    void keepBuffer(u32 shader_id, String name);

    void registerInt(i32 i) const;
    void registerTexture(Texture tex) const;
    
    template <typename T>
    void registerVal(T val) {
      PEACE_GL_CHECK_ERROR;
      glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
      glBufferData(GL_UNIFORM_BUFFER,
		   sizeof(T),
		   &val,
		   GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER,
		       id, buffer_id);
      PEACE_GL_CHECK_ERROR;
    }

    template <typename T>
    void registerArray(T* data, u32 num_elems) {
      PEACE_GL_CHECK_ERROR;
      debugAssert(block_id != GL_INVALID_INDEX,
		  "You're trying to send data to a "
		  "buffer that doesn't exist");
      glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
      glBufferData(GL_UNIFORM_BUFFER,
		   num_elems*sizeof(T),
		   data,
		   GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER,
		       id, buffer_id);
      PEACE_GL_CHECK_ERROR;
    }
    
    template <typename T>
    void registerArray(Array<T> data) {
      PEACE_GL_CHECK_ERROR;
      registerArray(data.begin(), data.size());
      PEACE_GL_CHECK_ERROR;
    }

    static u32 largest_id;
  };

  PEACE_DEFINE_BITFLAGS(ShaderFlags, 16,
			SHADER_NO_FLAGS = 0x00,
			SHADER_USE_NORMAL = 0x01,
			SHADER_USE_COLOR = 0x02,
			SHADER_USE_TEXTURE = 0x04,
			SHADER_SKELETAL = 0x08,
		        SHADER_2D = 0x10,
			SHADER_3D = 0x20,
			SHADER_LAST_FLAG = 0x21,
			SHADER_ALL_FLAGS = 0x3f);

  /*
    SHADER_PLAIN is used to define a plain old shader,
    without any of the regular uniforms and variables
    we add in
  */
    
  PEACE_DEFINE_BITFLAGS(ShaderSettings, 16,
			SHADER_NO_SETTINGS = 0x00,
			SHADER_UNINITIALIZED = 0x01,
			SHADER_PLAIN = 0x02);

  struct GlobalShaderVar : ShaderVar {

    String name;
    GlobalShaderVar(String _name,
		    ShaderFlags flags,
		    i32 var_id,
		    ShaderTypeName var_type);
    static Array<GlobalShaderVar*> vars[SHADER_ALL_FLAGS+1];
    
  };

  struct GlobalShaderUniform : ShaderUniform {

    String name;
    GlobalShaderUniform(String _name,
			ShaderFlags flags,
			u32 num = 0,
			bool is_buffer = true);
    static Array<GlobalShaderUniform*> uniforms[SHADER_ALL_FLAGS+1];

  };

  struct Shader {

    i32 id;
    ShaderSettings settings;
    ShaderFlags flags;
    i32 flag_ids[SHADER_ALL_FLAGS+1];
    String vert_str;
    String frag_str;

    Shader(ShaderSettings _settings = SHADER_NO_SETTINGS);
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

    const static GlobalShaderVar POSITION;
    const static GlobalShaderVar POSITION_2D_SHORT;
    const static GlobalShaderVar COLOR;
    const static GlobalShaderVar TEX_COORD;
    const static GlobalShaderVar NORMAL;
    const static GlobalShaderVar BONE_INDEXES0;
    const static GlobalShaderVar BONE_WEIGHTS0;

    static GlobalShaderUniform UNI_TEXTURE;
    static GlobalShaderUniform UNI_MODEL;
    static GlobalShaderUniform UNI_VIEW_PROJ;
    static GlobalShaderUniform UNI_BONES;
    static GlobalShaderUniform UNI_DIR_LIGHTS;
    static GlobalShaderUniform UNI_AMBIENT;
    static GlobalShaderUniform UNI_COLOR;

    static Shader* cur_shader;

    const static u8 MAX_BONES = 50;
    const static u8 MAX_BONES_PER_VERTEX = 4;
    const static u8 MAX_DIR_LIGHTS = 1;

    const static Vec4f DEFAULT_COLOR;
    
    static void setFlags(ShaderFlags flags);
  };
  
}
