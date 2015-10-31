#include "VertexObjects.hpp"
#include "Shader.hpp"

NAMESPACE {

  void VAO::init(u32 num_vaos) {
    length = num_vaos;
    ids = new u32[length];
    glGenVertexArrays(length, ids);
  }

  void VAO::use(u32 index) {
    debugAssert(index < length, "VAO index out of bounds");
    //PEACE_GL_CHECK_ERROR;
    glBindVertexArray(ids[index]);
    //PEACE_GL_CHECK_ERROR;
  }

  void VAO::registerVars(Array<ShaderVar> vars, u32 index) {
    Array<size_t> offsets;
    size_t total_size = 0;

    for (ShaderVar var : vars) {
      offsets.push_back(total_size);
      total_size += var.info->size;
    }
    //Log::message("%u", vars.size());
    for (u32 n=0; n<vars.size(); ++n) {
      glEnableVertexAttribArray(vars[n].id);
      if (vars[n].info->elem_type == GL_UNSIGNED_INT
	  || vars[n].info->elem_type == GL_INT) {
	glVertexAttribIPointer(vars[n].id,
			       vars[n].info->elem_num,
			       vars[n].info->elem_type,
			       total_size,
			       (void*) offsets[n]);
      } else {
	glVertexAttribPointer(vars[n].id,
			      vars[n].info->elem_num,
			      vars[n].info->elem_type,
			      GL_FALSE,
			      total_size,
			      (void*) offsets[n]);
      }
    }
  }

  VAO::~VAO() {
    glDeleteVertexArrays(length, ids);
    delete[] ids;
  }

  /*VAO VAO::operator=(const VAO b) {
    glDeleteVertexArrays(length, ids);
    delete[] ids;
    
    }*/

  void VBO::init(u32 num_vbos) {
    length = num_vbos;
    ids = new u32[length];
    lengths = new u32[length];
    glGenBuffers(length, ids);
  }

  VBO::~VBO() {
    if (ids != NULL) {
      glDeleteBuffers(length, ids);
      delete[] ids;
    }
    if (lengths != NULL) {
      delete[] lengths;
    }
  }

  void VBO::draw(GLenum mode, u32 index) {
    glDrawArrays(mode, 0, lengths[index]);
  }


  void EBO::init(u32 num_vbos) {
    length = num_vbos;
    ids = new u32[length];
    lengths = new u32[length];
    glGenBuffers(length, ids);
    //Log::message("init ids: %p", ids);
    //Log::message("init lengths: %p", lengths);
  }

  /*EBO EBO::getEBO(u32 num_ebos) {
    EBO ret;
    ret.length = num_ebos;
    ret.ids = new u32[ret.length];
    ret.lengths = new u32[ret.length];
    glGenBuffers(ret.length, ret.ids);
    return ret;
    }*/

  void EBO::bindArray(Array<u32> arr, bool dynamic, u32 index) {
    lengths[index] = arr.size();
    //Log::message("EBO ID: %u", ids[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 lengths[index]*sizeof(u32), &arr[0],
		 dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  }

  /*void EBO::rebindArray(Array<u32> arr, bool dynamic,
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
    //Log::message("del ids: %p", ids);
    //Log::message("del lengths: %p", lengths);
    if (ids != NULL) {
      glDeleteBuffers(length, ids);
      delete[] ids;
    }
    if (lengths != NULL) {
      delete[] lengths;
    }
  }

}
