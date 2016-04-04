#include "GLObject.hpp"
#include "Shader.hpp"

NAMESPACE {

  GLObject::GLObject()
    : flags(PEACE_GL_UNINITIALIZED) {}
  
  GLObject::GLObject(const GLObject& b)
    : id(b.id), flags(b.flags | PEACE_GL_IS_COPY) {}
  
  GLObject::GLObject(GLObject&& b)
    : id(b.id), flags(b.flags) {
    b.flags |= PEACE_GL_UNINITIALIZED;
  }

  GLObject::~GLObject() {
    flags |= PEACE_GL_UNINITIALIZED;
  }
  
  GLObject& GLObject::operator=(const GLObject& b) {
    this->~GLObject();
    id = b.id;
    flags = b.flags | PEACE_GL_IS_COPY;
    return *this;
  }

  GLObject& GLObject::operator=(GLObject&& b) {
    this->~GLObject();
    id = b.id;
    flags = b.flags;
    b.flags |= PEACE_GL_UNINITIALIZED;
    return *this;
  }

  void GLObject::init() {
    this->~GLObject();
    flags &= ~PEACE_GL_UNINITIALIZED;
  }

  void VAO::init() {
    GLObject::init();
    glGenVertexArrays(1, &this->id);
  }

  VAO::~VAO() {
    if (PEACE_GL_SHOULD_DELETE) {
      glDeleteVertexArrays(1, &this->id);
    }
  }

  void VAO::use() {
    debugAssert(!(this->flags & PEACE_GL_UNINITIALIZED),
		"You must initialize a VAO before using it");
    glBindVertexArray(id);
  }

  void VAO::registerVars(Array<ShaderVar> vars) {
    
    Array<size_t> offsets;
    size_t total_size = 0;

    use();
    
    for (ShaderVar var : vars) {
      offsets.push_back(total_size);
      total_size += var.info->size;
    }

    for (u32 n=0; n<vars.size(); ++n) {
      glEnableVertexAttribArray(vars[n].id);
      if (vars[n].flags & SHADER_VAR_KEEP_INT) {
	glVertexAttribIPointer(vars[n].id,
			       vars[n].info->elem_num,
			       vars[n].info->elem_type,
			       total_size,
			       (void*) offsets[n]);
      } else {
	glVertexAttribPointer(vars[n].id,
			      vars[n].info->elem_num,
			      vars[n].info->elem_type,
			      vars[n].flags & SHADER_VAR_NORMALIZE,
			      total_size,
			      (void*) offsets[n]);
      }
    }
  }

  VBO::VBO() : length(0) {}

  void VBO::init() {
    GLObject::init();
    glGenBuffers(1, &this->id);
  }

  VBO::~VBO() {
    if (PEACE_GL_SHOULD_DELETE) {
      glDeleteBuffers(1, &this->id);
    }
  }

  void VBO::use() {
    debugAssert(!(this->flags & PEACE_GL_UNINITIALIZED),
		"You must initialize a VBO before using it");
    glBindBuffer(GL_ARRAY_BUFFER, id);
  }

  void VBO::draw(GLenum mode) {
    debugAssert(!(this->flags & PEACE_GL_UNINITIALIZED),
		"You must initialize a VBO before drawing with it");
    glDrawArrays(mode, 0, length);
  }

  EBO::EBO() : length(0) {}

  void EBO::init() {
    GLObject::init();
    glGenBuffers(1, &this->id);
  }

  EBO::~EBO() {
    if (PEACE_GL_SHOULD_DELETE) {
      glDeleteBuffers(1, &this->id);
    }
  }

  void EBO::bindArray(Array<u32>& arr, bool dynamic) {
    debugAssert(!(this->flags & PEACE_GL_UNINITIALIZED),
		"You must initialize an EBO before binding it");
    length = arr.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 length*sizeof(u32), arr.begin(),
		 dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  }
  
  void EBO::draw(GLenum mode) {
    debugAssert(!(this->flags & PEACE_GL_UNINITIALIZED),
		"You must initialize an EBO before drawing with it");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glDrawElements(mode, length, GL_UNSIGNED_INT, NULL);
  }

}
