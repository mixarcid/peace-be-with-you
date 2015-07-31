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

  ShaderUniform Shader::UNI_TEXTURE(0);
  ShaderUniform Shader::UNI_MODEL(1);
  ShaderUniform Shader::UNI_VIEW(2);
  ShaderUniform Shader::UNI_PROJ(3);
  ShaderUniform Shader::UNI_BONES(4);
  ShaderUniform Shader::UNI_FLAGS(5);

  const u8 Shader::MAX_BONES;
  const u8 Shader::MAX_BONES_PER_VERTEX;
  
  const static char* SHADER_HEADER_VERT = DIR_SHADER_HEADER ".vs";
  const static char* SHADER_HEADER_FRAG = DIR_SHADER_HEADER ".fs";
  const static i32 MAX_CHARS_IN_FILE = 5000;
  const static i32 MAX_LOG_SIZE = 1000;

  static String vert_header;
  static String frag_header;
  static bool first_shader_load = true;

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

  ShaderTypeInfo::ShaderTypeInfo(GLenum type_elem, GLint num_elem,
				 size_t total_size) {
    debugAssert(num_elem < 5,
		"ShaderTypeInfo elem_num must be less then 5");
    elem_type = type_elem;
    elem_num = num_elem;
    size = total_size;
  }

  ShaderVar::ShaderVar(GLint var_id, ShaderTypeName var_type) {
    id = var_id;
    debugAssert(var_type < TYPE_LAST,
		"ShaderVarType index out of bounds");
    info = &SHADER_TYPES[var_type];
  }

  ShaderUniform::ShaderUniform(GLint uniform_id) {
    id = uniform_id;
  }

  void ShaderUniform::registerInt(GLint i) const {
    glUniform1i(id, i);
  }

  void ShaderUniform::registerMat4f(Mat4f mat) const {
    glUniformMatrix4fv(id, 1, GL_FALSE, mat.data);
  }

  Shader::Shader(const String filename) {

    if (first_shader_load) {
      vert_header = getFileContents(SHADER_HEADER_VERT);
      frag_header = getFileContents(SHADER_HEADER_FRAG);
    }

    String vert_str = vert_header + "\n"
      + getFileContents((DIR_SHADERS + filename
			 + DIR_VERT_EXTENSION).c_str());
    String frag_str = frag_header + "\n"
      + getFileContents((DIR_SHADERS + filename +
			 DIR_FRAG_EXTENSION).c_str());
    const char* vert_source = vert_str.c_str();
    const char* frag_source = frag_str.c_str();
    //Log::message("%s", vert_source);

    u32 vert_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_id, 1, &vert_source, NULL);
    glCompileShader(vert_id);

    u32 frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_id, 1, &frag_source, NULL);
    glCompileShader(frag_id);

    char c_shader_Log[MAX_LOG_SIZE];
    glGetShaderInfoLog(vert_id, MAX_LOG_SIZE, NULL, c_shader_Log);
    String shader_Log = c_shader_Log;
    fatalAssert(shader_Log.length() == 0,
	       "Error compiling %s.vs: %s",
	       filename.c_str(), c_shader_Log);
    glGetShaderInfoLog(frag_id, MAX_LOG_SIZE, NULL, c_shader_Log);
    shader_Log = c_shader_Log;
    fatalAssert(shader_Log.length() == 0,
	       "Error compiling %s.fs: %s",
	       filename.c_str(), c_shader_Log);

    id = glCreateProgram();
    glAttachShader(id, vert_id);
    glAttachShader(id, frag_id);

    glBindFragDataLocation(id, 0, "outColor");
    glBindAttribLocation(id, POSITION.id, "inPosition");
    glBindAttribLocation(id, COLOR.id, "inColor");
    glBindAttribLocation(id, TEX_COORD.id, "inTexCoord");
    glBindAttribLocation(id, NORMAL.id, "inNormal");
    glBindAttribLocation(id, BONE_INDEXES0.id, "inBoneIndexes0");
    glBindAttribLocation(id, BONE_WEIGHTS0.id, "inBoneWeights0");

    glLinkProgram(id);
    glDeleteShader(vert_id);
    glDeleteShader(frag_id);

    //Load all the uniform buffer objects
    if (first_shader_load) {
      UNI_BONES.block_id = glGetUniformBlockIndex(id, "uniBoneData");
      glGenBuffers(1, &UNI_BONES.buffer_id);
      glBindBuffer(GL_UNIFORM_BUFFER, UNI_BONES.buffer_id);
      first_shader_load = false;
    }
    
    Log::message("Loaded shader %s", filename.c_str());
  }

  void Shader::use() {
    UNI_TEXTURE.id = glGetUniformLocation(id, "uniTexture");
    UNI_MODEL.id = glGetUniformLocation(id, "uniModel");
    UNI_VIEW.id = glGetUniformLocation(id, "uniView");
    UNI_PROJ.id = glGetUniformLocation(id, "uniProj");
    UNI_FLAGS.id = glGetUniformLocation(id, "uniFlags");
    glUseProgram(id);
  }

  ShaderVar Shader::getVar(const char* name,
			   ShaderTypeName type) {
    GLint var_id = glGetAttribLocation(id, name);
    return ShaderVar(var_id, type);
  }

  ShaderUniform Shader::getUniform(const char* name) {
    return ShaderUniform(glGetUniformLocation(id, name));
  }

  Shader::~Shader() {
    glDeleteProgram(id);
  }
}
