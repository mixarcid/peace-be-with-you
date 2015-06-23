#include "VertexObjects.hpp"
#include "Shader.hpp"

NAMESPACE {

  void VAO::init(u32 num_vaos) {
    length = num_vaos;
    ids = new GLuint[length];
    glGenVertexArrays(length, ids);
  }

  void VAO::use(u32 index) {
    debugAssert(index < length, "VAO index out of bounds");
    glBindVertexArray(ids[index]);
  }

  void VAO::registerVars(Array<ShaderVar> vars,
			 u32 index) {
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

  void VBO::init(u32 num_vbos) {
    length = num_vbos;
    ids = new GLuint[length];
    lengths = new unsigned int[length];
    glGenBuffers(length, ids);
  }

  VBO::~VBO() {

    if (ids != NULL) {
      glDeleteBuffers(length, ids);
      delete ids;
    }
    delete lengths;
  }

  void VBO::draw(GLenum mode, u32 index) {
    glDrawArrays(mode, 0, lengths[index]);
  }


  void EBO::init(u32 num_vbos) {
    length = num_vbos;
    ids = new GLuint[length];
    lengths = new unsigned int[length];
    glGenBuffers(length, ids);
  }

  /*EBO EBO::getEBO(u32 num_ebos) {
    EBO ret;
    ret.length = num_ebos;
    ret.ids = new GLuint[ret.length];
    ret.lengths = new unsigned int[ret.length];
    glGenBuffers(ret.length, ret.ids);
    return ret;
    }*/

  void EBO::bindArray(Array<GLuint> arr, bool dynamic,
		      u32 index) {
    lengths[index] = arr.size();
    //Log::message("EBO ID: %u", ids[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 lengths[index]*sizeof(GLuint), &arr[0],
		 dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  }

  /*void EBO::rebindArray(Array<GLuint> arr, bool dynamic,
		   u32 index) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 lengths[index], &arr[0],
		 dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  }*/

  void EBO::draw(GLenum mode, u32 index) {
    //Log::message("%u", lengths[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[index]);
    glDrawElements(mode, lengths[index], GL_UNSIGNED_INT, NULL);
  }

  EBO::~EBO() {
    if (ids != NULL) {
      glDeleteBuffers(length, ids);
      delete ids;
    }
    delete lengths;
  }

}
