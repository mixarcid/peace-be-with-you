#include "Shader.hpp"
#include "Assets.hpp"

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
  
  const ShaderVar Shader::POSITION(0, TYPE_VECTOR3F);
  const ShaderVar Shader::COLOR(1, TYPE_VECTOR4F);
  const ShaderVar Shader::TEX_COORD(2, TYPE_VECTOR2F);
  const ShaderVar Shader::NORMAL(3, TYPE_VECTOR3F);
  const ShaderVar Shader::BONE_INDEXES0(4, TYPE_VECTOR4U);
  const ShaderVar Shader::BONE_WEIGHTS0(5, TYPE_VECTOR4F);
  const ShaderVar Shader::POSITION_2D_SHORT(6, TYPE_VECTOR2S);

  ShaderUniform Shader::UNI_TEXTURE(0);
  ShaderUniform Shader::UNI_MODEL(1);
  ShaderUniform Shader::UNI_VIEW_PROJ(2);
  ShaderUniform Shader::UNI_BONES(3);
  ShaderUniform Shader::UNI_DIR_LIGHTS(4);
  ShaderUniform Shader::UNI_AMBIENT(5);
  ShaderUniform Shader::UNI_COLOR(6);

  Shader* Shader::cur_shader = NULL;

  const u8 Shader::MAX_BONES;
  const u8 Shader::MAX_BONES_PER_VERTEX;

  const Vec4f Shader::DEFAULT_COLOR(1,1,1,1);
  
  const static char* SHADER_HEADER_VERT = DIR_SHADER_HEADER ".vs";
  const static char* SHADER_HEADER_FRAG = DIR_SHADER_HEADER ".fs";
  const static i32 MAX_CHARS_IN_FILE = 5000;
  const static i32 MAX_LOG_SIZE = 1000;

  static String vert_header;
  static String frag_header;
  
  PEACE_DEFINE_BITFLAGS(FirstLoadFLags, 8,
			FIRST_SHADER_LOAD = 0x01,
			FIRST_COLOR_LOAD = 0x02,
			FIRST_TEXTURE_LOAD = 0x04,
			FIRST_SKELETAL_LOAD = 0x08,
			FIRST_3D_LOAD = 0x10,
			FIRST_2D_LOAD = 0x20,
			FIRST_LOAD_ALL = 0xff)
  
  FirstLoadFLags first_load_flags = FIRST_LOAD_ALL;

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

    char* contents = new char[file_size];
    i32 line_num = -1;

    while (!feof(file)) {
      fatalAssert(line_num < MAX_CHARS_IN_FILE,
		  "File %s is too long", filename.c_str());
      contents[++line_num] = fgetc(file);
    }
    contents[line_num] = '\0';
    fatalAssert(contents != NULL,
		"Error reading %s", filename.c_str());

    fclose(file);

    String ret(contents);
    delete[] contents;
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

  ShaderVar::ShaderVar(i32 var_id, ShaderTypeName var_type) {
    id = var_id;
    debugAssert(var_type < TYPE_LAST,
		"ShaderVarType index out of bounds");
    info = &SHADER_TYPES[var_type];
  }

  ShaderUniform::ShaderUniform(u32 _id)
    : id(_id) {};

  void ShaderUniform::initBuffer(u32 shader_id,
				 String name) {
    String comb_name = "_" + name;
    block_id = glGetUniformBlockIndex(shader_id,
				      comb_name.c_str());
    fatalAssert(block_id != GL_INVALID_INDEX,
		"The uniform block %s does not "
		"exist in the shader", comb_name.c_str());
    glUniformBlockBinding(shader_id,
			  block_id,
			  id);
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
  }

  void ShaderUniform::keepBuffer(u32 shader_id,
				 String name) {
    initBuffer(shader_id, name);
  }

  void ShaderUniform::registerInt(i32 i) const {
    glUniform1i(id, i);
  }

  Shader::Shader()
    : flags(SHADER_UNINITIALIZED) {}

  void Shader::init(const String vert, const String frag) {
    
    for (u8 i=0; i<SHADER_FLAG_LAST; ++i) {
      flag_ids[i] = -1;
    }

    if (first_load_flags & FIRST_SHADER_LOAD) {
      vert_header = getFileContents(SHADER_HEADER_VERT);
      frag_header = getFileContents(SHADER_HEADER_FRAG);
    }

    vert_str = vert_header + "\n"
      + getFileContents((DIR_SHADERS + vert
			 + DIR_VERT_EXTENSION));
    frag_str = frag_header + "\n"
      + getFileContents((DIR_SHADERS + frag +
			 DIR_FRAG_EXTENSION));

    localSetFlags(SHADER_SKELETAL);
#ifdef PEACE_LOG_LOADED_ASSETS
    Log::message("Loaded vertex shader %s with fragment shader %s",
		 vert.c_str(), frag.c_str());
#endif
  }

  void Shader::localSetFlags(ShaderFlags shade_flags) {
    
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
	PEACE_GL_CHECK_ERROR;
	_use_no_check();
      }
      return;
    }

    String preprocess = "#version 150 core\n";
    if (shade_flags & SHADER_USE_COLOR) {
      preprocess += "#define SHADER_USE_COLOR\n";
    }
    if (shade_flags & SHADER_SKELETAL) {
      preprocess += "#define SHADER_SKELETAL\n";
    }
    if (shade_flags & SHADER_2D) {
      preprocess += "#define SHADER_2D\n";
    }
    
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

    if (shade_flags & SHADER_2D) {
      glBindAttribLocation(id, POSITION_2D_SHORT.id, "inPosition2d");
    } else {
      glBindAttribLocation(id, POSITION.id, "inPosition");
      glBindAttribLocation(id, NORMAL.id, "inNormal");
    }
    
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

    glBindFragDataLocation(id, 0, "outColor");

    flags = shade_flags;
    flag_ids[flags] = id;
    PEACE_GL_CHECK_ERROR;
    if (cur_shader == this) {
      _use_no_check();
    }
    PEACE_GL_CHECK_ERROR;
  }

  void Shader::_use_no_check() {
    debugAssert(!(flags & SHADER_UNINITIALIZED),
		"You must initialize the shader "
		"before using it");
    glUseProgram(id);
    cur_shader = this;
    if (!(flags & SHADER_USE_COLOR)) {
      UNI_TEXTURE.id =
	glGetUniformLocation(id, "uniTexture");
    }
    if (first_load_flags & FIRST_SHADER_LOAD) {
      UNI_MODEL.initBuffer(id, "uniModel");
      UNI_VIEW_PROJ.initBuffer(id, "uniViewProj");
      first_load_flags &= ~FIRST_SHADER_LOAD;
    } else {
      UNI_MODEL.keepBuffer(id, "uniModel");
      UNI_VIEW_PROJ.keepBuffer(id, "uniViewProj");
    }
    PEACE_GL_CHECK_ERROR;
    if (flags & SHADER_2D) {
      if (first_load_flags & FIRST_2D_LOAD) {
	UNI_COLOR.initBuffer(id, "uniColor");
	first_load_flags &= ~FIRST_2D_LOAD;
      } else {
	UNI_COLOR.keepBuffer(id, "uniColor");
      }
      UNI_COLOR.registerVal(Vec4f(1,1,1,1));
    }
    if (!(flags & SHADER_2D)) {
      if (first_load_flags & FIRST_3D_LOAD) {
	UNI_DIR_LIGHTS.initBuffer(id, "uniDirLights");
	UNI_AMBIENT.initBuffer(id, "uniAmbient");
	first_load_flags &= ~FIRST_3D_LOAD;
      } else {
	UNI_DIR_LIGHTS.keepBuffer(id, "uniDirLights");
	UNI_AMBIENT.keepBuffer(id, "uniAmbient");
      }
    }
    PEACE_GL_CHECK_ERROR;
    if (flags & SHADER_SKELETAL) {
      //Log::message("0x%x", first_load_flags);
      if (first_load_flags & FIRST_SKELETAL_LOAD) {
	UNI_BONES.initBuffer(id, "uniBones");
	first_load_flags &= ~FIRST_SKELETAL_LOAD;
      } else {
	UNI_BONES.keepBuffer(id, "uniBones");
      }
    }
  }

  void Shader::use() {
    if (cur_shader != this) {
      _use_no_check();
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
