#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Standard.h"

NAMESPACE {

  struct VAO {

    unsigned int length;
    GLuint* ids;

    VAO(unsigned int num_vaos) {
      length = num_vaos;
      ids = new GLuint[length];
      glGenVertexArrays(length, ids);
    }

    void use(unsigned int index) {
      assert(index < length, "VAO index out of bounds");
      glBindVertexArray(ids[index]);
    }

    void remove() {
      glDeleteVertexArrays(length, ids);
      delete ids;
    }
  };

  struct VBO {

  };
  
}
