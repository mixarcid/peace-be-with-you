#pragma once

#include "Standard.hpp"
#include "Containers.hpp"
#include "String.hpp"
#include "GL.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"

NAMESPACE {
  
  struct ShaderTypeInfo {
    
    GLenum elem_type; //GL_FLOAT, etc
    i32 elem_num;
    size_t size; //total size of the object in bytes

    ShaderTypeInfo(GLenum type_elem, i32 num_elem,
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
    
    i32 id;
    const ShaderTypeInfo* info;

    ShaderVar(i32 var_id, ShaderTypeName var_type);
  };

  struct ShaderUniform {

    union {
      i32 id;
      struct {  //if it's a uniform buffer object
	u32 block_id;
	u32 buffer_id;
      };
    };

    void initBuffer(u32 shader_id, String name);
    void keepBuffer(u32 shader_id);
    
    void registerInt(i32 i) const;
    //void registerMat4f(Mat4f mat) const;
    
    template <typename T>
    void registerVal(T val) {
      glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
      glBufferData(GL_UNIFORM_BUFFER,
		   sizeof(T),
		   &val,
		   GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER,
		       block_id, buffer_id);
    }
    
    template <typename T>
    void registerArray(Array<T> data) {
      glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
      glBufferData(GL_UNIFORM_BUFFER,
		   data.size()*sizeof(T),
		   &data[0],
		   GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER,
			block_id, buffer_id);
    }

    template <typename T>
    void registerArray(T* data, u32 num_elems) {
      glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
      glBufferData(GL_UNIFORM_BUFFER,
		   num_elems*sizeof(T),
		   data,
		   GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER,
			block_id, buffer_id);
    }

    /*static void initBuffer(u32 shader_id,
			   String block_name,
			   InitList<ShaderUniform*>
			   uniforms);*/
      
  };

  PEACE_DEFINE_BITFIELD(ShaderFlags, 8,
			SHADER_NO_FLAGS = 0x00,
			SHADER_UNINITIALIZED = 0x01,
			SHADER_USE_COLOR = 0x02,
			SHADER_SKELETAL = 0x04,
			SHADER_2D = 0x08,
			SHADER_FLAG_LAST = 0x09)
  
  struct Shader {

    i32 id;
    ShaderFlags flags;
    i32 flag_ids[SHADER_FLAG_LAST];
    String vert_str;
    String frag_str;

    Shader(const String filename = "Default");
    void use();
    void __use_no_check();
    void localSetFlags(ShaderFlags shade_flags);
    //ShaderVar getVar(const char* name, ShaderTypeName type);
    //ShaderUniform getUniform(const char* name);
    ~Shader();

    const static ShaderVar POSITION;
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

    static Shader* cur_shader;

    const static u8 MAX_BONES = 50;
    const static u8 MAX_BONES_PER_VERTEX = 4;
    const static u8 MAX_DIR_LIGHTS = 1;
    
    static void setFlags(ShaderFlags flags);
  };
  
}
