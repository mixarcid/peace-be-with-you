#include "VertexObjects.h"
#include "Shader.h"

NAMESPACE {
  
  VAO::VAO(unsigned int num_vaos) {
    length = num_vaos;
    ids = new GLuint[length];
    glGenVertexArrays(length, ids);
  }

  void VAO::use(unsigned int index) {
    debugAssert(index < length, "VAO index out of bounds");
    glBindVertexArray(ids[index]);
  }

  void VAO::registerVars(Array<ShaderVar> vars,
			 unsigned int index) {
    Array<size_t> offsets;
    size_t total_size = 0;

    for (ShaderVar var : vars) {
      offsets.push_back(total_size);
      total_size += var.info->size;
    }
    for (int n=0; n<vars.size(); ++n) {
      glEnableVertexAttribArray(vars[n].id);
      glVertexAttribPointer(vars[n].id, vars[n].info->elem_num,
			    vars[n].info->elem_type, GL_FALSE,
			    total_size, (void*) offsets[n]);
    }
  }

  VAO::~VAO() {
    glDeleteVertexArrays(length, ids);
    delete ids;
  }


  VBO::VBO(unsigned int num_vaos) {
    length = num_vaos;
    ids = new GLuint[length];
    lengths = new unsigned int[length];
    glGenBuffers(length, ids);
  }

  VBO::~VBO() {
    glDeleteBuffers(length, ids);
    delete lengths;
    delete ids;
  }

  void VBO::draw(GLenum mode, unsigned int index) {
    glDrawArrays(mode, 0, lengths[index]);
  }

  EBO::EBO(unsigned int num_vaos) {
    length = num_vaos;
    ids = new GLuint[length];
    lengths = new unsigned int[length];
    glGenBuffers(length, ids);
  }

  void EBO::bindArray(Array<GLuint> arr, bool dynamic,
		      unsigned int index) {
    lengths[index] = arr.size()*sizeof(GLuint);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, lengths[index], &arr[0],
		 dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  }

  void EBO::rebindArray(Array<GLuint> arr, bool dynamic,
		   unsigned int index) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 lengths[index], &arr[0],
		 dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  }

  void EBO::draw(GLenum mode, unsigned int index) {
    glDrawElements(mode, lengths[index], GL_UNSIGNED_INT, NULL);
  }

  EBO::~EBO() {
    glDeleteBuffers(length, ids);
    delete lengths;
    delete ids;
  }

}
