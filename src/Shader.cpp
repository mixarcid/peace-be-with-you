#include "Shader.hpp"
#include "Color.hpp"
#include "Assets.hpp"

NAMESPACE {

  const static ShaderTypeInfo SHADER_TYPES[TYPE_LAST] = {
    ShaderTypeInfo(GL_FLOAT, 1, sizeof(f32)),
    ShaderTypeInfo(GL_UNSIGNED_INT, 1, sizeof(u32)),
    ShaderTypeInfo(GL_FLOAT, 2, 2*sizeof(f32)),
    ShaderTypeInfo(GL_FLOAT, 3, 3*sizeof(f32)),
    ShaderTypeInfo(GL_FLOAT, 4, 4*sizeof(f32)),
    ShaderTypeInfo(GL_UNSIGNED_INT, 4, 4*sizeof(u32))
  };
  
  const ShaderVar Shader::POSITION(0, TYPE_VECTOR3F);
  const ShaderVar Shader::COLOR(1, TYPE_VECTOR4F);
  const ShaderVar Shader::TEX_COORD(2, TYPE_VECTOR2F);
  const ShaderVar Shader::NORMAL(3, TYPE_VECTOR3F);
  const ShaderVar Shader::BONE_INDEXES0(5, TYPE_VECTOR4U);
  const ShaderVar Shader::BONE_WEIGHTS0(6, TYPE_VECTOR4F);

  ShaderUniform Shader::UNI_TEXTURE;
  ShaderUniform Shader::UNI_MODEL;
  ShaderUniform Shader::UNI_VIEW_PROJ;
  ShaderUniform Shader::UNI_BONES;
  ShaderUniform Shader::UNI_DIR_LIGHTS;
  ShaderUniform Shader::UNI_AMBIENT;

  Shader* Shader::cur_shader = NULL;

  const u8 Shader::MAX_BONES;
  const u8 Shader::MAX_BONES_PER_VERTEX;
  
  const static char* SHADER_HEADER_VERT = DIR_SHADER_HEADER ".vs";
  const static char* SHADER_HEADER_FRAG = DIR_SHADER_HEADER ".fs";
  const static i32 MAX_CHARS_IN_FILE = 5000;
  const static i32 MAX_LOG_SIZE = 1000;

  static String vert_header;
  static String frag_header;
  
  PEACE_DEFINE_BITFIELD(FirstLoadFLags, 8,
			FIRST_SHADER_LOAD = 0x01,
			FIRST_COLOR_LOAD = 0x02,
			FIRST_TEXTURE_LOAD = 0x04,
			FIRST_SKELETAL_LOAD = 0x05,
			FIRST_LOAD_ALL = 0xff)
  
  FirstLoadFLags first_load_flags = FIRST_LOAD_ALL;

  static String getFileContents(const char* filename) {
    
    char contents[MAX_CHARS_IN_FILE];
    FILE* file = fopen(filename, "r");
    fatalAssert(file != NULL,
	       "Error opening %s", filename);
    i32 line_num = -1;

    while (!feof(file)) {
      fatalAssert(line_num < MAX_CHARS_IN_FILE,
		 "File %s is too long", filename);
      contents[++line_num] = fgetc(file);
    }
    contents[line_num] = '\0';
    fatalAssert(contents != NULL,
	       "Error reading %s", filename);

    fclose(file);

    return String(contents);
  }

  ShaderTypeInfo::ShaderTypeInfo(GLenum type_elem, i32 num_elem,
				 size_t total_size) {
    debugAssert(num_elem < 5,
		"ShaderTypeInfo elem_num must be less then 5");
    elem_type = type_elem;
    elem_num = num_elem;
    size = total_size;
  }

  ShaderVar::ShaderVar(i32 var_id, ShaderTypeName var_type) {
    id = var_id;
    debugAssert(var_type < TYPE_LAST,
		"ShaderVarType index out of bounds");
    info = &SHADER_TYPES[var_type];
  }

  void ShaderUniform::initBuffer(u32 shader_id,
				 String name) {
    const char* c_name = ("_" + name).c_str();
    block_id = glGetUniformBlockIndex(shader_id,
				      c_name);
    //Log::message(name);
    debugAssert(block_id != GL_INVALID_INDEX,
		"The uniform block %s does not "
		"exist in the shader", c_name);
    glUniformBlockBinding(shader_id,
			  block_id,
			  block_id);
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);

    i32 block_size;
    glGetActiveUniformBlockiv(shader_id, block_id,
			      GL_UNIFORM_BLOCK_DATA_SIZE,
			      &block_size);
    Log::message(name);
    Log::message("name: %s, size: %d\n"
		 "block id: %d, buffer id: %u",
		 c_name,
		 block_size,
		 block_id,
		 buffer_id);
  }

  void ShaderUniform::keepBuffer(u32 shader_id) {
    glUniformBlockBinding(shader_id,
			  block_id,
			  block_id);
  }

  void ShaderUniform::registerInt(i32 i) const {
    glUniform1i(id, i);
  }

  /*void ShaderUniform::registerMat4f(Mat4f mat) const {
    glUniformMatrix4fv(id, 1, GL_FALSE, mat.data);
    }*/

  Shader::Shader(const String filename)
    : flags(SHADER_UNINITIALIZED) {

    for (u8 i=0; i<SHADER_FLAG_LAST; ++i) {
      flag_ids[i] = -1;
    }

    //printf("%u\n", FIRST_SHADER_LOAD | first_load_flags);

    if (first_load_flags & FIRST_SHADER_LOAD) {
      vert_header = getFileContents(SHADER_HEADER_VERT);
      frag_header = getFileContents(SHADER_HEADER_FRAG);
    }

    vert_str = vert_header + "\n"
      + getFileContents((DIR_SHADERS + filename
			 + DIR_VERT_EXTENSION).c_str());
    frag_str = frag_header + "\n"
      + getFileContents((DIR_SHADERS + filename +
			 DIR_FRAG_EXTENSION).c_str());

    localSetFlags(SHADER_NO_FLAGS);
    Log::message("Loaded shader %s", filename.c_str());
  }

  void Shader::localSetFlags(ShaderFlags shade_flags) {

    /*Log::message("0x%x, 0x%x, 0x%x, 0x%x",
		 shade_flags,
		 flags,
		 SHADER_SKELETAL,
		 shade_flags & SHADER_SKELETAL);*/
    
    if (flags == shade_flags) {
      return;
    }

    debugAssert(flags < SHADER_FLAG_LAST,
		"Shader::setFlags called with invalid flags");
    i32 flag_id = flag_ids[shade_flags];
    if (flag_id != -1) {
      id = flag_id;
      flags = shade_flags;
      if (cur_shader == this) {
	__use_no_check();
      }
      return;
    }
    //Log::message("!");

    String preprocess = "#version 150 core\n";
    if (shade_flags & SHADER_USE_COLOR) {
      preprocess += "#define SHADER_USE_COLOR\n";
    }
    if (shade_flags & SHADER_SKELETAL) {
      preprocess += "#define SHADER_SKELETAL\n";
    }
    
    String vert_source = preprocess + vert_str;
    String frag_source = preprocess + frag_str;
    const char* c_vert_source = vert_source.c_str();
    const char* c_frag_source = frag_source.c_str();
    //Log::message(c_vert_source);

    u32 vert_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_id, 1, &c_vert_source, NULL);
    //printf("%p", &glCompileShader);
    glCompileShader(vert_id);

    u32 frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_id, 1, &c_frag_source, NULL);
    glCompileShader(frag_id);

    char c_shader_log[MAX_LOG_SIZE];
    glGetShaderInfoLog(vert_id, MAX_LOG_SIZE, NULL, c_shader_log);
    String shader_log = c_shader_log;
    fatalAssert(shader_log.length() == 0,
		"%s", c_shader_log);
    glGetShaderInfoLog(frag_id, MAX_LOG_SIZE, NULL, c_shader_log);
    shader_log = c_shader_log;
    fatalAssert(shader_log.length() == 0,
	       "%s", c_shader_log);

    id = glCreateProgram();
    glAttachShader(id, vert_id);
    glAttachShader(id, frag_id);

    glBindFragDataLocation(id, 0, "outColor");
    
    glBindAttribLocation(id, POSITION.id, "inPosition");
    glBindAttribLocation(id, NORMAL.id, "inNormal");
    
    if (shade_flags & SHADER_USE_COLOR) {
      glBindAttribLocation(id, COLOR.id, "inColor");
    } else {
      glBindAttribLocation(id, TEX_COORD.id, "inTexCoord");
    }
    if (shade_flags & SHADER_SKELETAL) {
      glBindAttribLocation(id, BONE_INDEXES0.id, "inBoneIndexes0");
      glBindAttribLocation(id, BONE_WEIGHTS0.id, "inBoneWeights0");
    }

    glLinkProgram(id);
    glDeleteShader(vert_id);
    glDeleteShader(frag_id);

    flags = shade_flags;
    flag_ids[flags] = id;
    if (cur_shader == this) {
      __use_no_check();
    }
  }

  void Shader::__use_no_check() {
    glUseProgram(id);
    if (!(flags & SHADER_USE_COLOR)) {
      UNI_TEXTURE.id =
	glGetUniformLocation(id, "uniTexture");
    }
    if (first_load_flags & FIRST_SHADER_LOAD) {
      UNI_MODEL.initBuffer(id, "uniModel");
      UNI_VIEW_PROJ.initBuffer(id, "uniViewProj");
      UNI_BONES.initBuffer(id, "uniBones");
      UNI_DIR_LIGHTS.initBuffer(id, "uniDirLights");
      UNI_AMBIENT.initBuffer(id, "uniAmbient");
      first_load_flags &= ~FIRST_SHADER_LOAD;
    } else {
      UNI_MODEL.keepBuffer(id);
      UNI_VIEW_PROJ.keepBuffer(id);
      UNI_BONES.keepBuffer(id);
      UNI_DIR_LIGHTS.keepBuffer(id);
      UNI_AMBIENT.keepBuffer(id);
    }
    cur_shader = this;
  }

  void Shader::use() {
    if (cur_shader != this) {
      __use_no_check();
    }
  }

  /*ShaderVar Shader::getVar(const char* name,
			   ShaderTypeName type) {
    i32 var_id = glGetAttribLocation(id, name);
    return ShaderVar(var_id, type);
    }*/

  /*ShaderUniform Shader::getUniform(const char* name) {
    ShaderUniform ret;
    ret.initBuffer(id, name);
    return ret;
    }*/

  Shader::~Shader() {
    glDeleteProgram(id);
  }

  void Shader::setFlags(ShaderFlags flags) {
    debugAssert(cur_shader != NULL,
		"You must use a shader before setting flags");
    cur_shader->localSetFlags(flags);
  }
}
