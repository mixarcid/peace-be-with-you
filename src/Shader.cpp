#include "Shader.hpp"
#include "Color.hpp"
#include "FileSystem.hpp"

NAMESPACE {

  const static ShaderTypeInfo SHADER_TYPES[TYPE_LAST] = {
    ShaderTypeInfo(GL_FLOAT, 2, sizeof(Vec2f)),
    ShaderTypeInfo(GL_FLOAT, 3, sizeof(Vec3f)),
    ShaderTypeInfo(GL_FLOAT, 4, sizeof(Color4f)),
  };
  
  const ShaderVar Shader::POSITION(0, TYPE_VECTOR3F);
  const ShaderVar Shader::COLOR(1, TYPE_COLOR4F);
  const ShaderVar Shader::TEX_COORD(2, TYPE_VECTOR2F);
  const ShaderVar Shader::NORMAL(3, TYPE_VECTOR3F);

  const ShaderUniform Shader::UNI_TEXTURE(0);
  const ShaderUniform Shader::UNI_MODEL(1);
  const ShaderUniform Shader::UNI_VIEW(2);
  const ShaderUniform Shader::UNI_PROJ(3);
  
  const static char* SHADER_HEADER_VERT = DIR_SHADER_HEADER ".vs";
  const static char* SHADER_HEADER_FRAG = DIR_SHADER_HEADER ".fs";
  const static int MAX_CHARS_IN_FILE = 5000;
  const static int MAX_LOG_SIZE = 1000;

  static String vert_header;
  static String frag_header;
  static bool first_shader_load = true;

  static String getFileContents(const char* filename) {
    
    char contents[MAX_CHARS_IN_FILE];
    FILE* file = fopen(filename, "r");
    exitAssert(file != NULL,
	       "Error opening %s", filename);
    int line_num = -1;

    while (!feof(file)) {
      exitAssert(line_num < MAX_CHARS_IN_FILE,
		 "File %s is too long", filename);
      contents[++line_num] = fgetc(file);
    }
    contents[line_num] = '\0';
    exitAssert(contents != NULL,
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
      first_shader_load = false;
    }

    String vert_str = vert_header + "\n"
      + getFileContents((DIR_SHADERS + filename
			 + DIR_VERT_EXTENSION).c_str());
    String frag_str = frag_header + "\n"
      + getFileContents((DIR_SHADERS + filename +
			 DIR_FRAG_EXTENSION).c_str());
    const char* vert_source = vert_str.c_str();
    const char* frag_source = frag_str.c_str();
    //log::message("%s", vert_source);

    GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_id, 1, &vert_source, NULL);
    glCompileShader(vert_id);

    GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_id, 1, &frag_source, NULL);
    glCompileShader(frag_id);

    char c_shader_log[MAX_LOG_SIZE];
    glGetShaderInfoLog(vert_id, MAX_LOG_SIZE, NULL, c_shader_log);
    String shader_log = c_shader_log;
    exitAssert(shader_log.length() == 0,
	       "Error compiling %s.vs: %s",
	       filename.c_str(), c_shader_log);
    glGetShaderInfoLog(frag_id, MAX_LOG_SIZE, NULL, c_shader_log);
    shader_log = c_shader_log;
    exitAssert(shader_log.length() == 0,
	       "Error compiling %s.fs: %s",
	       filename.c_str(), c_shader_log);

    id = glCreateProgram();
    glAttachShader(id, vert_id);
    glAttachShader(id, frag_id);
    
    glBindFragDataLocation(id, 0, "outColor");
    glBindAttribLocation(id, POSITION.id, "inPosition");
    glBindAttribLocation(id, COLOR.id, "inColor");
    glBindAttribLocation(id, TEX_COORD.id, "inTexCoord");
    glBindAttribLocation(id, NORMAL.id, "inNormal");

    //UNI_TEXTURE = glGetUniformLocation(id, "uniTexture");
    
    glLinkProgram(id);

    glGetProgramInfoLog(frag_id, MAX_LOG_SIZE, NULL, c_shader_log);
    shader_log = c_shader_log;
    exitAssert(shader_log.length() == 0,
	       "Error linking shader %s: %s",
	       filename.c_str(), c_shader_log);

    glDeleteShader(vert_id);
    glDeleteShader(frag_id);

    log::message("Loaded shader %s", filename.c_str());
  }

  void Shader::use() {
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
