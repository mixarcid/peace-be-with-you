#pragma once

#include "Standard.hpp"
#include "Containers.hpp"
#include "VertexObjects.hpp"
#include "Shader.hpp"

NAMESPACE {

  struct ShaderVar;

  template <typename T>
    struct Renderable {

    Array<T> data;
    VBO vbo;
    VAO vao;
    
    void init(Array<ShaderVar> vars, bool isDynamic) {
      /*if (Renderable<T>::firstVAO) {
	Renderable<T>::vao = VAO::getVAO();
	Renderable<T>::firstVAO = false;
	}*/
      vao = VAO::getVAO();
      vao.use();
      vbo.bindArray(data, isDynamic);
      vao.registerVars(vars);
    }
    virtual void render() {
      Renderable<T>::vao.use();
      vbo.draw(GL_TRIANGLES);
    }
    virtual ~Renderable() {}

    //static VAO vao;
    //static bool firstVAO;
    
  };

  /*template <typename T>
    VAO Renderable<T>::vao;
  template <typename T>
  bool Renderable<T>::firstVAO = true;*/

  template <typename T>
    struct RenderableEBO : public Renderable<T> {

    EBO ebo;
    Array<GLuint> elements;

    void init(Array<ShaderVar> vars, bool isDynamic) {
      /*if (Renderable<T>::firstVAO) {
	Renderable<T>::vao = VAO::getVAO();
	Renderable<T>::firstVAO = false;
	}*/
      Renderable<T>::vao = VAO::getVAO();
      Renderable<T>::vao.use();
      Renderable<T>::vbo.bindArray(Renderable<T>::data, isDynamic);
      ebo.bindArray(elements, isDynamic);
      Renderable<T>::vao.registerVars(vars);
    }
    virtual void render() {
      Renderable<T>::vao.use();
      ebo.draw(GL_TRIANGLES);
    }
    
  };
  
}
