#include "Shader.hpp"
#include "Assets.hpp"
#include "Texture.hpp"

NAMESPACE {

  const static ShaderTypeInfo SHADER_TYPES[TYPE_LAST] = {
    ShaderTypeInfo(GL_FLOAT, 1, sizeof(f32)),
    ShaderTypeInfo(GL_UNSIGNED_INT, 1, sizeof(u32)),
    ShaderTypeInfo(GL_FLOAT, 2, 2*sizeof(f32)),
    ShaderTypeInfo(GL_FLOAT, 3, 3*sizeof(f32)),
    ShaderTypeInfo(GL_FLOAT, 4, 4*sizeof(f32)),
    ShaderTypeInfo(GL_UNSIGNED_INT, 4, 4*sizeof(u32)),
    ShaderTypeInfo(GL_SHORT, 2, 2*sizeof(i16))
  };

  Array<GlobalShaderVar*> GlobalShaderVar::vars[SHADER_ALL_FLAGS+1];
  Array<GlobalShaderUniform*> GlobalShaderUniform::uniforms[SHADER_ALL_FLAGS+1];
  
  const GlobalShaderVar Shader::POSITION
    ("inPosition", SHADER_3D, 0, TYPE_VECTOR3F);
  const GlobalShaderVar Shader::COLOR
    ("inColor", SHADER_USE_COLOR, 1, TYPE_VECTOR4F);
  const GlobalShaderVar Shader::TEX_COORD
    ("inTexCoord", SHADER_USE_TEXTURE, 2, TYPE_VECTOR2F);
  const GlobalShaderVar Shader::NORMAL
    ("inNormal", SHADER_USE_NORMAL, 3, TYPE_VECTOR3F);
  const GlobalShaderVar Shader::BONE_INDEXES0
    ("inBoneIndexes0", SHADER_SKELETAL, 4, TYPE_VECTOR4U);
  const GlobalShaderVar Shader::BONE_WEIGHTS0
    ("inBoneWeights0", SHADER_SKELETAL, 5, TYPE_VECTOR4F);
  const GlobalShaderVar Shader::POSITION_2D_SHORT
    ("inPosition2d", SHADER_2D, 6, TYPE_VECTOR2S);

  GlobalShaderUniform Shader::UNI_TEXTURE
    ("uniTexture", SHADER_USE_TEXTURE, 0, false);
  GlobalShaderUniform Shader::UNI_MODEL
    ("uniModel", ~SHADER_NO_FLAGS);
  GlobalShaderUniform Shader::UNI_VIEW_PROJ
    ("uniViewProj", ~SHADER_NO_FLAGS);
  GlobalShaderUniform Shader::UNI_BONES
    ("uniBones", SHADER_SKELETAL);
  GlobalShaderUniform Shader::UNI_DIR_LIGHTS
    ("uniDirLights", SHADER_3D);
  GlobalShaderUniform Shader::UNI_AMBIENT
    ("uniAmbient", SHADER_3D);
  GlobalShaderUniform Shader::UNI_COLOR
    ("uniColor", SHADER_2D);

  Shader* Shader::cur_shader = NULL;

  const u8 Shader::MAX_BONES;
  const u8 Shader::MAX_BONES_PER_VERTEX;

  const Vec4f Shader::DEFAULT_COLOR(1,1,1,1);
  
  const static char* SHADER_HEADER_VERT = DIR_SHADER_HEADER ".vs";
  const static char* SHADER_HEADER_FRAG = DIR_SHADER_HEADER ".fs";
  const static i32 MAX_LOG_SIZE = 1000;

  static String vert_header;
  static String frag_header;

  static bool first_shader_load = true;

  static String getFileContents(String filename) {

    static HashMap<String, String> all_contents;

    auto it = all_contents.find(filename);
    if (it != all_contents.end()) {
      return it->second;
    }

    FILE* file = fopen(filename.c_str(), "r");
    fatalAssert(file != NULL,
		"Error opening %s", filename.c_str());
    
    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    
    String ret;
    ret.resize(file_size);
    fread(&ret[0], 1, ret.size(), file);
    
    fclose(file);
    all_contents[filename] = ret;
    
    return ret;
  }

  ShaderTypeInfo::ShaderTypeInfo(GLenum type_elem, i32 num_elem,
				 size_t total_size) {
    debugAssert(num_elem < 5,
		"ShaderTypeInfo elem_num must be less then 5");
    elem_type = type_elem;
    elem_num = num_elem;
    size = total_size;
  }

  ShaderVar::ShaderVar() {}

  ShaderVar::ShaderVar(i32 var_id, ShaderTypeName var_type) {
    id = var_id;
    debugAssert(var_type < TYPE_LAST,
		"ShaderVarType index out of bounds");
    info = &SHADER_TYPES[var_type];
  }

  u32 ShaderUniform::largest_id = 0;
  ShaderUniform::ShaderUniform(u32 _num, bool is_buffer)
    : id(largest_id++),
    block_id(is_buffer ? 0 : -1),
    buffer_id(is_buffer ? 0 : -1),
    num(_num) {};

  void ShaderUniform::initBuffer(u32 shader_id,
				 String name) {
    PEACE_GL_CHECK_ERROR;
    String comb_name = "_" + name;
    block_id = glGetUniformBlockIndex(shader_id,
				      comb_name.c_str());
    /*fatalAssert(block_id != GL_INVALID_INDEX,
		"The uniform block %s does not "
		"exist in the shader", comb_name.c_str());*/
    if (block_id != GL_INVALID_INDEX) {
      glUniformBlockBinding(shader_id,
			    block_id,
			    id);
      glGenBuffers(1, &buffer_id);
      glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
    }
  }

  void ShaderUniform::keepBuffer(u32 shader_id,
				 String name) {
    initBuffer(shader_id, name);
  }

  void ShaderUniform::registerInt(i32 i) const {
    glUniform1i(id, i);
  }

  void ShaderUniform::registerTexture(Texture tex) const {
    registerInt(num);
    GLenum tex_index = GL_TEXTURE0 + num;
    debugAssert(tex_index < GL_MAX_TEXTURE_UNITS,
		"The requested texture index is too large.");
    glActiveTexture(tex_index);
    tex.use();
  }

  GlobalShaderVar::GlobalShaderVar(String _name,
				   ShaderFlags flags,
				   i32 var_id,
				   ShaderTypeName var_type)
    : ShaderVar(var_id, var_type), name(_name) {
    for (u8 mask=SHADER_NO_FLAGS+1; mask<SHADER_ALL_FLAGS+1; ++mask) {
      if (mask & flags) {
	vars[mask].push_back(this);
      }
    }
  }

  GlobalShaderUniform::GlobalShaderUniform(String _name,
					   ShaderFlags flags,
					   u32 num,
					   bool is_buffer)
    : ShaderUniform(num, is_buffer), name(_name) {
    for (u8 mask=SHADER_NO_FLAGS+1; mask<SHADER_ALL_FLAGS+1; ++mask) {
      if (mask & flags) {
	uniforms[mask].push_back(this);
      }
    }
  }

  Shader::Shader(ShaderSettings _settings)
    : settings(_settings | SHADER_UNINITIALIZED), flags(SHADER_NO_FLAGS) {}

  Shader::~Shader() {
    if (!(settings & SHADER_UNINITIALIZED)) {
      glDeleteProgram(id);
    }
  }

  void Shader::init(const String vert, const String frag) {
    
    for (u8 i=0; i<SHADER_ALL_FLAGS+1; ++i) {
      flag_ids[i] = -1;
    }

    if (!(settings & SHADER_PLAIN) &&
	first_shader_load) {
      first_shader_load = false;
      vert_header = getFileContents(SHADER_HEADER_VERT);
      frag_header = getFileContents(SHADER_HEADER_FRAG);
    }

    vert_str = vert_header + "\n"
      + getFileContents((DIR_SHADERS + vert
			 + DIR_VERT_EXTENSION));
    frag_str = frag_header + "\n"
      + getFileContents((DIR_SHADERS + frag +
			 DIR_FRAG_EXTENSION));

    if (settings & SHADER_PLAIN) {
      localSetFlags(SHADER_NO_FLAGS);
    } else {
      localSetFlags(SHADER_ALL_FLAGS);
    }
    
#ifdef PEACE_LOG_LOADED_ASSETS
    Log::message("Loaded vertex shader %s with fragment shader %s",
		 vert.c_str(), frag.c_str());
#endif
  }

  void Shader::localSetFlags(ShaderFlags shade_flags) {

    debugAssert(!(settings & SHADER_PLAIN)
		|| (shade_flags == SHADER_NO_FLAGS),
		"You cannot call Shader::localSetFlags "
		"on a plain Shader (unless with Shader::NO_FLAGS)");
    
    if ((flags == shade_flags) &&
	!((settings & SHADER_UNINITIALIZED))) {
      return;
    }
    
    i32 flag_id = flag_ids[shade_flags];
    if (flag_id != -1) {
      id = flag_id;
      flags = shade_flags;
      if (cur_shader == this) {
	PEACE_GL_CHECK_ERROR;
	_use_no_check();
      }
      return;
    }

    String preprocess = "#version 150 core\n";

    if (shade_flags == SHADER_NO_FLAGS) {
      preprocess += "#define SHADER_NO_FLAGS\n";
    }
    
    FOR(entry in getEnumEntries("ShaderFlags")[:-2]);
    if (shade_flags & $(entry)) {
      preprocess += "#define $(entry)\n";
    }
    END_FOR;
    
    String vert_source = preprocess + vert_str;
    String frag_source = preprocess + frag_str;
    const char* c_vert_source = vert_source.c_str();
    const char* c_frag_source = frag_source.c_str();

    u32 vert_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_id, 1, &c_vert_source, NULL);
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

    if (!(settings & SHADER_PLAIN)) {
      for (GlobalShaderVar* var : GlobalShaderVar::vars[shade_flags]) {
	glBindAttribLocation(id, var->id, var->name.c_str());
      }
    }

    glLinkProgram(id);
    glDeleteShader(vert_id);
    glDeleteShader(frag_id);

    if (!(settings & SHADER_PLAIN)) {
      flags = shade_flags;
      flag_ids[flags] = id;
    }
    
    settings &= ~SHADER_UNINITIALIZED;
    
    //if (cur_shader == this) {
      _use_no_check();
      //}
    
  }

  void Shader::_use_no_check() {
    debugAssert(!(settings & SHADER_UNINITIALIZED),
		"You must initialize the shader "
		"before using it");
    glUseProgram(id);
    cur_shader = this;
    if (!(settings & SHADER_PLAIN)) {
      for (GlobalShaderUniform* uniform : GlobalShaderUniform::uniforms[flags]) {
	if (uniform->buffer_id != -1) {
	  uniform->initBuffer(id, uniform->name);
	} else {
	  uniform->id = glGetUniformLocation(id, uniform->name.c_str());
	}
      }
    }
  }

  void Shader::use() {
    if (cur_shader != this) {
      _use_no_check();
    }
  }

  ShaderVar Shader::getVar(String name,
			   ShaderTypeName type) {
    i32 var_id = glGetAttribLocation(id, name.c_str());
    debugAssert(var_id != -1,
		"Could not find the ShaderVar %s in the Shader",
		name.c_str());
    return ShaderVar(var_id, type);
  }

  ShaderUniform Shader::getUniform(String name,
				   u32 num) {
    ShaderUniform ret(num);
    ret.id = glGetUniformLocation(id, name.c_str());
    return ret;
  }

  ShaderUniform Shader::getUniformBuffer(String name,
					 u32 num) {
    ShaderUniform ret(num);
    ret.initBuffer(id, name.c_str());
    return ret;
  }

  void Shader::bindOutputs(Array<String> names) {
    debugAssert(!(settings & SHADER_UNINITIALIZED),
		"You must initialize the shader before binding outputs");
    u32 index = 0;
    for (String name : names) {
      glBindFragDataLocation(id, index++, name.c_str());
    }
  }

  void Shader::setFlags(ShaderFlags flags) {
    debugAssert(cur_shader != NULL,
		"You must use a shader before setting flags");
    cur_shader->localSetFlags(flags);
  }
}
