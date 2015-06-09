#pragma once

#include "Standard.h"
#include "Containers.h"
#include "VertexObjects.h"
#include "Shader.h"

NAMESPACE {

  struct ShaderVar;

  template <typename T>
    struct Renderable {

    Array<T> data;
    VBO vbo;

    Renderable() {
      if (Renderable<T>::firstVAO) {
	Renderable<T>::vao = VAO::getNextVAOIndex();
	firstVAO = false;
      }
      VAO::all_vaos.use(Renderable<T>::vao);
    }
    void init(Array<ShaderVar> vars, bool isDynamic) {
      vbo.bindArray(data, isDynamic);
      VAO::all_vaos.registerVars(vars, Renderable<T>::vao);
    }
    virtual void render() {
      VAO::all_vaos.use(Renderable<T>::vao);
      vbo.draw(GL_TRIANGLES);
    }
    //virtual ~Renderable() = 0;

    static unsigned int vao;
    static bool firstVAO;
    
  };

  template <typename T>
    unsigned int Renderable<T>::vao;
  template <typename T>
    bool Renderable<T>::firstVAO = true;

  template <typename T>
    struct RenderableEBO : public Renderable<T> {

    EBO ebo;
    Array<GLuint> elements;

    using Renderable<T>::Renderable;
    void init(Array<ShaderVar> vars, bool isDynamic) {
      Renderable<T>::vbo.bindArray(Renderable<T>::data, isDynamic);
      ebo.bindArray(elements, isDynamic);
      VAO::all_vaos.registerVars(vars, Renderable<T>::vao);
    }
    virtual void render() {
      VAO::all_vaos.use(Renderable<T>::vao);
      ebo.draw(GL_TRIANGLES);
    }
    
  };
  
}
