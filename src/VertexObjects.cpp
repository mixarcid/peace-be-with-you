#include "VertexObjects.h"
#include "Shader.h"

NAMESPACE {

  VAO* VAO::current_vao = NULL;
  VAO VAO::all_vaos(NUM_VAOS, false);
  
  VAO::VAO(unsigned int num_vaos, bool initialize) {
    length = num_vaos;
    ids = new GLuint[length];
    initialize = is_initialized;
    if (initialize) {
      init();
    }
  }

  void VAO::init() {
    glGenVertexArrays(length, ids);
  }

  void VAO::use(unsigned int index) {
    debugAssert(index < length, "VAO index out of bounds");
    if (VAO::current_vao != this) {
      glBindVertexArray(ids[index]);
      VAO::current_vao = this;
    }
  }

  void VAO::registerVars(Array<ShaderVar> vars,
			 unsigned int index) {
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

  VAO::~VAO() {
    if (ids != NULL && is_initialized) {
      glDeleteVertexArrays(length, ids);
      delete ids;
    }
  }

  unsigned int VAO::getNextVAOIndex() {
    static unsigned int last_index = 0;
    return last_index++;
  }


  VBO::VBO(unsigned int num_vaos) {
    length = num_vaos;
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

  void VBO::draw(GLenum mode, unsigned int index) {
    glDrawArrays(mode, 0, lengths[index]);
  }

  EBO::EBO(unsigned int num_ebos) {
    length = num_ebos;
    ids = new GLuint[length];
    lengths = new unsigned int[length];
    glGenBuffers(length, ids);
  }

  void EBO::bindArray(Array<GLuint> arr, bool dynamic,
		      unsigned int index) {
    lengths[index] = arr.size()*sizeof(GLuint);
    //log::message("%u", arr.size()*sizeof(GLuint));
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
    //log::message("%u", lengths[index]);
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
