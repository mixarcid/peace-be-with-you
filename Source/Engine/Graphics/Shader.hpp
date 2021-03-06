#pragma once

#include "Standard.hpp"
#include "Containers.hpp"
#include "String.hpp"
#include "GL.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"

NAMESPACE {

  struct Texture;

   /*
    SHADER_TYPE_CONVERT_TO_INT and SHADER_TYPE_NORMALIZED are bitflags that can
    be used with any integral type that specify how the application should send
    the data to the GPU: KEEP_AS_INT ensures that the type is not converted to a
    floating point value, while NORMALIZED specifies that, if it is converted, it
    will be normalized to a value between -1 and 1
   */
  
  struct ShaderTypeInfo {
    
    GLenum elem_type; //GL_FLOAT, etc
    i32 elem_num;
    size_t size; //total size of the object in bytes

    ShaderTypeInfo(GLenum type_elem,
		   i32 num_elem,
		   size_t total_size);
  };
  
  PEACE_ENUM(ShaderTypeName, 8,
	     SHADER_TYPE_F32,
	     SHADER_TYPE_U32,
	     SHADER_TYPE_VECTOR2F,
	     SHADER_TYPE_VECTOR3F,
	     SHADER_TYPE_VECTOR4F,
	     SHADER_TYPE_VECTOR2I,
	     SHADER_TYPE_VECTOR3I,
	     SHADER_TYPE_VECTOR4I,
	     SHADER_TYPE_VECTOR2U,
	     SHADER_TYPE_VECTOR3U,
	     SHADER_TYPE_VECTOR4U,
	     SHADER_TYPE_VECTOR2S,
	     SHADER_TYPE_VECTOR3S,
	     SHADER_TYPE_VECTOR4S,
	     SHADER_TYPE_VECTOR2US,
	     SHADER_TYPE_VECTOR3US,
	     SHADER_TYPE_VECTOR4US,
	     SHADER_TYPE_VECTOR2B,
	     SHADER_TYPE_VECTOR3B,
	     SHADER_TYPE_VECTOR4B,
	     SHADER_TYPE_VECTOR2UB,
	     SHADER_TYPE_VECTOR3UB,
	     SHADER_TYPE_VECTOR4UB,
	     SHADER_TYPE_LAST);

  PEACE_DEFINE_BITFLAGS(ShaderVarFlags, 8,
			SHADER_VAR_NO_FLAGS = 0x00,
			SHADER_VAR_KEEP_INT = 0x01,
			SHADER_VAR_NORMALIZE = 0x02);
  
  struct ShaderVar {

    const ShaderTypeInfo* info;
    i32 id;
    ShaderVarFlags flags;

    ShaderVar();
    ShaderVar(i32 var_id,
	      ShaderTypeName var_type,
	      ShaderVarFlags _flags =
	      SHADER_VAR_NO_FLAGS);
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

    void initBuffer(u32 shader_id, String name, u32 size);
    void keepBuffer(u32 shader_id, String name, u32 size);

    void registerInt(i32 i) const;
    void registerTexture(Texture tex) const;
    
    template <typename T>
    void registerVal(T val) {
      PEACE_GL_CHECK_ERROR;
      glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
      glBufferSubData(GL_UNIFORM_BUFFER,
		      0,
		      sizeof(T),
		      &val);
		      //GL_DYNAMIC_DRAW);
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
      glBufferSubData(GL_UNIFORM_BUFFER,
		      0,
		      num_elems*sizeof(T),
		      data);
		      //GL_DYNAMIC_DRAW);
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
			SHADER_TERRAIN = 0x40,
			SHADER_BILLBOARD = 0x80,
			SHADER_WATER = 0x100,
			SHADER_LAST_FLAG = 0x101,
			SHADER_ALL_FLAGS = 0x1ff);

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
		    ShaderTypeName var_type,
		    ShaderVarFlags var_flags =
		    SHADER_VAR_NO_FLAGS);
    static Array<GlobalShaderVar*> vars[SHADER_ALL_FLAGS+1];
    
  };

  PEACE_DEFINE_BITFLAGS(GlobalUniformFlags, 8,
			UNIFORM_NO_FLAGS = 0x00,
			UNIFORM_UNINITIALIZED = 0x01);

  struct GlobalShaderUniform : ShaderUniform {

    String name;
    u32 size;
    GlobalUniformFlags flags;
    
    GlobalShaderUniform(String _name,
			ShaderFlags flags,
			u32 _size,
			u32 num = 0,
			bool is_buffer = true);
    void initOrKeep(u32 shader_id);
    static Array<GlobalShaderUniform*> uniforms[SHADER_ALL_FLAGS+1];

  };

  struct Shader {

    u32 id;
    u32 cur_tex_id;
    ShaderSettings settings;
    ShaderFlags flags;
    i32 flag_ids[SHADER_ALL_FLAGS+1];
    String vert_full_name;
    String frag_full_name;
    String vert_str;
    String frag_str;

    Shader(ShaderSettings _settings = SHADER_NO_SETTINGS);
    ~Shader();
    void init(const String vert, const String frag);
    void use();
    void _use_no_check();
    void localSetFlags(ShaderFlags shade_flags);
    
    u32 newTextureId();
    
    ShaderVar getVar(String name, ShaderTypeName type);
    ShaderUniform getUniform(String name, u32 num=0);
    
    inline ShaderUniform getTexUniform(String name) {
      return getUniform(name, newTextureId());
    }
    
    ShaderUniform getUniformBuffer(String name,
				   u32 size,
				   u32 num=0);
    void bindOutputs(Array<String> names);

    const static GlobalShaderVar POSITION;
    const static GlobalShaderVar POSITION_2D_SHORT;
    const static GlobalShaderVar COLOR;
    const static GlobalShaderVar TEX_COORD;
    const static GlobalShaderVar NORMAL;
    const static GlobalShaderVar BONE_INDEXES0;
    const static GlobalShaderVar BONE_WEIGHTS0;
    const static GlobalShaderVar HEIGHT;
    const static GlobalShaderVar POSITION_TERRAIN;
    const static GlobalShaderVar BIOME_DATA;

    static GlobalShaderUniform UNI_TEXTURE;
    static GlobalShaderUniform UNI_MODEL;
    static GlobalShaderUniform UNI_VIEW_PROJ;
    static GlobalShaderUniform UNI_BONES;
    static GlobalShaderUniform UNI_DIR_LIGHTS;
    static GlobalShaderUniform UNI_AMBIENT;
    static GlobalShaderUniform UNI_COLOR;
    static GlobalShaderUniform UNI_BILLBOARD_CENTER;
    static GlobalShaderUniform UNI_PAINT;

    static Shader* cur_shader;

    const static u8 MAX_BONES = 50;
    const static u8 MAX_BONES_PER_VERTEX = 4;
    const static u8 MAX_DIR_LIGHTS = 1;
    const static u32 MAX_MODEL_MATS = 1000;

    const static Vec4f DEFAULT_COLOR;
    
    static void setFlags(ShaderFlags flags);
  };
  
}
