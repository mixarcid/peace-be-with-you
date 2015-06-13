#include "VertexObjects.hpp"
#include "Shader.hpp"

NAMESPACE {

  unsigned int VAO::length = 0;
  GLuint* VAO::ids = NULL;
  bool VAO::is_initialized  = 0;
  unsigned int VAO::current = 0;

  void VAO::use() {
    debugAssert(index < VAO::length, "VAO index out of bounds");
    if (VAO::current != index) {
      glBindVertexArray(VAO::ids[index]);
      VAO::current = index;
    }
  }

  void VAO::registerVars(Array<ShaderVar> vars) {
    Array<size_t> offsets;
    size_t total_size = 0;

    for (ShaderVar var : vars) {
      offsets.push_back(total_size);
      total_size += var.info->size;
    }
    for (unsigned int n=0; n<vars.size(); ++n) {
      glEnableVertexAttribArray(vars[n].id);
      glVertexAttribPointer(vars[n].id, vars[n].info->elem_num,
			    vars[n].info->elem_type, GL_FALSE,
			    total_size, (void*) offsets[n]);
    }
  }
  
  void VAO::init(unsigned int num_vaos) {
    glGenVertexArrays(length, ids);
    VAO::length = num_vaos;
    VAO::ids = new GLuint[length];
    VAO::is_initialized = true;
  }

  VAO VAO::getVAO() {
    static unsigned int last_index = 0;
    VAO ret;
    ret.index = last_index++;
    return ret;
  }
  
  void VAO::terminate() {
    if (VAO::ids != NULL && VAO::is_initialized) {
      glDeleteVertexArrays(length, ids);
      delete ids;
    } else {
      log::error("Why are you calling VAO::terminate() without"
		 " calling VAO::init()?");
    }
  }


  void VBO::init(unsigned int num_vbos) {
    length = num_vbos;
    ids = new GLuint[length];
    lengths = new unsigned int[length];
    glGenBuffers(length, ids);
  }
    
  /*VBO VBO::getVBO(unsigned int num_vbos) {
    VBO ret;
    ret.length = num_vbos;
    ret.ids = new GLuint[ret.length];
    ret.lengths = new unsigned int[ret.length];
    glGenBuffers(ret.length, ret.ids);
    return ret;
  }*/

  VBO::~VBO() {

    if (ids != NULL) {
      glDeleteBuffers(length, ids);
      delete ids;
    }
    delete lengths;
  }

  void VBO::draw(GLenum mode, unsigned int index) {
    glDrawArrays(mode, 0, lengths[index]);
  }


  void EBO::init(unsigned int num_vbos) {
    length = num_vbos;
    ids = new GLuint[length];
    lengths = new unsigned int[length];
    glGenBuffers(length, ids);
  }

  /*EBO EBO::getEBO(unsigned int num_ebos) {
    EBO ret;
    ret.length = num_ebos;
    ret.ids = new GLuint[ret.length];
    ret.lengths = new unsigned int[ret.length];
    glGenBuffers(ret.length, ret.ids);
    return ret;
    }*/

  void EBO::bindArray(Array<GLuint> arr, bool dynamic,
		      unsigned int index) {
    lengths[index] = arr.size();
    //log::message("EBO ID: %u", ids[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 lengths[index]*sizeof(GLuint), &arr[0],
		 dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  }

  /*void EBO::rebindArray(Array<GLuint> arr, bool dynamic,
		   unsigned int index) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 lengths[index], &arr[0],
		 dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  }*/

  void EBO::draw(GLenum mode, unsigned int index) {
    //log::message("%u", lengths[index]);
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
